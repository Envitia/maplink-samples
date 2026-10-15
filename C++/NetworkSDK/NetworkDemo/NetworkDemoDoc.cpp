// NetworkDemoDoc.cpp : implementation of the CNetworkDemoDoc class
//

#include "stdafx.h"
#include "NetworkDemo.h"
#include "NetworkDemoDoc.h"

#include "CostObjects.h"
#include "orientationfilter.h"
#include "TestTimer.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkDemoDoc

const char* CNetworkDemoDoc::m_mapLayerName   = "map";
const char* CNetworkDemoDoc::m_routeLayerName = "route";
const char* CNetworkDemoDoc::m_pinLayerName   = "pin";
const char* CNetworkDemoDoc::m_sqLayerName    = "spatialquery";

// These are the pin IDs (refer to tslsymbols.dat).
static const int START_ROUTE_PIN =    1;
static const int END_ROUTE_PIN	 =    2;
static const int WAYPT_ROUTE_PIN =    3;

static const int ROUTE_SYMBOL_ID = 1001;  // Ball.tmf

static const int STARTPIN_COLOUR =   31;  // Green, for start.
static const int ENDPIN_COLOUR   =  181;  // Red, for end.
static const int WAYPTPIN_COLOUR =    6;  // Blue, for intermediate.

static const int SQ_PIN =   4;
static const int SQ_PIN_COLOUR =    6;  // Blue, for intermediate.

IMPLEMENT_DYNCREATE(CNetworkDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CNetworkDemoDoc, CDocument)
//{{AFX_MSG_MAP(CNetworkDemoDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkDemoDoc construction/destruction

CNetworkDemoDoc::CNetworkDemoDoc()
  : m_routeDataLayer  ( new TSLStandardDataLayer() ),
    m_pinDataLayer    ( new TSLStandardDataLayer() ),
    m_sqDataLayer     ( new TSLStandardDataLayer() ),
    m_network	        ( 0 ),
    m_networkFilename ( "" ),
    m_routingType     ( QuickestRoute ),
    m_endPinAdded     ( false )
{
  // Initialise my variables
  m_mapDataLayer = 0 ;

  m_TMCperMU = 0.0;

  static const char* startPin = "StartPin";
  static const char* endPin   = "EndPin";
  static const char* wayptPin = "WayptPin";

  static const char* sqPin = "SpatialQueryPin";

  m_pinDataLayer->addFeatureRendering( startPin, START_ROUTE_PIN );
  m_pinDataLayer->setFeatureRendering( startPin, 0, TSLRenderingAttributeSymbolColour, STARTPIN_COLOUR);
  m_pinDataLayer->setFeatureRendering( startPin, 0, TSLRenderingAttributeSymbolStyle, ROUTE_SYMBOL_ID);
  m_pinDataLayer->setFeatureRendering( startPin, 0, TSLRenderingAttributeSymbolFixedSize, true);
  m_pinDataLayer->setFeatureRendering( startPin, 0, TSLRenderingAttributeSymbolSizeFactor, 1.0);

  m_pinDataLayer->addFeatureRendering( endPin, END_ROUTE_PIN );
  m_pinDataLayer->setFeatureRendering( endPin, 0, TSLRenderingAttributeSymbolColour, ENDPIN_COLOUR);
  m_pinDataLayer->setFeatureRendering( endPin, 0, TSLRenderingAttributeSymbolStyle, ROUTE_SYMBOL_ID);
  m_pinDataLayer->setFeatureRendering( endPin, 0, TSLRenderingAttributeSymbolFixedSize, true);
  m_pinDataLayer->setFeatureRendering( endPin, 0, TSLRenderingAttributeSymbolSizeFactor, 1.0);

  m_pinDataLayer->addFeatureRendering( wayptPin, WAYPT_ROUTE_PIN );
  m_pinDataLayer->setFeatureRendering( wayptPin, 0, TSLRenderingAttributeSymbolColour, WAYPTPIN_COLOUR);
  m_pinDataLayer->setFeatureRendering( wayptPin, 0, TSLRenderingAttributeSymbolStyle, ROUTE_SYMBOL_ID);
  m_pinDataLayer->setFeatureRendering( wayptPin, 0, TSLRenderingAttributeSymbolFixedSize, true);
  m_pinDataLayer->setFeatureRendering( wayptPin, 0, TSLRenderingAttributeSymbolSizeFactor, 1.0);

  m_sqDataLayer->addFeatureRendering( sqPin, SQ_PIN );
  m_sqDataLayer->setFeatureRendering( sqPin, 0, TSLRenderingAttributeSymbolColour, SQ_PIN_COLOUR);
  m_sqDataLayer->setFeatureRendering( sqPin, 0, TSLRenderingAttributeSymbolStyle, ROUTE_SYMBOL_ID);
  m_sqDataLayer->setFeatureRendering( sqPin, 0, TSLRenderingAttributeSymbolFixedSize, true);
  m_sqDataLayer->setFeatureRendering( sqPin, 0, TSLRenderingAttributeSymbolSizeFactor, 1.0);
}

