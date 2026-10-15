// ddoDoc.cpp : implementation of the CAPP6ADDODoc class
//

#include "stdafx.h"
#include "app6addo.h"
#include <direct.h>

#include "app6addodoc.h"

#include "MainFrm.h"
#include "app6addoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


long CAPP6ADDODoc::m_reset_timer  = true;
long CAPP6ADDODoc::m_ddo_timer_dt = 0;


/////////////////////////////////////////////////////////////////////////////
// CAPP6ADDODoc

IMPLEMENT_DYNCREATE(CAPP6ADDODoc, CDocument)

BEGIN_MESSAGE_MAP(CAPP6ADDODoc, CDocument)
//{{AFX_MSG_MAP(CAPP6ADDODoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAPP6ADDODoc construction/destruction

CAPP6ADDODoc::CAPP6ADDODoc()
{
  m_dataLayer = NULL ;

  m_objectLayer = 0;
}

CAPP6ADDODoc::~CAPP6ADDODoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// CAPP6ADDODoc diagnostics

#ifdef _DEBUG
void CAPP6ADDODoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CAPP6ADDODoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAPP6ADDODoc commands

BOOL CAPP6ADDODoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  
  // Create a map data layer 
  m_dataLayer = new TSLMapDataLayer();
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // load map
  m_dataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;

  // report errors
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_dataLayer->destroy() ;
    m_dataLayer = 0 ;
  }

  m_objectLayer = new TSLObjectDataLayer();
  
  // Calculate how many TMCS per metre (map/user unit)
  double x1, y1, x2, y2;
  
  //get the extent of this newly loaded map in map units and tmcs
  m_dataLayer->getMUExtent( &x1, &y1, &x2, &y2 );
  
  TSLTMC tx1, ty1, tx2, ty2;
  m_dataLayer->getTMCExtent( &tx1, &ty1, &tx2, &ty2 );
  
  double tpm = ((double)tx2 - (double)tx1) / (x2 - x1);
  
  App6aAircraft::tmcPerMeter( tpm );

  typedef std::pair<double, double> LAT_LONG;

  // Lat/Long coordinates (approx.) of various destinations.

  const LAT_LONG airports[] = {
				LAT_LONG(   51.4667,   -0.4500 ),    // London
				LAT_LONG(   40.6667,  -74.0833 ),    // New York
				LAT_LONG(   52.5333,   13.4167 ),    // Berlin
				LAT_LONG(   49.0167,    2.5167 ),    // Paris
				LAT_LONG(   35.6667,  139.7500 ),    // Tokyo
				LAT_LONG(  -33.9167,  151.1667 ),    // Sydney
				LAT_LONG(  -31.9667,  115.8167 ),    // Perth
				LAT_LONG(   37.7500, -122.4500 ),    // San Francisco
				LAT_LONG(  -22.8833,  -43.2833 ),    // Rio de Janeiro
				LAT_LONG(   55.7500,   37.7000 ),    // Moscow
				LAT_LONG(   28.6167,   77.2167 ),    // New Delhi
				LAT_LONG(   39.9167,  116.4333 ),    // Beijing
				LAT_LONG(   33.3333,   44.4333 ),    // Baghdad
				LAT_LONG(  -33.5000,  -70.6667 ),    // Santiago
				LAT_LONG(   64.7167, -157.0000 ),    // Alaska (Galena)
				LAT_LONG(   71.9833,  102.5167 ),    // Russian Fed. (Khatanga)
				LAT_LONG(  -26.1667,   28.0333 )     // Johannesberg
			      };

  const int N_AP = sizeof(airports) / sizeof(LAT_LONG);
  
  // Instantiate some App6aAircraft
  const int Nplanes = 350;
  for ( int i = 0; i < Nplanes; ++i )
  {
    App6aAircraft* plane = new App6aAircraft( App6aAircraft::random( TMS_747, TMS_Cessna ) ) ;
    
    long dep = App6aAircraft::random( 0, N_AP-1 );
    long arr = App6aAircraft::random( 0, N_AP-1 );

    TSLTMC x = 0;
    TSLTMC y = 0;
    
    m_dataLayer->latLongToTMC( airports[dep].first, airports[dep].second, &x, &y );
    
    plane->move( x, y );

    m_dataLayer->latLongToTMC( airports[arr].first, airports[arr].second, &x, &y );

    plane->destination( x, y );
    plane->velocity( App6aAircraft::random(200.0, 800.0) );
    
    m_objectLayer->addDDO( plane );
  }

  m_ddoTimerID = SetTimer( 0, 0, 75, OnUpdate );

  return TRUE;
}

void CAPP6ADDODoc::OnUpdate( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
  CMainFrame* pFrame = (CMainFrame*)  AfxGetMainWnd();
  CAPP6ADDOView  * pView  = (CAPP6ADDOView*)  ( pFrame->GetActiveView() );

 
  if ( !pView->IsKindOf( RUNTIME_CLASS(CPreviewView) ) && !pView->isPrinting() )
  {
    if ( m_reset_timer ) 
    {
      CAPP6ADDODoc::m_ddo_timer_dt = dwTime;
    
      m_reset_timer = false;
    }

    CAPP6ADDODoc   * pDoc   = pView->GetDocument();
  
    TSLObjectDataLayer* objectLayer = pDoc->getObjectLayer();
  
    double rate = 10000.0 / App6aAircraft::tmcPerMeter();
  
    if ( objectLayer ) 
    {
      const TSLlist<TSLDynamicDataObject*>& listDDO = objectLayer->getDDOList();
    
      TSLlist<TSLDynamicDataObject*>::const_iterator it    = listDDO.begin();
      TSLlist<TSLDynamicDataObject*>::const_iterator itEnd = listDDO.end();
    
      while ( it != itEnd ) 
      {
        App6aAircraft* plane = (App6aAircraft*)(*it);
        plane->update( (dwTime-CAPP6ADDODoc::m_ddo_timer_dt)*1.0e-03, rate );
      
        ++it;
      }

      objectLayer->notifyChanged( true );
    }
  
    CAPP6ADDODoc::m_ddo_timer_dt = dwTime;
  
    pDoc->UpdateAllViews( 0 );
  }
  else
  {
    TRACE( "UPDATE IGNORED SINCE PRINTING\n" ) ;
  }
}

void CAPP6ADDODoc::DeleteContents() 
{
  if ( m_dataLayer )
  {
    m_dataLayer->destroy() ;
    m_dataLayer = 0 ;
  }
  if ( m_objectLayer )
  {
    m_objectLayer->destroy();
    m_objectLayer = 0;
  }
  CDocument::DeleteContents();
}
