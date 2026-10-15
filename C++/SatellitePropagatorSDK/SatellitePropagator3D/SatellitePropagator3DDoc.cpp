// SatellitePropagator3DDoc.cpp : implementation of the SatellitePropagator3DDoc class
//

#include "stdafx.h"
#include "SatellitePropagator3D.h"
#include "SatellitePropagator3DDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Example custom renderer for Satellites
class MyRenderer : public TSL3DCustomSatelliteHandler
{
  public:
    MyRenderer ( TSL3DPolyline * poly ) : m_poly( poly ), m_sensor( 0 ) {}

    virtual ~MyRenderer ()
    {
      if ( m_poly )
      {
        m_poly->destroy() ;
        m_poly = 0 ; 
      }

      if ( m_sensor )
      {
        m_sensor->destroy() ;
        m_sensor = 0 ;
      }
    }

    virtual bool render (TSLSatellite* satellite,  TSL3DRenderingInterface* renderingInterface, TSL3DCoord &position, 
      bool visible, TSL3DDepthTest depthTest)
    {
      if ( !m_sensor )
      {
        // Create a sensor - with the initial position position initially - updated on tick
        int colour ;
        satellite->getRendering( TSLRenderingAttributeSymbolColour, &colour ) ;

        m_sensor = TSL3DSensor::create(0, position, position.altitude() * 1.4, -0.3, 0.3, -0.3, 0.3,
          true, 0.0, -0.5*3.14159, 0.0);
        m_sensor->setRendering(TSLRenderingAttributeEdgeColour, colour ) ;
        m_sensor->setRendering(TSLRenderingAttributeEdgeStyle, 1);
        m_sensor->setRendering(TSLRenderingAttributeEdgeThickness, 1);
        m_sensor->setRendering(TSLRenderingAttributeFillStyle, 503);
        m_sensor->setRendering(TSLRenderingAttributeFillColour, colour );

        // store position of sensor.
        m_lastSensorPosition = position;
      }
      else if ( m_lastSensorPosition != position )
      {
        m_sensor->centre( position ) ;
        // store position of sensor - don't want to update if no position change.
        m_lastSensorPosition = position;
      }
    
      // We don't want to draw the line or dome in pick mode - just use the satellite for picking purposes
      if ( renderingInterface->getRenderingMode() != TSL3DRenderingModePick )
      {
        if (depthTest == TSL3DDepthTestStandard)
        {
          // Draw the predicted path from when we started, and the sensor dome.
          //
          // Could use OpenGL directly here by querying from the rendering interface.
          //
          // We allways draw the line because this is the satellite predicted track.
          if ( m_poly )
          {
            renderingInterface->drawEntity( m_poly ) ;
          }
        }
        else if (depthTest == TSL3DDepthTestDelayedWithHorizonCheck ||
          depthTest == TSL3DDepthTestDelayedWithoutHorizonCheck)
        {
          // Draw the sensor if it the satellite is visible and this is a delayed render call.
          // Note:
          //  With Horizon check you only get visible == true.
          //  With no Horizon check you get visible as either true or false.
          if ( m_sensor && visible)
          {
            // Note calling drawEntity will draw the entity with Depth Test turned on.
            renderingInterface->drawEntity( m_sensor ) ;
          }
        }
        return true ;
      }

      return false ;
    }

    virtual bool tick ( TSLSatellite* satellite, TSLTimeServer* s)
    {
      // NOTE, we cannot change any geometry in here since it
      // is being called in a separate thread.  OpenGL will not allow
      // that to happen if compilation lists are applied. 
      // Therefore we must just return true.
      // We could store things in here and use them in the render, but
      // we should be careful to stay thread-safe.
      // 
      return true ;
    }

  private:
    TSL3DPolyline * m_poly ;
    TSL3DSensor   * m_sensor ;
    TSL3DCoord      m_lastSensorPosition ;
} ;

/////////////////////////////////////////////////////////////////////////////
// SatellitePropagator3DDoc

const char * SatellitePropagator3DDoc::m_mapLayerName = "map" ;
const char * SatellitePropagator3DDoc::m_satLayerName = "sat" ;


IMPLEMENT_DYNCREATE(SatellitePropagator3DDoc, CDocument)

BEGIN_MESSAGE_MAP(SatellitePropagator3DDoc, CDocument)
//{{AFX_MSG_MAP(SatellitePropagator3DDoc)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SatellitePropagator3DDoc construction/destruction