CNetworkDemoDoc::~CNetworkDemoDoc()
{
  // Destroy any data layer and path list loaded
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  if ( m_network )
  {
    m_network->destroy();
    m_network = 0;
  }

  if ( m_routeDataLayer )
  {
    m_routeDataLayer->destroy();
    m_routeDataLayer = 0;
  }

  if ( m_pinDataLayer )
  {
    m_pinDataLayer->destroy();
    m_pinDataLayer = 0;
  }

  if ( m_sqDataLayer )
  {
    m_sqDataLayer->destroy();
    m_sqDataLayer = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkDemoDoc diagnostics

#ifdef _DEBUG
void CNetworkDemoDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CNetworkDemoDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNetworkDemoDoc commands

BOOL CNetworkDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  // Create a map data layer
  m_mapDataLayer = new TSLMapDataLayer();
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // Load the map
  m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) );

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  // Ask the user which network to load.
  CFileDialog nwkDialog( TRUE, NULL, NULL, NULL, _T("Standard Network Files (*.nwk)|*.nwk||") );
  nwkDialog.m_ofn.lpstrInitialDir = _T(".\\Reference Networks");
  nwkDialog.m_ofn.lpstrTitle      = _T("Open a Network");
  nwkDialog.m_ofn.Flags |= OFN_NOCHANGEDIR;
  if ( nwkDialog.DoModal() == IDOK )
  {
    CString fileName = nwkDialog.GetPathName();

    loadStandardNetwork( fileName );
  }
  
  return TRUE;
}

bool CNetworkDemoDoc::addToSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Add the map to the specified drawing surface,
  surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;
  surface->setDataLayerProps( m_mapLayerName, TSLPropertyVisible, true ) ;
  surface->setDataLayerProps( m_mapLayerName, TSLPropertyDetect,  true ) ;
  surface->setDataLayerProps( m_mapLayerName, TSLPropertyBuffered,true ) ;

  surface->addDataLayer( m_routeDataLayer, m_routeLayerName );
  surface->setDataLayerProps( m_routeLayerName, TSLPropertyVisible, true );
  surface->setDataLayerProps( m_routeLayerName, TSLPropertyDetect,  false );
  surface->setDataLayerProps( m_routeLayerName, TSLPropertySelect,  false );

  surface->addDataLayer( m_pinDataLayer, m_pinLayerName );
  surface->setDataLayerProps( m_pinLayerName, TSLPropertyVisible, true );
  surface->setDataLayerProps( m_pinLayerName, TSLPropertyDetect,  false );
  surface->setDataLayerProps( m_pinLayerName, TSLPropertySelect,  false );

  surface->addDataLayer( m_sqDataLayer, m_sqLayerName );
  surface->setDataLayerProps( m_sqLayerName, TSLPropertyVisible, true );
  surface->setDataLayerProps( m_sqLayerName, TSLPropertyDetect,  false );
  surface->setDataLayerProps( m_sqLayerName, TSLPropertySelect,  false );

  m_TMCperMU = surface->TMCperMU();

  return true ;
}

bool CNetworkDemoDoc::detachFromSurface( TSLDrawingSurface * surface )
{
  if ( !m_mapDataLayer  )
    return false;

  // Remove the map to the specified drawing surface,
  surface->removeDataLayer( m_mapLayerName ) ;

  return true ;
}

void CNetworkDemoDoc::DeleteContents() 
{
  // Clean up by destroying the map
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  if ( m_network )
  {
    m_network->destroy();
    m_network = 0;
  }

  removeRouteAndPinData();

  CDocument::DeleteContents();
}

void CNetworkDemoDoc::removeRouteAndPinData()
{
  if ( m_routeDataLayer )
  {
    m_routeDataLayer->removeData();
    m_routeDataLayer->notifyChanged( true );
  }

  if ( m_pinDataLayer )
  {
    m_pinDataLayer->removeData();
    m_pinDataLayer->notifyChanged( true );
  }

  m_endPinAdded = false;

  m_routeNodeIDs.clear();
}

void CNetworkDemoDoc::clearSpatialQueryData()
{
  if ( m_sqDataLayer )
  {
    m_sqDataLayer->removeData();
    m_sqDataLayer->notifyChanged( true );
  }
}

