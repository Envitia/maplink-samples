// C2DemoDoc.cpp : implementation of the CC2DemoDoc class
//

#include "stdafx.h"
#include "C2DemoDoc.h"
#include "Animator.h"

#include "TrackManager.h"

#include "OwnShipTrack.h"
#include "RangeRing.h"
#include "AircraftCarrier.h"
#include "Bomber.h"
#include "Destroyer.h"
#include "Frigate.h"
#include "GeneralAir.h"
#include "Helicopter.h"
#include "Interceptor.h"
#include "Submarine.h"
#include "Unknown.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CC2DemoDoc

IMPLEMENT_DYNCREATE(CC2DemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CC2DemoDoc, CDocument)
//{{AFX_MSG_MAP(CC2DemoDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CC2DemoDoc construction/destruction

CC2DemoDoc::CC2DemoDoc()
{
  m_mapDataLayer      = 0 ;
  m_standardDataLayer = 0 ;
  m_objectDataLayer   = 0 ;
  m_animator	      = new CAnimator() ;

  ////////////////////////////////////////////////
  // Handle any errors during initialisation

  const char * errMsg = TSLErrorStack::errorString( "Errors occurred during initialisation\n" ) ;
  
  if ( errMsg )
  {
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(errMsg), MB_OK ) ;
    TSLErrorStack::clear() ;
  }
  
}

CC2DemoDoc::~CC2DemoDoc()
{
  if ( m_animator ) 
  {
    delete m_animator ;
    m_animator = 0 ; 
  }

}

/////////////////////////////////////////////////////////////////////////////
// CC2DemoDoc diagnostics

#ifdef _DEBUG
void CC2DemoDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CC2DemoDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CC2DemoDoc commands

BOOL CC2DemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  
  // Create a map data layer
  m_mapDataLayer = new TSLMapDataLayer() ;

  // Create a standard data layer
  m_standardDataLayer = new TSLStandardDataLayer() ;

  // Create an object data layer
  m_objectDataLayer = new TSLObjectDataLayer() ;

  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;
  m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;


  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layers and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_standardDataLayer->destroy() ;
    m_objectDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
    m_standardDataLayer = 0 ;
    m_objectDataLayer = 0 ;
    return FALSE ;
  }

  // Work out the TMC extent for the map data layer and then set
  // the tmc per metre value tracks....
  TSLTMC	  tx1, ty1, tx2, ty2 ;
  m_mapDataLayer->getTMCExtent( &tx1, &ty1, &tx2, &ty2 ) ;

  // Set the static members for the manager class for the TMC extent of the map 
  // ( useful for when the tracks meet the edge of the map ) and also the TMCs per
  // metre for the current map
  CTrackManager::setTMCExtent( tx1, ty1, tx2, ty2 ) ;
  CTrackManager::tmcPerMetre( m_mapDataLayer->TMCperMU() ) ;

  // Create the DDOs and place them on the object data layer
  CTrack * pTrack ;

  // Create the OwnShip DDO and add it to the object data layer
  m_ownShipTrack = new COwnShipTrack() ;
  
  m_ownShipTrack->move( 215000000, 0 ) ;
  m_objectDataLayer->addDDO( m_ownShipTrack ) ;

  // Create the RangeRing DDO & set the extent to cover the whole map
  m_rangeRing = new CRangeRing() ;
  m_rangeRing->setExtent(  tx1, ty1, tx2, ty2 ) ;
  
  // Register the Own Ship with the range ring DDO
  m_rangeRing->ownShipTrack( m_ownShipTrack ) ;
  // Add the range ring DDO to the object data layer
  m_objectDataLayer->addDDO( m_rangeRing ) ;

  // Register the Own Ship with the TrackManager
  CTrackManager::ownShip( m_ownShipTrack ) ;

  // Create some tracks
  for ( int i = 0; i < 30 ; i++ ) 
  {
    switch( CTrackManager::random( 0, NUM_TRACK_TYPES) )
    {
      case AIRCRAFT_CARRIER :
        pTrack = new CAircraftCarrier( ) ;
        break ;   
      case BOMBER :
        pTrack = new CBomber( ) ;
        break ;   
      case DESTROYER :
        pTrack = new CDestroyer( ) ;
        break ;   
      case FRIGATE :
        pTrack = new CFrigate( ) ;
        break ;   
      case GENERAL_AIR :
        pTrack = new CGeneralAir( ) ;
        break ;   
      case HELICOPTER :
        pTrack = new CHelicopter( ) ;
        break ;   
      case INTERCEPTOR :
        pTrack = new CInterceptor( ) ;
        break ;   
      case SUBMARINE :
        pTrack = new CSubmarine( ) ;
        break ;   
      case UNKNOWN_TRACK :
      default:
        pTrack = new CUnknown( ) ;
        break ;   
    }

    pTrack->move( CTrackManager::random( tx1, tx2 ), CTrackManager::random( ty1, ty2 ) ) ;
    m_objectDataLayer->addDDO( pTrack ) ;

    // Set the hostility for this track randomly
    pTrack->hostility( (HOSTILE_TYPE)CTrackManager::random( 0, 3 ) ) ;
  }

  // Add the map features to the tree control in the Tree View
  UpdateAllViews( 0, HINT_NEW_MAP_LOADED, 0 ) ;
  
  return TRUE;
}

void CC2DemoDoc::DeleteContents() 
{
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
  if ( m_standardDataLayer )
  {
    m_standardDataLayer->destroy() ;
    m_standardDataLayer = 0 ;
  }
  if ( m_objectDataLayer )
  {
    m_objectDataLayer->destroy() ;
    m_objectDataLayer = 0 ;
  }
	
  CDocument::DeleteContents();
}

void CC2DemoDoc::OnCloseDocument() 
{
  if ( m_animator )
  {
    m_animator->stopAnimation() ;
    delete m_animator ;
    m_animator = 0 ;
  }
  
  CDocument::OnCloseDocument();

}