SatellitePropagator3DDoc::SatellitePropagator3DDoc()
{
  // Initialise my variables
  m_mapDataLayer = 0 ;
  m_satDataLayer = 0 ;
}

SatellitePropagator3DDoc::~SatellitePropagator3DDoc()
{
}

/////////////////////////////////////////////////////////////////////////////
// SatellitePropagator3DDoc diagnostics

#ifdef _DEBUG
void SatellitePropagator3DDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void SatellitePropagator3DDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// SatellitePropagator3DDoc commands

BOOL SatellitePropagator3DDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  
  // Create a map data layer
  m_mapDataLayer = new TSLMapDataLayer();
  
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // Load the map
  // This will automatically load all associated files such as 
  // the .pth and .pal files.  These needed to be loaded
  // directly by the application in earlier versions of MapLink
  m_mapDataLayer->loadData( TSLUTF8Encoder(lpszPathName) ) ;

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    // The map load failed, so delete the data layer and path list
    AfxMessageBox( (LPCTSTR)TSLUTF8Decoder(msg), MB_ICONERROR ) ;
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }

  return TRUE;
}

bool SatellitePropagator3DDoc::addToSurface( TSLDrawingSurfaceBase * surface, TSLTimeServer * timeServer )
{
  if ( m_mapDataLayer )
  {
    // Add the map to the specified drawing surface,
    surface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;
  }

  // Initialise and add the Satellite Layer
  if ( !m_satDataLayer )
  {
    m_satDataLayer = new TSL3DSatelliteDataLayer ;
  }

  if ( m_satDataLayer->numberOfSatellites() == 0 )
  {
    loadSatellites() ;

    // Attach the time server
    if ( timeServer )
      m_satDataLayer->attachToTimeServer( timeServer ) ;
  }

  surface->addDataLayer( m_satDataLayer, m_satLayerName );

  // Set the detectability of the satellite layer to allo for picking
  surface->setDataLayerProps( m_satLayerName, TSLPropertyDetect,  true ) ;

  return true ;
}

bool SatellitePropagator3DDoc::detachFromSurface( TSLDrawingSurfaceBase * surface )
{
  // Remove the map to the specified drawing surface,
  if ( m_mapDataLayer )
    surface->removeDataLayer( m_mapLayerName ) ;

  if ( m_satDataLayer )
    surface->removeDataLayer( m_satLayerName ) ;

  return true ;
}

void SatellitePropagator3DDoc::DeleteContents() 
{
  // Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
	
  if (m_satDataLayer)
  {
    m_satDataLayer->destroy();
    m_satDataLayer = 0;
  }

  CDocument::DeleteContents();
}