bool CNetworkDemoDoc::loadStandardNetwork(LPCTSTR filename)
{
  // Clear route display and journey definition.
  removeRouteAndPinData();

  // If an old network exists, kill it.
  if ( m_network )
  {
    m_network->destroy();
    m_network = 0;
  }

  m_network = new TSLNetwork();
  m_networkFilename = TSLUTF8Encoder(filename);

  TSLifstream ifs( m_networkFilename.c_str() );

  ifs >> m_network;

  // Has it loaded properly?
  if ( ifs.bad() )
  {
    m_network->destroy();
    m_network = 0;

    return false;
  }

  return true;
}

bool CNetworkDemoDoc::addRoutePin(long xTMC, long yTMC, double TMCperMU, long pinSize, bool endPin)
{
  // If m_routeNodeIDs is empty, the start pin is being added.
  // Otherwise, either the end pin or intermediate pin are being added,
  // depending on the value of endPin.
  int PIN_ID = m_routeNodeIDs.size() == 0 ? START_ROUTE_PIN : endPin ? END_ROUTE_PIN : WAYPT_ROUTE_PIN;

  // Find the node ID closest to this position within the aperture.
  //
  // Use a 2 km aperture when searching for the node.
  static const long APERTURE = 2000.0*TMCperMU;
  long nodeID = findNodeID( xTMC, yTMC, APERTURE );

  if ( nodeID < 0 )
  {
    // The node was not found.
    return false;
  }
  
  // Create the pin.
  TSLEntitySet* entitySet = m_pinDataLayer->entitySet();

  // We snap to the node position.
  const TSLCoord& pos = m_network->get_node( nodeID ).position();
  entitySet->createSymbol( PIN_ID, pos.x(), pos.y(), pinSize );

  m_pinDataLayer->notifyChanged( true );
  
  // Add the pin at the appropriate position in the journey vector.
  if ( m_routeNodeIDs.empty() || endPin || !m_endPinAdded )
  {
    m_routeNodeIDs.push_back( nodeID );
  }
  else
  {
    m_routeNodeIDs.insert( m_routeNodeIDs.end() - 1, nodeID );
  }
  
  if ( endPin )
  {
    m_endPinAdded = true;
  }

  return true;
}

// pinSize is the 'size' of the pin.
// Returns true if added successfully.
bool CNetworkDemoDoc::addSpatialQuerySearchPointPin(long xTMC, long yTMC, double TMCperMU, long pinSize)
{
  // Get the SQ Data Layer Entity Set
  TSLEntitySet* entitySet = m_sqDataLayer->entitySet();

  // Add the Search Point Pin at the position passed in
  entitySet->createSymbol( SQ_PIN, xTMC, yTMC, pinSize );

  m_sqDataLayer->notifyChanged( true );

  return true;
}

// Displays a line from the search point and the extent of the search range
bool CNetworkDemoDoc::displaySpatialQueryOrientationRange( const TSLEnvelope& extent,
  const TSLCoord& endPt, double TMCperMU )
{
  // Get the Data Layer's entity set
  TSLEntitySet* entitySet = m_sqDataLayer->entitySet();

  // Rectangle from Extent
  TSLRectangle* rect = entitySet->createRectangle( 0, extent.bottomLeft(), 
                                                      extent.topRight() );
  rect->setRendering(TSLRenderingAttributeEdgeColour, 1); // Black.
  rect->setRendering(TSLRenderingAttributeEdgeThickness, 2);
  rect->setRendering(TSLRenderingAttributeEdgeStyle, 1); // Solid.

  // Create line, representing Orientation.
  TSLCoordSet* coSet = new TSLCoordSet();
  coSet->add( extent.centre() );
  coSet->add( endPt );
  TSLPolyline* line = entitySet->createPolyline( 0, coSet, true );
  line->setRendering(TSLRenderingAttributeEdgeColour, 6); // Blue.
  line->setRendering(TSLRenderingAttributeEdgeThickness, 2);
  line->setRendering(TSLRenderingAttributeEdgeStyle, 1); // Solid.

  // Inform the Data Layer that it has changed
  m_sqDataLayer->notifyChanged( true );

  return true;
}

long CNetworkDemoDoc::findNodeID(long xTMC, long yTMC, long aperture) const
{
  long nodeID = -1;

  if ( m_network )
  {
    TSLNetwork::const_node_iterator nit = m_network->find_node( TSLCoord(xTMC, yTMC), aperture );
    
    if ( nit != m_network->end_nodes() )
    {
      // The ID is the distance of the iterator from that of the first node.
      nodeID = nit - m_network->begin_nodes();
    }
  }

  return nodeID;
}

