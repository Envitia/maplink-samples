// C2PerformanceDemoDoc.cpp : implementation of the CC2PerformanceDemoDoc class
//

#include "stdafx.h"
#include "C2PerformanceDemoDoc.h"
#include "Animator.h"
#include "RangeRing.h"
#include "OwnShipTrack.h"
#include "APP6ATrack.h"
#include "TrackManager.h"

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
// CC2PerformanceDemoDoc

IMPLEMENT_DYNCREATE(CC2PerformanceDemoDoc, CDocument)

  BEGIN_MESSAGE_MAP(CC2PerformanceDemoDoc, CDocument)
    //{{AFX_MSG_MAP(CC2PerformanceDemoDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
  END_MESSAGE_MAP()

  /////////////////////////////////////////////////////////////////////////////
  // CC2PerformanceDemoDoc construction/destruction

  CC2PerformanceDemoDoc::CC2PerformanceDemoDoc()
  {
    m_mapDataLayer      = NULL ;
    m_standardDataLayer = NULL ;
    m_objectDataLayer   = NULL ;
    m_rangeRing         = NULL ;
    m_ownShipTrack      = NULL ;
    m_animator	        = new CAnimator() ;
    m_isAPP6A           = false;
    m_is2525B           = false;
    m_NoOfSymbolTracks  = 30;
    m_symbolHelper      = NULL;
    m_latLonGridDataLayer = NULL;
    ////////////////////////////////////////////////
    // Handle any errors during initialisation

    const char * errMsg = TSLErrorStack::errorString( "Errors occurred during initialisation\n" ) ;

    if ( errMsg )
    {
      AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(errMsg), MB_OK ) ;
      TSLErrorStack::clear() ;
    }

  }

  CC2PerformanceDemoDoc::~CC2PerformanceDemoDoc()
  {

    if ( m_symbolHelper )
    {
      m_symbolHelper->destroy();
      m_symbolHelper = NULL;
    }

    if ( m_animator ) 
    {
      delete m_animator ;
      m_animator = NULL ; 
    }

  }

  /////////////////////////////////////////////////////////////////////////////
  // CC2PerformanceDemoDoc diagnostics

#ifdef _DEBUG
  void CC2PerformanceDemoDoc::AssertValid() const
  {
    CDocument::AssertValid();
  }

  void CC2PerformanceDemoDoc::Dump(CDumpContext& dc) const
  {
    CDocument::Dump(dc);
  }