void SatellitePropagator3DDoc::loadSatellites()
{
  // First find the example TLE file - try current directory, then "..\", then MapLink home
  /////////////////////////////////////////////////////////////////////////////////////////
  CString filename = _T("exampleTLE.txt");

  FILE * fp = TSLFileHelper::fopen( TSLUTF8Encoder(filename), "rb" ) ;
  if ( !fp )
  {
    CString elsewhere = _T("..\\") + filename ;
    fp = TSLFileHelper::fopen( TSLUTF8Encoder(elsewhere), "rb" );
    if ( !fp )
    {
      elsewhere.SetString( (LPCTSTR)TSLUTF8Decoder(TSLUtilityFunctions::getMapLinkHome()) );
      elsewhere += _T("\\samples\\nt\\SatellitePropagator3D\\");
      elsewhere += filename ;
      fp = TSLFileHelper::fopen( TSLUTF8Encoder(elsewhere ), "rb" );
      if ( !fp )
      {
        // Give up, can't find the TLE file.
        AfxMessageBox( _T("Cannot find the example TLE file.  No satellites will be displayed"), MB_ICONERROR ) ;
        return ;
      }
    }
    filename = elsewhere ;
  }
  TSLFileHelper::fclose( fp ) ;

  // Create a satellite manager and load the file
  ///////////////////////////////////////////////
  TSLSatelliteManager *satMan = new TSLSatelliteManager();
  TSLUTF8Encoder tleFile( filename );
  const char *tleFilePtr = tleFile;

  if ( !satMan->loadTLEFiles( &tleFilePtr, 1) )
  {
    CString err ;
    err.Format( _T("Error loading TLE file '%s'\nNo satellites will be displayed"), tleFile ) ;
    AfxMessageBox( err, MB_ICONERROR ) ;
    return ;
  }

  // Define some colours that we may use for the rendering
  int red    = TSLDrawingSurface::getIDOfNearestColour( 255, 0,   0 ) ;
  int yellow = TSLDrawingSurface::getIDOfNearestColour( 255, 255, 0 ) ;
  int cyan   = TSLDrawingSurface::getIDOfNearestColour(   0, 255, 255 ) ;
  int white  = TSLDrawingSurface::getIDOfNearestColour( 255, 255, 255 ) ;

  // Define some sizes, in points (1/72nd of an inch)
  double textSize = 10.0 ;
  double satSize  = 15.0 ;
  int    satStyle = 2013 ; // Triangle with dot

  // Define the labelling we want on the satellites
  /////////////////////////////////////////////////

  TSLEntitySet * labelling = TSLEntitySet::create( ) ;
  TSLText *text = labelling->createText(0, 0, 0, "__name" ) ; // Dynamically expanded to satellite name
  text->setRendering(TSLRenderingAttributeTextHorizontalAlignment, TSLHorizontalAlignmentCentre ) ;
  text->setRendering(TSLRenderingAttributeTextVerticalAlignment, TSLVerticalAlignmentMiddle ) ;
  text->setRendering(TSLRenderingAttributeTextSizeFactorUnits, TSLDimensionUnitsPoints ) ;
  text->setRendering(TSLRenderingAttributeTextSizeFactor, textSize ) ;
  text->setRendering(TSLRenderingAttributeTextOffsetUnits, TSLDimensionUnitsPoints) ;
  text->setRendering(TSLRenderingAttributeTextOffsetY, -satSize ) ;
  text->setRendering(TSLRenderingAttributeTextRotatable, false ) ;
  text->setRendering(TSLRenderingAttributeTextColour, white ) ;
  text->setRendering(TSLRenderingAttributeTextFont, 2 ) ;

  // Instantiate the Satellites, applying appropriate rendering
  /////////////////////////////////////////////////////////////
  int colours[ ] = { red, yellow, cyan } ;

  for ( int i = 0 ; i < satMan->numberOfAvailableSatellites() ; ++i )
  {
    TSLSatellite *sat = satMan->create( satMan->satelliteID( i ) ) ;

    sat->setRendering(TSLRenderingAttributeSymbolColour, colours[ i % 3 ] );
    sat->setRendering(TSLRenderingAttributeSymbolStyle, satStyle);
    sat->setRendering(TSLRenderingAttributeSymbolSizeFactorUnits, TSLDimensionUnitsPoints);
    sat->setRendering(TSLRenderingAttributeSymbolSizeFactor, satSize );
    // Set the depth test to delayed because the sensor will be drawn with an alpha based colour.
    //
    // The line will be drawn on the TSL3DDepthTestStandard call.
    //
    // The sensor will be drawn on the TSL3DDepthTestDelayedWithHorizonCheck call.
    //
    // TSL3DDepthTestStandardAndDelayedWithHorizonCheck, or TSL3DDepthTestStandardAndDelayedWithoutHorizonCheck
    sat->setRendering(TSLRenderingAttributeSymbolDepthTest, TSL3DDepthTestStandardAndDelayedWithHorizonCheck) ; 

    // Add the labelling, which will be copied
    sat->addText( labelling ) ;

    // Now the path - 1 hour, point every 2 minutes, path prediction, actual path
    TSL3DPolyline *poly = TSL3DSatelliteHelper::path(sat, TSLTimeServer::defaultTimeHelper().getCurrentTime(), 
      400, 1.0, true,0, true );

    // extract the polylines
    poly->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;
    poly->setRendering( TSLRenderingAttributeEdgeColour, colours[ i % 3 ]  ) ;
    poly->setRendering( TSLRenderingAttributeEdgeThickness, 1 ) ;
    poly->setRendering( TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPixels ) ;

    MyRenderer * renderer = new MyRenderer( poly ) ;   // Sensor created when we know the initial position
    sat->addCustomRendering( renderer, false, true ) ; // Draw before satellite and sat should assume ownership
    m_satDataLayer->addSatellite( sat );
  }

  // The labelling was copied when added, so we should destroy the template
  labelling->destroy() ;

  m_satDataLayer->notifyChanged();

  satMan->destroy() ;
}