void CNetworkDemoDoc::calculateRoute()
{
  // Check that both start and end have been defined.
  if ( m_routeNodeIDs.size() < 2 )
  {
    return;
  }

  // The cost objects.
  TSLPathCost shortestRoute;
  TSLQuickest quickestRoute;

  // Make the routing object.
  TSLRouteFinder* routingEngine;

  switch ( m_routingType )
  {
  case ShortestRoute:
    routingEngine = TSLRouteFinder::make( m_network, shortestRoute );
    break;

  case QuickestRoute:
  default:
    routingEngine = TSLRouteFinder::make( m_network, quickestRoute );
    break;
  }

  static int routeColours[] = { 211, 60, 35, 233, 120, 170, 20, 4 };
  static int colourCycle = 0;

  int numSubRoutes  = m_routeNodeIDs.size() - 1;

  int fromIDX = 0;
  int toIDX   = 1;

  double routeLength = 0.0;
  double routeTime   = 0.0;

  // Network is assumed to be in metres with a tmc per mu of 1000
  // So Max Route Distance is 500km
  static const long MAX_ROUTE_DISTANCE = 500000.0 * (m_TMCperMU * 0.001);

  for ( int nr = 0; nr < numSubRoutes; ++nr )
  {
    long fromID = m_routeNodeIDs[fromIDX];
    long toID   = m_routeNodeIDs[toIDX];

    TSLNetwork::node_iterator nitF = m_network->begin_nodes() + fromID;
    TSLNetwork::node_iterator nitT = m_network->begin_nodes() + toID;

    // This is the calculation of the route.
    int numLinksInRoute = routingEngine->shortest_path( nitF, nitT, MAX_ROUTE_DISTANCE );

    if ( numLinksInRoute > 0 )
    {
      // Add the sub-route to the route data layer.
      TSLPolyline* line = routingEngine->generatePolyline();

      // First, create the background line.
      TSLEntity* lineBack = m_routeDataLayer->entitySet()->createClone( line );
      lineBack->setRendering(TSLRenderingAttributeEdgeColour, 1); // Black.
      lineBack->setRendering(TSLRenderingAttributeEdgeThickness, 7);
      lineBack->setRendering(TSLRenderingAttributeEdgeStyle, 1); // Solid.

      // Now modify the foreground line.
      line->setRendering(TSLRenderingAttributeEdgeColour, routeColours[colourCycle]);
      line->setRendering(TSLRenderingAttributeEdgeThickness, 3);
      line->setRendering(TSLRenderingAttributeEdgeStyle, 1); // Solid.
      m_routeDataLayer->entitySet()->insert( line );

      // Keep track of total length & time taken.
      for ( int i = 0; i < numLinksInRoute; ++i )
      {
        TSLRouteFinder::Segment seg = (*routingEngine)[i];
      
        const TSLLink* link = seg.m_link;
      
        // link length is in TMCs - convert to map units
        routeLength += ( link->length() / m_TMCperMU );
        routeTime   += ( link->length() / m_TMCperMU ) / getSpeed( link->featureCode() );
      }
    }
    else
    {
      ::MessageBox( 0, _T("No Links for Selected Route"), _T("Route Details"), MB_OK | MB_TASKMODAL );
    }

    // Move on to next pair of nodes.
    fromIDX = toIDX++;
  }

  // Network is assumed to be in metres with a TMC per map Unit value of 1000
  // Route length is in map units (metres) so needs to be divided by 1000 to get it into km
  routeLength *= 0.001;

  // Display a message about the route details.
  int totalMilliSeconds = routeTime*1000;
  int seconds = (int) (totalMilliSeconds / 1000) % 60 ;
  int minutes = (int) ((totalMilliSeconds / (1000*60)) % 60);
  int hours   = (int) ((totalMilliSeconds / (1000*60*60)) % 24);
  int milliSeconds = (totalMilliSeconds % 1000); 
  //int hours   = routeTime/3600.0;
  //int minutes = (int)(routeTime/60.0) - hours*60;
  
  TCHAR info[255];
  _stprintf( info, _T("Journey length:  %.4f km\nTime taken:  %d hrs. %d min. %d sec. %d millisec."), routeLength, hours, minutes, seconds, milliSeconds );
  //_stprintf( info, _T("Journey length:  %.2f km\nTime taken:  %d hrs. %d min."), routeLength, hours, minutes );
  
  ::MessageBox( 0, info, _T("Route details"), MB_OK | MB_TASKMODAL );

  if ( ++colourCycle > 7 )
  {
    colourCycle = 0;
  }
  
  m_routeDataLayer->notifyChanged( true );

  routingEngine->destroy();
}