#endif //_DEBUG

  /////////////////////////////////////////////////////////////////////////////
  // CC2PerformanceDemoDoc commands

  BOOL CC2PerformanceDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
  {
    if (!CDocument::OnOpenDocument(lpszPathName))
      return FALSE;


    // Create a map data layer
    m_mapDataLayer = new TSLMapDataLayer() ;

    // Create a standard data layer
    m_standardDataLayer = new TSLStandardDataLayer() ;

    // Create an object data layer
    m_objectDataLayer = new TSLObjectDataLayer() ;

    // Create an grid data-layer
    m_latLonGridDataLayer = new TSLLatLongGridDataLayer() ;

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
      m_latLonGridDataLayer->destroy();
      m_mapDataLayer = NULL ;
      m_standardDataLayer = NULL ;
      m_objectDataLayer = NULL ;
      m_latLonGridDataLayer = NULL;
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

    int tempCopy = m_NoOfSymbolTracks;
    m_NoOfSymbolTracks = 0;

    //this will create all the necessary tracks
    setCurrentNoOfSymbolTracks ( tempCopy );

    // Add the map features to the tree control in the Tree View
    UpdateAllViews( 0, HINT_NEW_MAP_LOADED, 0 ) ;

    return TRUE;
  }

  void CC2PerformanceDemoDoc::DeleteContents() 
  {
    if ( m_latLonGridDataLayer )
    {
      m_latLonGridDataLayer->destroy();
      m_latLonGridDataLayer = NULL;
    }
    if ( m_mapDataLayer )
    {
      m_mapDataLayer->destroy() ;
      m_mapDataLayer = NULL ;
    }
    if ( m_standardDataLayer )
    {
      m_standardDataLayer->destroy() ;
      m_standardDataLayer = NULL ;
    }
    if ( m_objectDataLayer )
    {
      m_objectDataLayer->destroy() ;
      m_objectDataLayer = NULL ;
      m_rangeRing = NULL;
      m_ownShipTrack = NULL;
    }

    CDocument::DeleteContents();
  }

  void CC2PerformanceDemoDoc::OnCloseDocument() 
  {
    if ( m_animator )
    {
      m_animator->stopAnimation() ;
      delete m_animator ;
      m_animator = NULL ;
    }

    CDocument::OnCloseDocument();

  }

  bool CC2PerformanceDemoDoc::changeToAPP6aSymbols ()
  {
    if ( m_isAPP6A )
      return true;

    m_isAPP6A = true;
    m_is2525B = false;

    CTrackManager::selectedTrack(0);
    UpdateAllViews( 0, HINT_UPDATE_SELECTED_TRACK, 0 ) ;

    m_animator->stopAnimation();

    m_objectDataLayer->removeData();

    //removing the data destroys these objects, so update our pointers
    m_ownShipTrack = NULL; 
    m_rangeRing = NULL;

    int tempCopy = m_NoOfSymbolTracks;
    m_NoOfSymbolTracks = 0;

    CTrackManager::resetTrackIDCount();

    if ( m_symbolHelper )
    {
      m_symbolHelper->destroy();
    }
    const char * maplHome = TSLUtilityFunctions::getMapLinkHome() ;
    char buffer[MAX_PATH];
    sprintf(buffer, "%s\\config\\app6aConfig.csv", maplHome);

    m_symbolHelper = new TSLAPP6AHelper(buffer);

    //this will create all the necessary tracks
    setCurrentNoOfSymbolTracks ( tempCopy );

    return true;
  }

  bool CC2PerformanceDemoDoc::changeTo2525BSymbols()
  {
    if ( m_is2525B )
      return true;

    m_isAPP6A = false;
    m_is2525B = true;

    CTrackManager::selectedTrack(0);
    UpdateAllViews( 0, HINT_UPDATE_SELECTED_TRACK, 0 ) ;

    m_animator->stopAnimation();

    m_objectDataLayer->removeData();

    //removing the data destroys these objects, so update our pointers
    m_ownShipTrack = NULL; 
    m_rangeRing = NULL;

    int tempCopy = m_NoOfSymbolTracks;
    m_NoOfSymbolTracks = 0;

    CTrackManager::resetTrackIDCount();

    if ( m_symbolHelper )
    {
      m_symbolHelper->destroy();
    }

    const char * maplHome = TSLUtilityFunctions::getMapLinkHome() ;
    char buffer[MAX_PATH];
    sprintf(buffer, "%s\\config\\2525bConfig.csv", maplHome);

    m_symbolHelper = new TSLAPP6AHelper(buffer);

    //this will create all the necessary tracks
    setCurrentNoOfSymbolTracks ( tempCopy );

    return true;
  }

  bool CC2PerformanceDemoDoc::changeToStandardSymbols ()
  {
    if ( !m_isAPP6A && !m_is2525B )
      return true;

    CTrackManager::selectedTrack(0);
    UpdateAllViews( 0, HINT_UPDATE_SELECTED_TRACK, 0 ) ;

    m_isAPP6A = false;
    m_is2525B = false;

    m_animator->stopAnimation();

    m_objectDataLayer->removeData();

    //removing the data destroys these objects, so update our pointers
    m_ownShipTrack = NULL; 
    m_rangeRing = NULL;

    int tempCopy = m_NoOfSymbolTracks;
    m_NoOfSymbolTracks = 0;

    CTrackManager::resetTrackIDCount();

    if ( m_symbolHelper )
    {
      m_symbolHelper->destroy();
      m_symbolHelper = NULL;
    }

    //this will create all the necessary tracks
    setCurrentNoOfSymbolTracks ( tempCopy );

    return true;
  }

  int CC2PerformanceDemoDoc::getCurrentNoOfSymbolTracks ()
  {
    return m_NoOfSymbolTracks;
  }

  void CC2PerformanceDemoDoc::setCurrentNoOfSymbolTracks ( int newNumber )
  {
    m_animator->stopAnimation();

    TSLTMC    tx1, ty1, tx2, ty2 ;
    m_mapDataLayer->getTMCExtent( &tx1, &ty1, &tx2, &ty2 ) ;

    if ( !m_NoOfSymbolTracks )
    {
      if ( !m_rangeRing )
      {
        // Create the RangeRing DDO & set the extent to cover the whole map
        m_rangeRing = new CRangeRing() ;
        m_rangeRing->setExtent(  tx1, ty1, tx2, ty2 ) ;
      }

      m_objectDataLayer->addDDO( m_rangeRing ) ;

      if ( newNumber )
      {
        // Create the OwnShip DDO and add it to the object data layer
        TSLTMC x = tx1 + (TSLTMC)(fabs((float)tx2 - tx1) / 2.0);
        TSLTMC y = ty1 + (TSLTMC)(fabs((float)ty2 - ty1) / 2.0);

        if ( m_isAPP6A || m_is2525B )
        {
          m_ownShipTrack = new APP6ATrack( m_symbolHelper, FRIENDLY, OWN_SHIP, (int)TSLAPP6ASymbol::TypeSeaSurface ) ;
        }
        else
        {
          m_ownShipTrack = new COwnShipTrack() ;
        }

        m_ownShipTrack->move( x, y ) ;
        m_objectDataLayer->addDDO( m_ownShipTrack ) ;

        // Register the Own Ship with the range ring DDO
        m_rangeRing->ownShipTrack( m_ownShipTrack ) ;
        // Register the Own Ship with the TrackManager
        CTrackManager::ownShip( m_ownShipTrack ) ;
      }
      else
      {
        m_rangeRing->ownShipTrack( NULL );
        CTrackManager::ownShip( NULL ) ;
      }
    }

    //The following code removes or adds ddos depending upon the new
    //number of tracks specified.

    int difference = abs( newNumber - m_NoOfSymbolTracks);

    if ( m_NoOfSymbolTracks < newNumber )
    { //add more ddos

      //remember the owns ship track
      if ( !m_NoOfSymbolTracks )
      {
        --difference;
      }

      CTrack * pTrack ;
      for ( int i = 0; i < difference; ++i )
      {
        int hostility;
        int val = CTrackManager::random( 0, SUBMARINE );

        if ( m_isAPP6A || m_is2525B )
        {
          //APP6A has a number of extra hositlities
          hostility = CTrackManager::random( 0, (int)FAKER );
          //APP6A has an extra type
          int type = CTrackManager::random(1, (int)TSLAPP6ASymbol::TypeInstallation);

          pTrack = new APP6ATrack( m_symbolHelper, (HOSTILE_TYPE)hostility, val, type ) ;
        }
        else
        {
          //Non app6a only has 4 hostility types
          hostility = CTrackManager::random( 0, (int)UNKNOWN );
          switch( val )
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
          pTrack->hostility( (HOSTILE_TYPE) hostility ) ;
        }

        pTrack->move( CTrackManager::random( tx1, tx2 ), CTrackManager::random( ty1, ty2 ) ) ;
        m_objectDataLayer->addDDO( pTrack ) ;
      }
    }
    else
    {
      //remove some ddos
      long ddoCount = m_objectDataLayer->ddoCount();

      if ( m_NoOfSymbolTracks )
      {
        //this should be the number of tracks plus the range rings
        ASSERT(ddoCount == m_NoOfSymbolTracks + 1);
      }

      --ddoCount;

      //the following loop should remove a sufficient number of of ddos
      //it assumes that the range rings will appear at position 0 in the
      //TSLObjectDataLayer

      for ( int i = 0; i < difference; ++i )
      {
        TSLDynamicDataObject* temp = m_objectDataLayer->getDDO( ddoCount - i );
        ASSERT( temp != m_rangeRing );
        m_objectDataLayer->removeDDO(temp);
      }

      if ( newNumber == 0 )
      {
        m_rangeRing->ownShipTrack( NULL );
        CTrackManager::ownShip( NULL ) ;
      }
      CTrackManager::setTrackIDCount(newNumber + 1);

    }

    m_NoOfSymbolTracks = newNumber;
    m_objectDataLayer->notifyChanged( true ) ;
    m_animator->startAnimation();
  }

  void CC2PerformanceDemoDoc::refreshAPP6AAnnotationLevel ()
  {
    AnnotationLevelEnum level = CTrackManager::getAnnotationLevel();
    long ddoCount = m_objectDataLayer->ddoCount();
    for ( int i = 1; i < ddoCount; ++i )
    {
      APP6ATrack* temp = (APP6ATrack*)m_objectDataLayer->getDDO( i );
      temp->setAnnotationLevel(level);
    }
  }