void CNetworkDemoDoc::clearRouteDisplay()
{
  if ( m_routeDataLayer )
  {
    m_routeDataLayer->removeData();

    m_routeDataLayer->notifyChanged( true );
  }
}

void CNetworkDemoDoc::clearRoutes()
{
  removeRouteAndPinData();
}

void CNetworkDemoDoc::prepareForPrinting()
{
  changeRouteSize( 3.0, 4.0 );
}

void CNetworkDemoDoc::finishedPrinting()
{
  changeRouteSize( 1.0/3.0, 1.0/4.0 );
}

void CNetworkDemoDoc::changeRouteSize(double routeFactor, double pinFactor)
{
  TSLEntitySet* routes = m_routeDataLayer->entitySet();
  
  int NR = routes->size();
  
  // Change the route size.
  for ( int iR = 0; iR < NR; ++iR )
  {
    TSLEntity* line = (*routes)[iR];
    
    double val;
    line->getRendering(TSLRenderingAttributeEdgeThickness, &val);
    
    val *= routeFactor;
    
    line->setRendering(TSLRenderingAttributeEdgeThickness, val);
  }
  
  // Change the pin size.
  TSLEntitySet* pins = m_pinDataLayer->entitySet();
  
  int NP = pins->size();
  
  for ( int iP = 0; iP < NP; ++iP )
  {
    TSLSymbol* pin = (TSLSymbol*)( (*pins)[iP] );
    
    long val = pin->height();
    
    val *= pinFactor;
    
    pin->height( val );
  }
}


int CNetworkDemoDoc::performSpatialQuery( const TSLEnvelope& extent, 
                                          const double& range, 
                                          const double& orientation,
                                          bool useSpatialIndex,
                                          std::string &displayResult)
{
  int result = 0;
  displayResult = "";

  // Test Timer
  TestTimer testTimer;
  double totalTime = 0;

  // Orientation Tolerance +/- in degrees
  double tolerance = 90;

  // Check Spatial Index flag
  if ( useSpatialIndex )
  {
    // Create/load Spatial Index
    if ( !loadSpatialIndex() )
    {
      // Unable to load Spatial Index
      return 1;
    }
  }

  // Orientation Filter object
  OrientationFilter* orientationFilter = new OrientationFilter( extent.centre(), orientation, tolerance );
  if ( NULL == orientationFilter )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( _T("Spatial Query\n Cannot create Orientation Filter\n"), MB_ICONERROR ) ;
    return 1;
  }

  // Start Timer
  testTimer.begin();

  // Perform Test
  TSLNetwork::Query* query = m_network->find_links( 
    extent.centre(), 
    range, 
    orientationFilter,
    -1, useSpatialIndex );

  // Stop Timer 
  testTimer.end();

  // Retrieve time
  double dtFindTime = testTimer.interval();
  totalTime += dtFindTime;

  // Number of Links found
  int numberOfReturnedLinks = query->size_links();

  // Now need to extract information from Query
  // Loop thru set extracting links
  const TSLLink* closestLink = NULL;
  double bestDistance = DBL_MAX;
  double actualDistance;
  TSLCoord bestlinkSegStartPt;
  TSLCoord bestlinkSegEndPt;
  std::multimap<double, TSLPolyline*> lines;
  for ( int linkIndex = 0; linkIndex < numberOfReturnedLinks; ++linkIndex )
  {
    // Start Timer
    testTimer.begin();

    // Retrieve link
    const TSLLink* link = query->link( linkIndex );
    if ( link != NULL )
    {
      // Foreground line
      TSLPolyline* line = m_sqDataLayer->entitySet()->createPolyline( 0, link->getPoints(), true );

      // First, create the background line.
      TSLEntity* lineBack = m_sqDataLayer->entitySet()->createClone( line );
      lineBack->setRendering(TSLRenderingAttributeEdgeColour, 1); // Black.
      lineBack->setRendering(TSLRenderingAttributeEdgeThickness, 2);
      lineBack->setRendering(TSLRenderingAttributeEdgeStyle, 1); // Solid.

      m_sqDataLayer->entitySet()->insert( line );

      TSLCoord linkSegStartPt;
      TSLCoord linkSegEndPt;
      double sqDistance;
      double segAngle;

      // Find the shortest distance to a segment whose orientation is 
      // within the tolerance
      bool linkDataOk = Utility::getShortestDistanceToSegmentWithOrientation( 
        extent.centre(),
        *link, 
        orientation,
        sqDistance, 
        segAngle, 
        tolerance,
        &linkSegStartPt,
        &linkSegEndPt);

      if ( linkDataOk )
      {
        // Ok
        // Stop Timer 
        testTimer.end();

        // Convert distance
        actualDistance =  sqrt(sqDistance);

        lines.insert( make_pair( actualDistance, line ) );

        // Is this Distance the closest ?
        if ( actualDistance < bestDistance )
        {
          // Closest so record values
          closestLink = link;
          bestDistance = actualDistance;
          bestlinkSegStartPt = linkSegStartPt;
          bestlinkSegEndPt = linkSegEndPt;
        }
      }
      else
      {
        // Error

        // Stop Timer 
        testTimer.end();
      }
    }
    else
    {
      // Error

      // Stop Timer 
      testTimer.end();
    }

    // Retrieve time
    double dtLinkTime = testTimer.interval();

    // Add to test running total
    totalTime += dtLinkTime;
  }

  if( lines.size() > 0 )
  {
    // Set up the colours for the links found. The closest link is coloured red (RGB 255,0,0),
    // fading to blue (RGB 0,0,255) as the distance increases
    multimap<double, TSLPolyline*>::iterator linkIt = lines.begin();
    multimap<double, TSLPolyline*>::iterator linkEnd = lines.end();

    double colourStepping = 255.0 / (double)lines.size();
    int count = 0;

    for( ; linkIt != linkEnd; ++linkIt, ++count )
    {
      linkIt->second->setRendering( TSLRenderingAttributeEdgeStyle, 1 );
      linkIt->second->setRendering( TSLRenderingAttributeEdgeColour, TSLComposeRGB( (int)( 255.0 - ( colourStepping * count ) ),
                                                                                    0, (int)( colourStepping * count ) ) );
      linkIt->second->setRendering( TSLRenderingAttributeEdgeThickness, 5 );
      linkIt->second->setRendering( TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPixels );
    }
  }

  // Determine Search Data for display
  double searchPtLat;
  double searchPtLong;
  double searchRangeMapUnits;
  if ( m_mapDataLayer )
  {
    const TSLCoordinateSystem* coordSys = m_mapDataLayer->queryMapCoordinateSystem();
    if ( coordSys )
    {
      // Search position Lat/long
      coordSys->TMCToLatLong( extent.centre().m_x, extent.centre().m_y,
                                   &searchPtLat, &searchPtLong, true );
      searchRangeMapUnits = range / m_TMCperMU;
    }
  }

  // Build string to enable Returned Data to be displayed in a Message Box
  string closestLinkTestData( "Spatial Query" );
  closestLinkTestData.append( "\n" );

  // Using Spatial Index ?
  if ( useSpatialIndex )
  {
    closestLinkTestData.append( "\nSearch utilised Spatial Index" );
  }
  else
  {
    closestLinkTestData.append( "\nSearch without Spatial Index" );
  }

  // Network Data
  closestLinkTestData.append( getNetworkDataString() );

  // Search Criteria
  closestLinkTestData.append( buildSpatialQuerySearchCriteriaString( 
    searchPtLat,
    searchPtLong,
    searchRangeMapUnits,
    orientation,
    tolerance ) );

  // Results
  closestLinkTestData.append( "\n\nSearch Results" );

  if ( numberOfReturnedLinks > 0 )
  {
    // Number of links returned
    // Char string used for number->string conversions
    char numStr[254];
    closestLinkTestData.append( "\nNumber of links returned = " );
    _snprintf( numStr, sizeof( numStr ), "%d", numberOfReturnedLinks );
    closestLinkTestData += numStr;
  }
  else
  {
    // No links returned
    closestLinkTestData.append( "\nNo links returned within range." );
  }

  // Closest Distance
  if ( closestLink )
  {
    // Highlight link
    // Foreground line
    m_sqDataLayer->entitySet()->createPolyline( 0, closestLink->getPoints(), true );

    // Link Segment
    TSLCoordSet* coSet = new TSLCoordSet();
    coSet->add( bestlinkSegStartPt );
    coSet->add( bestlinkSegEndPt );

    // Highlight closest segment in RED
    TSLPolyline* seg = m_sqDataLayer->entitySet()->createPolyline( 0, coSet, true );

    seg->setRendering(TSLRenderingAttributeEdgeColour, 181); // Red
    seg->setRendering(TSLRenderingAttributeEdgeThickness, 3);
    seg->setRendering(TSLRenderingAttributeEdgeStyle, 1); // Solid.

    // Build results string
    closestLinkTestData.append( buildSpatialQuerySearchResultsString( 
      *closestLink,
      bestlinkSegStartPt,
      bestlinkSegEndPt,
      bestDistance,
      dtFindTime,
      totalTime ) );
  }

  // Return the description of the result.
  displayResult = closestLinkTestData;

  // Clean up
  query->destroy();
  if ( orientationFilter )
  {
    delete orientationFilter;
    orientationFilter = NULL;
  }

  return result;
}

// Retrieves Data about the Network and formats it into a single string
// ready for output to a message box.
// Outputs the following information about the network:-
//   Number of Nodes
//   Number of Links
//   Node Extent Height in kilometres
//   Node Extent Width in kilometres
string CNetworkDemoDoc::getNetworkDataString() const
{
  char numStr[254];
  string networkData( "\n\nNetwork Data" );

  // Network Number of Nodes
  int numNodes = m_network->size();
  networkData.append( "\nNumber of Nodes in Network = " );
  _snprintf( numStr, sizeof( numStr ), "%i", numNodes );
  networkData += numStr;

  // Network Number of Links
  int numLinks = m_network->size_links();
  networkData.append( "\nNumber of Links in Network = " );
  _snprintf( numStr, sizeof( numStr ), "%i", numLinks );
  networkData += numStr;

  // Calculate Network Node Extent
  // Iterate thru nodes building the Network extent
  TSLNetwork::const_node_iterator itS( m_network->begin_nodes() );
  TSLNetwork::const_node_iterator itE( m_network->end_nodes() );
  TSLNode const* node = *itS;
  TSLEnvelope nodeExtent( node->position(), node->position() );
  ++itS;
  for ( ; itS != itE; ++itS )
  {
    node = *itS;

    if ( node )
    {
      nodeExtent += node->position();
    }
  }

  // Network Node Extent Height & Width
  unsigned long nwkHeight = nodeExtent.height();
  networkData.append( "\nNetwork (Node Extent) Height (km) = " );
  double nwkHeightkm = ( nwkHeight / m_TMCperMU ) / 1000.0;
  _snprintf( numStr, sizeof( numStr ), "%g", nwkHeightkm );
  networkData += numStr;

  unsigned long nwkWidth = nodeExtent.width();
  networkData.append( "\nNetwork (Node Extent) Width (km) = " );
  double nwkWidthkm = ( nwkWidth / m_TMCperMU ) / 1000.0;
  _snprintf( numStr, sizeof( numStr ), "%g", nwkWidthkm );
  networkData += numStr;

  return networkData;
}

// Function that loads from file or builds the Spatial Index.
// Initally checks the current Spatial Index status as it only needs to 
// load or build it if the status is invalid.
// Function returns if valid.
// If invalid, the function attempts to load ithe Spatial Index from a file.
// It assumes the path to the file and the filename will be the same as that
// used to load the network but with the extension .nsi.
// If any errors occur then the Spatial Index is built from scratch.
// If this fails then a message box is output to the user.
bool CNetworkDemoDoc::loadSpatialIndex( void )
{
  bool result = true;

  // Need to check Spatial Index status
  if ( !m_network->spatialIndexUpToDate() )
  {
    // First check to see if there is a file to load
    // Loading from file is much quicker than building the Spatial Index
    bool errorLoadingSpatialIndexFromFile = false;

    // Get filename
    // If a Spatial Index file exists it should be in the same directory
    // as the network file with the same name but with the extension .nsi
    string spatialIndexInputFilename( m_networkFilename );
    int extensionIndex = spatialIndexInputFilename.rfind( "nwk" );
    if ( extensionIndex != std::string::npos )
    {
      spatialIndexInputFilename.replace( extensionIndex, 3, "nsi" );
      TSLifstream nwkSpatialIndexIn( spatialIndexInputFilename.c_str() );
      // Check stream object        
      if ( !nwkSpatialIndexIn.bad() )
      {
        // Stream the Spatial Index in
        if ( !m_network->loadSpatialIndex( nwkSpatialIndexIn ) )
        {
          // Error, unable to load Spatial Index from file
          errorLoadingSpatialIndexFromFile = true;
        }
      }
      else
      {
        // Error, .nsi file probably doesn't exist
        errorLoadingSpatialIndexFromFile = true;
      }
    }
    else
    {
      // Error Network file doesn't have .nwk extension
      errorLoadingSpatialIndexFromFile = true;
    }

    // Check Spatial Index status
    if ( errorLoadingSpatialIndexFromFile || !m_network->spatialIndexUpToDate() )
    {
      // Invalid so need to build
      if ( !m_network->updateSpatialIndex() )
      {
        // Error - The network spatial index build failed.
        AfxMessageBox( _T("Spatial Query\n Network failed to build Spatial Index\n"), MB_ICONERROR ) ;      
        result = false;
      }
    }
  }

  return result;
}


// Formats the input Search Results data into a single string
// ready for output to a message box.
// Outputs the following information:-
//   Closest Link Segment Start Point Latitude/Longitude (in degrees)
//   Distance to Closest Link Segment (in metres)
//   Orientation of Closest Link Segment (in degrees)
//   Time to find Closest Link Segment (in seconds)
//   Total Test Time (in seconds)
string CNetworkDemoDoc::buildSpatialQuerySearchResultsString( 
          const TSLLink& closestLink,
          const TSLCoord& bestLinkSegStartPt,
          const TSLCoord& bestLinkSegEndPt,
          const double& bestDistance,
          const double& findTime,
          const double& totalTime ) const
{
  string resultString( "" );
  // Char string used for number->string conversions
  char numStr[254];

  // Convert results from Map to Real World units
  double segStartPtLat;
  double segStartPtLong;
  double segEndPtLat;
  double segEndPtLong;
  double segmentAngle = Utility::baseLineAngle( bestLinkSegStartPt, bestLinkSegEndPt );
  if ( m_mapDataLayer )
  {
    const TSLCoordinateSystem* coordSys = m_mapDataLayer->queryMapCoordinateSystem();
    if ( coordSys )
    {
      // Closest Link Segment Start position Lat/long
      coordSys->TMCToLatLong( bestLinkSegStartPt.m_x, bestLinkSegStartPt.m_y,
                                   &segStartPtLat, &segStartPtLong, true );
      // Closest Link Segment End position Lat/long
      coordSys->TMCToLatLong( bestLinkSegEndPt.m_x, bestLinkSegEndPt.m_y,
                                   &segEndPtLat, &segEndPtLong, true );

        // Closest Link Position
      resultString.append( "\nClosest Link Segment Start Position (lat/long) = " );
      _snprintf( numStr, sizeof( numStr ), "%g", segStartPtLat );
      resultString += numStr;
      resultString.append( "\\" );
      _snprintf( numStr, sizeof( numStr ), "%g", segStartPtLong );
      resultString += numStr;
      resultString.append( "\nClosest Link Segment End Position (lat/long) = " );
      _snprintf( numStr, sizeof( numStr ), "%g", segEndPtLat );
      resultString += numStr;
      resultString.append( "\\" );
      _snprintf( numStr, sizeof( numStr ), "%g", segEndPtLong );
      resultString += numStr;

    }
  }
  // Distance to Closest Link 
  resultString.append( "\n\nDistance to Closest Link Segment (m) = " );
  _snprintf( numStr, sizeof( numStr ), "%g", ( bestDistance / m_TMCperMU ) );
  resultString += numStr;

  // Orientation of Closest Link Segment
  resultString.append( "\nOrientation of Closest Link Segment (degs) = " );
  _snprintf( numStr, sizeof( numStr ), "%g", segmentAngle );
  resultString += numStr;
  resultString.append( "\n" );

  // Time to find closest link
  resultString.append( "\nTime to find closest link = " );
  _snprintf( numStr, sizeof( numStr ), "%g", findTime );
  resultString += numStr;

  // Total Test Time
  resultString.append( "\nTotal Test Time = " );
  _snprintf( numStr, sizeof( numStr ), "%g", totalTime );
  resultString += numStr;

  return resultString;
}

// Formats the input Search Criteria data into a single string
// ready for output to a message box.
// Outputs the following information:-
//   Search Point Latitude/Longitude (in degrees)
//   Search Range (in metres)
//   Orientation (in degrees)
//   Tolerance (+/- degrees)
string CNetworkDemoDoc::buildSpatialQuerySearchCriteriaString( 
    const double& searchPtLat,
    const double& searchPtLong,
    const double& searchRangeMapUnits,
    const double& orientation,
    const double& tolerance ) const
{
  string resultString( "" );
  // Char string used for number->string conversions
  char numStr[254];

  // Search Position
  resultString.append( "\n\nSearch Criteria" );
  resultString.append( "\nSearch Position (lat/long) = " );
  _snprintf( numStr, sizeof( numStr ), "%g", searchPtLat );
  resultString += numStr;
  resultString.append( "\\" );
  _snprintf( numStr, sizeof( numStr ), "%g", searchPtLong );
  resultString += numStr;

  // Search Range
  resultString.append( "\nSearch Range (m) = " );
  _snprintf( numStr, sizeof( numStr ), "%g", searchRangeMapUnits );
  resultString += numStr;

  // Orientation
  resultString.append( "\nSearch Orientation (degs) = " );
  _snprintf( numStr, sizeof( numStr ), "%g", orientation );
  resultString += numStr;

  // Tolerance
  resultString.append( "\nSearch Orientation Tolerance (degs) = +/-" );
  _snprintf( numStr, sizeof( numStr ), "%g", tolerance );
  resultString += numStr;

  return resultString;
}
