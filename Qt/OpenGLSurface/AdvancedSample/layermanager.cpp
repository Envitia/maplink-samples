/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/
#include <QMessageBox>
#include <QOpenGLFunctions>

#ifdef _MSC_VER
# include <Windows.h>
# include <ctime>
#endif

#include "layers/tracklayer.h"
#include "layermanager.h"
#include "maplinkwidget.h"

#include "layers/TrackCustomDataLayer.h"
#include "layers/decluttermodel.h"

#include "tslkmldatalayer.h"

#ifdef HAVE_DIRECT_IMPORT_SDK
#include "MapLinkDirectImport.h"
#include "ui/directimportwizard/scalebandstable.h"
#endif

#include "tgm_api.h"

#include <iostream>
#include <cctype>
#include <algorithm>

#include "MapLinkDrawing.h"

#include "layers/frameratelayer.h"
#include "AddWaypointInteractionMode.h"


#include <GL/gl.h>
#include "MapLinkOpenGLSurface.h"

#undef None

// Constructor
LayerManager::LayerManager()
  : m_surface( NULL )
  , m_mapLayer( new TSLStaticMapDataLayer() )
  , m_mapLayer1( new TSLStaticMapDataLayer() )
  , m_mapLayer2( new TSLStaticMapDataLayer() )

  , m_mapLayerLoader( new TSLFileLoaderMT() )
  , m_mapLayerLoader1( new TSLFileLoaderMT() )
  , m_mapLayerLoader2( new TSLFileLoaderMT() )

  , m_loadedBackground( false )
  , m_loadedBackground2( false )

  , m_rasterLayer( new TSLRasterDataLayer() )
  , m_entityLayer( new TSLStandardDataLayer() )

  , m_trackLayer( new TSLCustomDataLayer() )
  , m_trackClient( new TrackCustomDataLayer() )

  , m_currentCoordinateSystem( NULL )

  , m_fpsLayer( new TSLCustomDataLayer() )
  , m_fpsClient( new FPSLayer() )

  , m_currentProjection(None)
  , m_lockProjectionCentre( true )
  , m_timeMultiplier( 1000.0 )
  , m_greatCircleDistance( 9704000.0 )
  , m_framerateCL(NULL)
  , m_tracksLayer( NULL )
  , m_trackCL( NULL )
  , m_tracksLayerName( "TracksLayer" )
  , m_lastFrameTime(-1)
  // We would normally use something like this with a unique_ptr
  //   , m_sharedCache(new TSLOpenGLSingleThreadCache(), [](TSLOpenGLSingleThreadCache* p) { if (p) p->destroy(); })
  , m_sharedCache(new TSLOpenGLSingleThreadCache())
#ifdef HAVE_DIRECT_IMPORT_SDK
  , m_directImportName( "" )
  , m_directImportAnalysisComplete( false )
#endif

{
  m_mapLayer->addLoader( m_mapLayerLoader, NULL, NULL, NULL, NULL );
  m_mapLayer->drawCacheSize( 512 * 1024 );

  m_mapLayer1->addLoader( m_mapLayerLoader1, NULL, NULL, NULL, NULL );
  m_mapLayer1->drawCacheSize( 512 * 1024 );

  m_trackLayer->setClientCustomDataLayer( m_trackClient, false );
  m_fpsLayer->setClientCustomDataLayer( m_fpsClient, false );

#ifdef _MSC_VER
  m_lastFrameTime = clock();
#endif
}

// Destructor
LayerManager::~LayerManager()
{
#ifdef HAVE_DIRECT_IMPORT_SDK
  SurfaceMap::iterator it( m_dataLayer.begin() );
  SurfaceMap::iterator end( m_dataLayer.end() );
  while( it != end )
  {
    if( it->second.second == TSLDataLayerTypeDirectImportDataLayer )
    {
      reinterpret_cast<TSLDirectImportDataLayer*>( it->second.first )->setCallbacks( NULL );
    }
    it->second.first->destroy();
    ++it;
  }
# endif

  if( m_mapLayer )
    m_mapLayer->destroy();
  if( m_mapLayer1 )
    m_mapLayer1->destroy();
  if( m_mapLayerLoader )
    m_mapLayerLoader->destroy();
  if( m_mapLayerLoader1 )
    m_mapLayerLoader1->destroy();
  if( m_mapLayerLoader2 )
    m_mapLayerLoader2->destroy();

  if( m_entityLayer )
    m_entityLayer->destroy();

  if( m_trackLayer )
  {
    m_trackLayer->destroy();
  }
  delete m_trackClient;
  if( m_fpsLayer )
  {
    m_fpsLayer->destroy();
  }

  delete m_fpsClient;
  if( m_rasterLayer )
  {
    m_rasterLayer->destroy();
  }
  if( m_currentCoordinateSystem )
  {
    m_currentCoordinateSystem->destroy();
  }
}

#ifdef HAVE_DIRECT_IMPORT_SDK

void LayerManager::onAnalysisStarted( const TSLDirectImportDataSet* /*dataSet*/ ) {}

void LayerManager::onAnalysisCancelled( const TSLDirectImportDataSet* /*dataSet*/ )
{
  // If analysis failed there will be no features, and the data will fail to load
  m_featureClassConfigMutex.lock();
  m_directImportAnalysisComplete = true;
  m_featureClassConfigWaitCondition.wakeAll();
  m_featureClassConfigMutex.unlock();
}

void LayerManager::onAnalysisFailed( const TSLDirectImportDataSet* /*dataSet*/ )
{
  // If analysis failed there will be no features, and the data will fail to load
  m_featureClassConfigMutex.lock();
  m_directImportAnalysisComplete = true;
  m_featureClassConfigWaitCondition.wakeAll();
  m_featureClassConfigMutex.unlock();
}

void LayerManager::onAnalysisComplete( const TSLDirectImportDataSet* /*dataSet*/, const TSLFeatureList* featureList )
{
  m_featureClassConfigMutex.lock();
  m_directImportAnalysisComplete = true;
  // Add any features from the data to the feature configuration
  if( featureList )
  {
    m_featureClassConfig.featureList().append( *featureList );
  }
  m_featureClassConfigWaitCondition.wakeAll();
  m_featureClassConfigMutex.unlock();
}

void LayerManager::setupFeatureConfig( const std::string& filename, TSLDirectImportDataLayer* layer )
{
  m_featureClassConfig = TSLFeatureClassConfig();

  // The unprocessed feature list within the feature config
  // known features and rendering are added to this object.
  //
  // This list will be modified if any classification/masking settings
  // are specified on the feature config.
  TSLFeatureList& featureList( m_featureClassConfig.featureList() );

  // A basic set of rendering attributes for any MapLink geometry
  TSLRenderingAttributes basicAttribs;
  // Symbols
  basicAttribs.m_symbolStyle = 4;
  basicAttribs.m_symbolColour = TSLComposeRGB( 0x44, 0x44, 0x44 );
  basicAttribs.m_symbolSizeFactor = 10.0;
  basicAttribs.m_symbolSizeFactorUnits = TSLDimensionUnitsPixels;
  // lines
  basicAttribs.m_edgeStyle = 1;
  basicAttribs.m_edgeColour = TSLComposeRGB( 0x00, 0x00, 0x00 );
  basicAttribs.m_edgeThickness = 1.0;
  basicAttribs.m_edgeThicknessUnits = TSLDimensionUnitsPixels;
  // Polygons
  basicAttribs.m_exteriorEdgeStyle = 1;
  basicAttribs.m_exteriorEdgeColour = TSLComposeRGB( 0x00, 0x00, 0x00 );
  basicAttribs.m_exteriorEdgeThickness = 1.0;
  basicAttribs.m_exteriorEdgeThicknessUnits = TSLDimensionUnitsPixels;
  basicAttribs.m_fillStyle = 1;
  basicAttribs.m_fillColour = TSLComposeRGB( 0xCC, 0xCC, 0xCC );
  // Text
  basicAttribs.m_textFont = 1;
  basicAttribs.m_textColour = TSLComposeRGB( 0x00, 0x00, 0x00 );
  basicAttribs.m_textSizeFactor = 15.0;
  basicAttribs.m_textSizeFactorUnits = TSLDimensionUnitsPixels;

  // The features present in loaded data may be determined through the use of
  // TSLDirectImportDataLayer::analyseData.
  // 
  // Features may also be added explicitly in situations where the features are known
  // or when the data has a defined 'product'

  // Check the file extension
  size_t dotPos( filename.find_last_of( '.' ) );
  if( dotPos == std::string::npos )
  {
    return;
  }
  string ext( filename.substr( dotPos, filename.size() - dotPos ) );
  std::transform( ext.begin(), ext.end(), ext.begin(), toupper );

  if( ext == ".SHP" )
  {
    // Set the basic rendering for any shapefile
    // Each shapefile contains 1 feature
    // The feature's name is the same as the filename

    size_t pos( filename.find_last_of( "\\/" ) );
    if( pos == std::string::npos )
    {
      return;
    }

    string featureName( filename.substr( pos + 1, dotPos - pos - 1 ) );

    // Create the Feature.
    //
    // The default styling is specified by basicAttribs.
    TSLFeature *feature = new TSLFeature( featureName.c_str(), basicAttribs );

    // Specify which attribute to use to extract and display Text.
    //
    // The styling is specified by basicAttribs.
    //
    // The attribute we are using is called 'NAME'. This may not be contained
    // in a shapefile. You should perform an analysis phase to find out what attributes
    // are available.
    //
    // If you know that the Shapefile you are using is from a product family
    // then you can specify the features and classification up-front without
    // doing the analysis phase.
    //
    // An analysis of the data can be expensive to perform, however it is the
    // most flexible approach if you don't know if your data is from a particular
    // product.
    //
    // Note: Shapefile is a carrier format, it does not specify what is contained
    // in the file other then the file format.
    //
    // Some data providers define what we call product's on top of the shapefile
    // format, that define the filename, directory and content in each file.
    //
    feature->setTextLabelAttribute( "NAME" );

    // Add the feature 
    featureList.addFeature( feature );
  }
  else if( ext == ".OSM" || ext == ".PBF" )
  {

    featureList.addFeature( new TSLFeature("lines", basicAttribs) );
    featureList.addFeature( new TSLFeature("points", basicAttribs) );
    featureList.addFeature( new TSLFeature("multipolygons", basicAttribs) );
    featureList.addFeature( new TSLFeature("multilinestrings", basicAttribs) );
    featureList.addFeature( new TSLFeature("other_relations", basicAttribs) );

    /*
    // TBD - more complex example
    // see directimportstylingtests.cpp for a start point

    // Range based subclassing:
    // - Level 1: Motorways are red, A roads are green
    // - Level 2: A24 is Pink
    TSLRenderingAttributes motorwayAttribs;
    motorwayAttribs.m_edgeColour = TSLComposeRGB( 0xff, 0x00, 0x00 );
    motorwayAttribs.m_edgeThickness = 2;
    TSLRenderingAttributes aroadAttribs;
    aroadAttribs.m_edgeColour = TSLComposeRGB( 0x00, 0xff, 0x00 );
    aroadAttribs.m_edgeThickness = 2;
    TSLRenderingAttributes a24Attribs;
    a24Attribs.m_edgeColour = TSLComposeRGB( 0xff, 0x00, 0xff );
    a24Attribs.m_edgeThickness = 5;

    TSLFeatureClassifierGraduated* highwayRanges1( TSLFeatureClassifierGraduated::create( "highway" ) );
    highwayRanges1->addOperation( TSLAttributeValueOperationEqualTo( "motorway", true, motorwayAttribs, "motorway" ) );
    highwayRanges1->addOperation( TSLAttributeValueOperationEqualTo( "motorway_link", true, motorwayAttribs, "motorway" ) );
    highwayRanges1->addOperation( TSLAttributeValueOperationEqualTo( "primary", true, motorwayAttribs, "motorway" ) );
    highwayRanges1->addOperation( TSLAttributeValueOperationEqualTo( "primary_link", true, motorwayAttribs, "motorway" ) );

    highwayRanges1->addOperation( TSLAttributeValueOperationEqualTo( "trunk", true, aroadAttribs, "A Road" ) );
    highwayRanges1->addOperation( TSLAttributeValueOperationEqualTo( "trunk_link", true, aroadAttribs, "A Road" ) );

    // Note: Note particularly useful as it will create lines.motorway.A24 and lines.A Road.A24
    // No application would use range-based classification for a road name, but this is just a test.
    TSLFeatureClassifierGraduated* highwayRanges2( TSLFeatureClassifierGraduated::create( "ref" ) );
    highwayRanges2->addOperation( TSLAttributeValueOperationEqualTo( "A24", true, a24Attribs, "A24 - Bright Pink" ) );
    highwayRanges2->addOperation( TSLAttributeValueOperationEqualTo( "M23", true, a24Attribs, "M23 - Bright Pink" ) );

    featureConfig.addClassifier( "lines", highwayRanges1 );
    featureConfig.addClassifier( "lines", highwayRanges2 );
  */
  }
  else
  {
    // Ask the direct import layer to analyse the data, and set basic rendering attributes for all features
    //
    // Note that this can be a very expensive process and should be performed as an offline process wherever possible.

    // Schedule the data analysis and wait
    m_featureClassConfigMutex.lock();

    // Feature analysis is an asynchronous process, setup the callbacks and wait for results
    layer->setAnalysisCallbacks( this );

    // Analysis is performed on a per-dataset basis
    // Most data paths will only create a single data set
    TSLvector<TSLDirectImportDataSet*>* dataSetsToAnalyse( layer->createDataSets( filename.c_str() ) );
    if( dataSetsToAnalyse )
    {
      for( unsigned int i(0); i < dataSetsToAnalyse->size(); ++i )
      {
        // Schedule the analysis
        // Any features read from the data will be added to m_featureClassConfig
        TSLDirectImportDataSet* dataSet( dataSetsToAnalyse->operator[](i) );
        if( dataSet )
        {
          m_directImportAnalysisComplete = false;
          if( layer->analyseData( dataSet ) )
          {
            while( m_directImportAnalysisComplete == false )
            {
              m_featureClassConfigWaitCondition.wait( &m_featureClassConfigMutex );
            }
          }
        }
      }
      // The data sets returned are owned by the application.
      // These may be passed to loadData layer, however in most situations
      // data analysis should be performed as a separate offline process.
      layer->destroyDataSets( dataSetsToAnalyse );
    }

    // Clear the analysis callback
    // This is not strictly required until the layer is destroyed
    layer->setAnalysisCallbacks( NULL );

    m_featureClassConfigMutex.unlock();

    // Now set rendering on any features in m_featureClassConfig
    for( unsigned int i(0); i < featureList.size(); ++i )
    {
      TSLFeature* feature( featureList[i] );
      if( feature )
      {
        feature->rendering() = basicAttribs;
      }
    } 
  }
}
#endif

// Configure a data layer
void LayerManager::configureDataLayer( TSLOpenGLSurface* surface, const string& name )
{
  if (!surface)
  {
    return;
  }

  SurfaceMap::const_iterator it( m_dataLayer.find( name ) );
  if( it == m_dataLayer.end() )
  {
    return;
  }

  TSLDataLayerTypeEnum fileType = it->second.second;
  surface->addDataLayer( it->second.first, name.c_str() );

  if( fileType == TSLDataLayerTypeKMLDataLayer ||
    fileType == TSLDataLayerTypeStandardDataLayer )
  {
    surface->setDataLayerProps( name.c_str(), TSLPropertyVisible, true );
    surface->setDataLayerProps( name.c_str(), TSLPropertyDetect, true );
    surface->setDataLayerProps( name.c_str(), TSLPropertySelect, true );
#ifndef ENABLE_PROJECTION
    surface->setDataLayerProps( name.c_str(), TSLPropertyBuffered, true );
#endif
  }
  else if( fileType == TSLDataLayerTypeDirectImportDataLayer )
  {
    surface->setDataLayerProps( name.c_str(), TSLPropertyProgressiveDisplay, true );
  }

#ifdef ENABLE_PROJECTION
  // Reatime reprojection has a number of limitations that limit how
  // the data is drawn and the impact on performance.
  //
  // The Geoemtry Rendering has to be Feature based. Geometry based rendering is ignored.
  //
  // If the Geoemtry is modified then you have to call notifyChanged() on the 
  // data-layer. This instructs MapLink to re-process the whole layer.
  //  
  // Rendering attributes can change but can result in re-batching.
  //
  // Changing the FeatureID on a Geometry object (entity) will require notifyChanged()
  // to be called.
  //
  // Decluttering on/off works normally. Range decluttering only evaluated on first draw.
  //
  // Dynamic Renderers are excluded because this modifies the Tile contents. 
  //
  surface->setDataLayerProps( name.c_str(), TSLPropertyRealtimeReprojection, true );
#endif

  // Make sure that the FPS layer is on top, followed by the track layer and then
  // the entity layer.
  surface->bringToFront( name.c_str() );

  incrementLayerCount( fileType );

  const char* val = "fps";
  const char* val2 = "EntityLayer";

  if( surface->getDataLayerInfo( 0, NULL, &val2 ) )
  {
    surface->bringToFront( "EntityLayer" );
  }

  if( surface->getDataLayerInfo( 0, NULL, &val ) )
  {
    surface->bringToFront( "track" );
    surface->bringToFront( "fps" );
  }
}

// Configure the entity layer
void LayerManager::configureEntityLayer( TSLOpenGLSurface *surface )
{
  if (!surface || !m_entityLayer)
  {
    return;
  }

  surface->addDataLayer( m_entityLayer, "EntityLayer" );

#ifdef ENABLE_PROJECTION
  // Reatime reprojection has a number of limitations that limit how
  // the data is drawn and the impact on performance.
  //
  // The Geoemtry Rendering has to be Feature based. Geometry based rendering is ignored.
  //
  // If the Geoemtry is modified then you have to call notifyChanged() on the 
  // data-layer. This instructs MapLink to re-process the whole layer.
  //  
  // Rendering attributes can change but can result in re-batching.
  //
  // Changing the FeatureID on a Geometry object (entity) will require notifyChanged()
  // to be called.
  //
  // Decluttering on/off works normally. Range decluttering only evaluated on first draw.
  //
  // Dynamic Renderers are excluded because this modifies the Tile contents. 
  //
  surface->setDataLayerProps( "EntityLayer", TSLPropertyRealtimeReprojection, true );
#endif

  setupRasterSymbol();

  ///////////////////////////////////////////////////////////////
  // Define All the Features we can create.
  ///////////////////////////////////////////////////////////////

  // Define the colours as RGB.
  // Colour index is simpler to use if you want to change the colours dynamically.
  TSLStyleID yellow = TSLComposeRGB( 255, 255, 0 );
  TSLStyleID black = TSLComposeRGB( 0, 0, 0 );
  TSLStyleID purple = TSLComposeRGB( 255, 0, 255 );
  TSLStyleID green = TSLDrawingSurface::getIDOfNearestColour( 0, 255, 0 );

  // Add a Feature - so we can setup feature rendering.
  // The FeatureCode is a positive integer (not zero).
  m_entityLayer->addFeatureRendering( "Polygon", POLYGON_FC );
  m_entityLayer->addFeatureRendering( "VSymbol", VSYMBOL_FC );
  m_entityLayer->addFeatureRendering( "RSymbol", RSYMBOL_FC );
  m_entityLayer->addFeatureRendering( "Polyline", POLYLINE_FC );
  m_entityLayer->addFeatureRendering( "Text", TEXT_FC );
  m_entityLayer->addFeatureRendering( "Arc", ARC_FC );
  m_entityLayer->addFeatureRendering( "Ellipse", ELLIPSE_FC );

  // Note:
  //  1. TSLDimensionMapUnits should be avoided for realtime reprojection
  //  2. The FeatureName does not need to be passed. Using the FeatureCode is quicker.
  //  3. Make sure you use the correct rendering attribute name for the geometry type you are
  //     drawing.
  //  4. Realtime reprojection only accepts Feature Rendering.
  //
  m_entityLayer->setFeatureRendering( NULL, POLYGON_FC, TSLRenderingAttributeFillStyle, 1 );
  m_entityLayer->setFeatureRendering( NULL, POLYGON_FC, TSLRenderingAttributeFillColour, yellow );
  m_entityLayer->setFeatureRendering( NULL, POLYGON_FC, TSLRenderingAttributeExteriorEdgeStyle, 1 );
  m_entityLayer->setFeatureRendering( NULL, POLYGON_FC, TSLRenderingAttributeExteriorEdgeColour, purple );
  m_entityLayer->setFeatureRendering( NULL, POLYGON_FC, TSLRenderingAttributeExteriorEdgeThickness, 1 );
  m_entityLayer->setFeatureRendering( NULL, POLYGON_FC, TSLRenderingAttributeExteriorEdgeThicknessUnits, TSLDimensionUnitsPixels );

  m_entityLayer->setFeatureRendering( NULL, TEXT_FC, TSLRenderingAttributeTextFont, 1 );
  m_entityLayer->setFeatureRendering( NULL, TEXT_FC, TSLRenderingAttributeTextColour, black );
  m_entityLayer->setFeatureRendering( NULL, TEXT_FC, TSLRenderingAttributeTextSizeFactor, 25.0 );
  m_entityLayer->setFeatureRendering( NULL, TEXT_FC, TSLRenderingAttributeTextSizeFactorUnits, TSLDimensionUnitsPixels );

  m_entityLayer->setFeatureRendering( NULL, VSYMBOL_FC, TSLRenderingAttributeSymbolStyle, 14 );
  m_entityLayer->setFeatureRendering( NULL, VSYMBOL_FC, TSLRenderingAttributeSymbolColour, green );
  m_entityLayer->setFeatureRendering( NULL, VSYMBOL_FC, TSLRenderingAttributeSymbolSizeFactor, 100.0 );
  m_entityLayer->setFeatureRendering( NULL, VSYMBOL_FC, TSLRenderingAttributeSymbolSizeFactorUnits, TSLDimensionUnitsPoints );

  m_entityLayer->setFeatureRendering( NULL, ARC_FC, TSLRenderingAttributeEdgeStyle, 3 );
  m_entityLayer->setFeatureRendering( NULL, ARC_FC, TSLRenderingAttributeEdgeColour, yellow );
  m_entityLayer->setFeatureRendering( NULL, ARC_FC, TSLRenderingAttributeEdgeThickness, 10.0 );
  m_entityLayer->setFeatureRendering( NULL, ARC_FC, TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPoints );

  m_entityLayer->setFeatureRendering( NULL, ELLIPSE_FC, TSLRenderingAttributeFillStyle, 1 );
  m_entityLayer->setFeatureRendering( NULL, ELLIPSE_FC, TSLRenderingAttributeFillColour, green );
  m_entityLayer->setFeatureRendering( NULL, ELLIPSE_FC, TSLRenderingAttributeExteriorEdgeStyle, 48 );
  m_entityLayer->setFeatureRendering( NULL, ELLIPSE_FC, TSLRenderingAttributeExteriorEdgeColour, purple );
  m_entityLayer->setFeatureRendering( NULL, ELLIPSE_FC, TSLRenderingAttributeExteriorEdgeThickness, 10.0 );
  m_entityLayer->setFeatureRendering( NULL, ELLIPSE_FC, TSLRenderingAttributeExteriorEdgeThicknessUnits, TSLDimensionUnitsPoints );

  m_entityLayer->setFeatureRendering( NULL, POLYLINE_FC, TSLRenderingAttributeEdgeStyle, 2 );
  m_entityLayer->setFeatureRendering( NULL, POLYLINE_FC, TSLRenderingAttributeEdgeColour, yellow );
  m_entityLayer->setFeatureRendering( NULL, POLYLINE_FC, TSLRenderingAttributeEdgeThickness, 5.0 );
  m_entityLayer->setFeatureRendering( NULL, POLYLINE_FC, TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsPoints );
}

// Configure the tracks layer
void LayerManager::configureTracksLayer( TSLOpenGLSurface* surface )
{
  // Puts the tracks layer back onto the drawing surface if it was removed.
  if(m_trackCL)
  {
      surface->addDataLayer( m_trackCL, m_tracksLayerName.c_str() );
      return;
  }

  // The tracks custom data layer is only created once the drawing surface is valid as
  // it uses QOpenGLFunctions which requires an active OpenGL context.
  m_tracksLayer = new TrackLayer();
  m_trackCL = new TSLCustomDataLayer();
  m_trackCL->setClientCustomDataLayer( m_tracksLayer, false );

  surface->addDataLayer( m_trackCL, m_tracksLayerName.c_str() );
  if( !m_tracksLayer->initialise( surface ) )
  {
    // Something went wrong when creating the data layer, tracks will not be visible
    delete m_tracksLayer;
    m_trackCL->destroy();
    m_trackLayer = NULL;
    m_trackCL = NULL;
  }
  else
  {
    m_declutterModel.addLayerFeatures( QString::fromUtf8( m_tracksLayerName.c_str() ), m_trackCL );
  }

  //surface->addDataLayer( m_framerateCL, "framerate" );
  // Make the framerate layer hidden by default, this only needs to be displayed
  // while the track simulation is running as otherwise the application does not
  // continuously update.
  //surface->setDataLayerProps( "framerate", TSLPropertyVisible, 0 );

  // Tell the declutter model about the drawing surface so that it can apply decluttering through it
  m_declutterModel.setDrawingSurface( surface );

}

// Configure the drawing surface
void LayerManager::configureDrawingSurface( TSLOpenGLSurface *surface, AddWaypointInteractionMode *waypointMode )
{
  m_surface = surface;
  surface->addDataLayer( m_mapLayer, "map" );

  m_declutterModel.addLayerFeatures( QString::fromUtf8( "map" ), m_mapLayer );

  // Set the coordinate providing layer of the drawing-surface.
  // The default behaviour is normally that the last layer added that is
  // a coordinate providing layer wins. Calling the method below
  // allows us to be explicit in our choice.
  surface->setCoordinateProvidingLayer( m_mapLayer, "map" );

  surface->addDataLayer( m_mapLayer1, "map1" );
#ifdef ENABLE_PROJECTION
  // Reatime reprojection has a number of limitations that limit how
  // the data is drawn and the impact on performance.
  //
  // The Geoemtry Rendering has to be Feature based. Geometry based rendering is ignored.
  //
  // If the Geoemtry is modified then you have to call notifyChanged() on the 
  // data-layer. This instructs MapLink to re-process the whole layer. Modifying Geometry
  // in a TSLMapDataLayer/TSLStaticMapDataLayer is not going to be persistent in any-case.
  //  
  // Rendering attributes can change but can result in re-batching.
  //
  // Decluttering on/off works normally. Range decluttering is only evaluated on first draw.
  //
  // Dynamic Renderers are excluded because this modifies the Tile contents. 
  //
  surface->setDataLayerProps( "map", TSLPropertyRealtimeReprojection, true );
  surface->setDataLayerProps( "map1", TSLPropertyRealtimeReprojection, true );

#else
  // Make the map layer buffered so it will be rendered asynchronously to tiles
  surface->setDataLayerProps( "map", TSLPropertyBuffered, true );
  surface->setDataLayerProps( "map1", TSLPropertyBuffered, true );

  // As we have buffered layer tiling enabled, turn on text and symbol view expansion to reduce
  // incidents of text and symbols being clipped at the tile boundaries
  surface->setDataLayerProps( "map", TSLPropertyLoadedSymbolsAndTextViewExpansion, 20 );
  surface->setDataLayerProps( "map1", TSLPropertyLoadedSymbolsAndTextViewExpansion, 20 );
#endif

  //surface->addDataLayer( m_rasterLayer, "raster" );
  surface->addDataLayer( m_trackLayer, "track" );
  surface->addDataLayer( m_fpsLayer, "fps" );

  waypointMode->setTargetLayer( m_trackClient );
}


#ifdef HAVE_DIRECT_IMPORT_SDK
// Create a direct import layer
TSLDirectImportDataLayer* LayerManager::createDirectImportLayer( const string& name, int cacheSize, int numProcessingThreads,
                                                                 const string& diskCacheDir, bool diskCacheFlushOnExit, int diskCacheSize,
                                                                 const TSLCoordinateSystem* cs, TSLDirectImportDataLayerCallbacks* layerCallbacks )
{
  if( m_directImportName != "" )
  {
    return NULL;
  }
  if( cs == NULL )
  {
    return NULL;
  }

  TSLDirectImportDataLayer* layer;
  m_directImportName = name;

  layer = new TSLDirectImportDataLayer(
        numProcessingThreads,
        cacheSize,
        diskCacheDir.empty() ? NULL : diskCacheDir.c_str(),
        diskCacheSize,
        diskCacheFlushOnExit );

  layer->setCallbacks( layerCallbacks );
  layer->setCoordinateSystem( cs );
  
  // The advancedsample will redraw the display as fast as possible, or in sync with the display refresh rate
  // When drawing in this manner the direct import datalayer can perform additional tweaks to reduce display stutter
  layer->enableDrawPerformanceTweaks( true );
  
  // As this sample may load and display an unknown amount of data it's best to allow extra space in the raster draw cache
  // This setting will not affect the processing of any data, or the display of vector data.
  layer->maxRasterDrawCacheSize( 1024 * 1024 );

  m_dataLayer[name] = std::make_pair( layer, TSLDataLayerTypeDirectImportDataLayer );

  // Add the direct import layer to the declutter panel
  // The layer won't contain any features until data has been loaded/processed
  // for display.
  m_declutterModel.addLayerFeatures( name.c_str(), layer );

  return layer;
}

TSLFeatureClassConfig* LayerManager::getDefaultFeatureConfig(const TSLDirectImportDataSet* dataSet)
{
  if (!dataSet)
  {
    return NULL;
  }

  SurfaceMap::const_iterator it(m_dataLayer.find(m_directImportName));
  if (it == m_dataLayer.end())
  {
    return NULL;
  }

  TSLDirectImportDataLayer* layer(reinterpret_cast<TSLDirectImportDataLayer*>(it->second.first));
  TSLFeatureClassConfig* featureConfig(NULL);
  if (dataSet->dataType() == TSLDirectImportDriver::DataTypeVector)
  {
    // Vector datasets require a TSLFeatureClassConfig in order to be loaded
    // This may be created by using the TSLDirectImportDataLayer to analyse the data
    // In most situations this should be performed as an offline process. This sample performs
    // it here in order to load any vector data with a default rendering setup.
    setupFeatureConfig(dataSet->dataPath(), layer);
    featureConfig = &m_featureClassConfig;
  }

  return featureConfig;
}

bool LayerManager::loadDataSetIntoDirectImportLayer( TSLDirectImportDataSet* dataSet, TSLDirectImportScaleBand* band, TSLFeatureClassConfig* featureConfig )
{
  if (!dataSet)
  {
    return false;
  }

  SurfaceMap::const_iterator it( m_dataLayer.find( m_directImportName ) );
  if( it == m_dataLayer.end() )
  {
    return false;
  }

  TSLDirectImportDataLayer* layer( reinterpret_cast<TSLDirectImportDataLayer*>(it->second.first) );

  if( dataSet->dataType() == TSLDirectImportDriver::DataTypeVector &&
      featureConfig == NULL)
  {
    // Vector datasets require a TSLFeatureClassConfig in order to be loaded
    // This may be created by using the TSLDirectImportDataLayer to analyse the data
    // In most situations this should be performed as an offline process. This sample performs
    // it here in order to load any vector data with a default rendering setup.
    setupFeatureConfig( dataSet->dataPath(), layer );
    featureConfig = &m_featureClassConfig;
  }

  // Load the data set into the layer
  // Assuming the parameters are valid the layer will begin processing of the data
  // in its thread pool.
  // Most of this processing will not be started until a redraw is performed.

  // Data may be loaded into a specific scale band, or automatically placed in a band
  // by the layer.
  if( band )
  {
    return band->loadData( dataSet, featureConfig );
  }
  else
  {
    return layer->loadData( dataSet, featureConfig );
  }
  return false;
}

#endif

// Load data into a layer
bool LayerManager::loadLayer( const string& path, const string& name, bool useSharedCache, int cacheSize )
{
  std::string fileType;
  size_t dotPos( path.find_last_of( "." ) );
  if( dotPos != std::string::npos )
  {
    fileType = path.substr( dotPos );
  }

  if( fileType == ".map" || fileType == ".mpc" )
  {
    TSLStaticMapDataLayer* layer = new TSLStaticMapDataLayer();

    layer->addLoader( m_mapLayerLoader1, NULL, NULL, NULL, NULL );
    if (useSharedCache)
    {
      m_sharedCache->setSharedCache(*layer);
    }
    else
    {
      layer->drawCacheSize(cacheSize);
    }

    m_dataLayer[name] = std::make_pair( layer, TSLDataLayerTypeStaticMapDataLayer );

    if( !layer->loadData( path.c_str() ) )
    {
      TSLSimpleString message;
      TSLThreadedErrorStack::errorString( message );
      QMessageBox::critical( NULL, "Failed to load .map data", message.c_str(), QMessageBox::Ok );
      return false;
    }
  }
  else if( fileType == ".kml" || fileType == ".kmz" )
  {
    TSLKMLDataLayer* layer = new TSLKMLDataLayer();

    m_dataLayer[name] = std::make_pair( reinterpret_cast<TSLDataLayer*>( layer ), TSLDataLayerTypeKMLDataLayer );

    const TSLCoordinateSystem *cs = NULL;
    const TSLDataLayer* csLayer( m_surface->getCoordinateProvidingLayer() );
    if( csLayer && csLayer->layerType() == TSLDataLayerTypeStaticMapDataLayer )
    {
      const TSLStaticMapDataLayer* csLayerMap( reinterpret_cast<const TSLStaticMapDataLayer*>( csLayer ) );
      cs = csLayerMap->queryMapCoordinateSystem();
    }

    layer->setCoordinateSystem( cs );

    if (useSharedCache)
    {
      m_sharedCache->setSharedCache(*layer);
    }

    if( !layer->loadData( path.c_str() ) )
    {
      TSLSimpleString message;
      TSLThreadedErrorStack::errorString( message );
      QMessageBox::critical( NULL, "Failed to load .kml data", message.c_str(), QMessageBox::Ok );
      return false;
    }
  }
  else if( fileType == ".tmf" )
  {
    TSLStandardDataLayer* layer = new TSLStandardDataLayer();

    m_dataLayer[name] = std::make_pair( reinterpret_cast<TSLDataLayer*>( layer ), TSLDataLayerTypeStandardDataLayer );

    if (useSharedCache)
    {
      m_sharedCache->setSharedCache(*layer);
    }

    if( !layer->loadData( path.c_str() ) )
    {
      TSLSimpleString message;
      TSLThreadedErrorStack::errorString( message );
      QMessageBox::critical( NULL, "Failed to load .tmf data", message.c_str(), QMessageBox::Ok );
      return false;
    }
  }

  return true;
}

bool LayerManager::projectionMapsLoaded() const
{
	return m_loadedBackground && m_loadedBackground2;
}

// Load a map
// This function loads the 2 background map layers.
// These are either the first 2 maps loaded, or provided via the command line.
bool LayerManager::loadMap( const char *path, TSLOpenGLSurface* surface, bool useSharedCache )
{
  bool result = false;
  if( m_loadedBackground == false )
  {
    if (m_mapLayer)
    {
      if (useSharedCache)
      {
        m_sharedCache->setSharedCache(*m_mapLayer);
      }

      if (m_mapLayer->loadData(path))
      {
        m_loadedBackground = true;

        // set the projection up - we need a consistent projection for
        // reprojected map layers.
		result = setProjection(m_currentProjection, surface);
        if(!result)
		{
		  m_mapLayer->removeData();
		  m_loadedBackground = false;
		}	
      }      
    }
  }
  else if( m_loadedBackground2 == false )
  {
    if (m_mapLayer1)
    {
      if (useSharedCache)
      {
        m_sharedCache->setSharedCache(*m_mapLayer1);
      }

      if (m_mapLayer1->loadData(path))
      {
        m_loadedBackground2 = true;

        // set the projection up - we need a consistent projection for
        // reprojected map layers.
		result = setProjection(m_currentProjection, surface);
		if(!result)
		{
		  m_mapLayer1->removeData();
		  m_loadedBackground2 = false;
		}
      }
    }
  }

  if (result)
  {
#ifdef WIN32
     m_lastFrameTime = clock();
#else
     struct timespec rs;
     clock_gettime( CLOCK_REALTIME, &rs );
     m_lastFrameTime = ( rs.tv_sec + rs.tv_nsec / 1E9 );
#endif
  }
  return result;
}

// Load a raster
bool LayerManager::loadRaster( const char * /*path*/ )
{
  /*static double xOffset = 0;
  if( m_rasterLayer->addRaster( path, true, -10000000.0 + xOffset, 10000000.0, 10000000.0 + xOffset, 10000000.0 ) )
  {
  xOffset += 20000000.0;
  QueryPerformanceCounter( &m_lastFrameTime );
  return true;
  }

  return false;*/
  return false;
}

// Update Layers
//
// Regularly updates the frame rate, projection and tracks in the 
// drawing surface.
void LayerManager::updateLayers()
{
#ifdef WIN32
  std::clock_t currentTime = clock();

  // Convert ticks to actual time
  double secsSinceLastUpdate = ( ( (float)currentTime / CLOCKS_PER_SEC ) - ( (float)m_lastFrameTime / CLOCKS_PER_SEC ) );
#else
  //clock_getres(CLOCK_REALTIME, &spec);

  struct timespec rs;
  clock_gettime( CLOCK_REALTIME, &rs );
  double secsSinceLastUpdate = ( rs.tv_sec + rs.tv_nsec / 1E9 ) - m_lastFrameTime;
#endif

  m_trackClient->updatePositions( secsSinceLastUpdate, m_timeMultiplier );
  m_fpsClient->update( secsSinceLastUpdate );

  double newProjectionCentreLat, newProjectionCentreLon;
  m_trackClient->getTrackPosition( newProjectionCentreLat, newProjectionCentreLon );

#ifdef ENABLE_PROJECTION
  if( !m_lockProjectionCentre )
  {
    m_mapLayer->setRuntimeProjectionOrigin( newProjectionCentreLat, newProjectionCentreLon, NULL, NULL );
    m_mapLayer1->setRuntimeProjectionOrigin( newProjectionCentreLat, newProjectionCentreLon, NULL, NULL );

    for( SurfaceMap::iterator it = m_dataLayer.begin(); it != m_dataLayer.end(); ++it )
    {
      if( it->second.second == TSLDataLayerTypeStaticMapDataLayer )
      {
        TSLStaticMapDataLayer* datalayer = reinterpret_cast<TSLStaticMapDataLayer*>( it->second.first );
        datalayer->setRuntimeProjectionOrigin( newProjectionCentreLat, newProjectionCentreLon, NULL, NULL );
      }
    }
  }
#endif

#ifdef WIN32
  m_lastFrameTime = clock();
#else
  m_lastFrameTime = ( rs.tv_sec + rs.tv_nsec / 1E9 );
#endif
}

// Set Time Acceleration Factor
//
// Changes the acceleration factor of the projection object to the
// value passed to it.
void LayerManager::setTimeAccelerationFactor( double factor )
{
  m_timeMultiplier = factor;
}

void LayerManager::setCurrentProjection( ProjectionType type )
{
  m_currentProjection = type;
}

LayerManager::ProjectionType LayerManager::getCurrentProjection()
{
  return m_currentProjection;
}

// Set Projection
//
// Changes the selected projection to one of the many available.
//
// Currently: Stereographic, TransverseMercatorUSGS and 
// GnomicSphericalEarth.
bool LayerManager::setProjection( ProjectionType type, TSLOpenGLSurface *surface )
{
  // set coordinate system from the input projection type.
  double newProjectionCentreLat, newProjectionCentreLon;
  m_trackClient->getTrackPosition( newProjectionCentreLat, newProjectionCentreLon );

  bool setOK = false;
  double tilt = 0.0;
  // could instantiate a projection by listing those in the coordinate system registry and picking one, 
  // in which case there would be no need to code the WGS84 parameters etc. as the ellipsoid would be 
  // inferred in the coordinate system.  I've hard coded these to keep the number of options down.
  double minimumRange = 25000000;

  TSLCoordinateSystem* coordinateSystem = nullptr;
  switch( type )
  {
  case Orthographic:
  {
    // Orthographic tilted
    coordinateSystem = TSLCoordinateSystem::createByDatumName( 10002, "CS", "Spherical Earth: Mean Radius" );  // this CS needs spherical datum
    if( coordinateSystem )
    {
      setOK = coordinateSystem->setProjectionOrthographic( newProjectionCentreLat, newProjectionCentreLon );
    }
    tilt = -23.5;
    break;
  }

  case ObliqueCylindricalEqualAreaPoint:
  {
    coordinateSystem = TSLCoordinateSystem::createByDatumName( 10003, "CS", "Spherical Earth: Mean Radius" );  // this CS needs spherical datum
    if( coordinateSystem )
    {
      setOK = coordinateSystem->setProjectionObliqueCylindricalEqualArea_CentralPoint( newProjectionCentreLat, newProjectionCentreLon, 0.0, 1.0 );
    }
    break;
  }

  case LambertAzimuthal:
  {
    coordinateSystem = TSLCoordinateSystem::createByDatumName( 10004, "CS", "World Geodetic System 1984" );
    if( coordinateSystem )
    {
      setOK = coordinateSystem->setProjectionLambertAzimuthalEqualArea( newProjectionCentreLat, newProjectionCentreLon );
    }
    break;
  }

  case AlbersEqualArea:
  {
    coordinateSystem = TSLCoordinateSystem::createByDatumName( 10004, "CS", "World Geodetic System 1984" );
    if( coordinateSystem )
    {
      setOK = coordinateSystem->setProjectionAlbersEqualAreaConic( 20.0, 60.0, newProjectionCentreLat, newProjectionCentreLon );
    }
    break;
  }

  case Bonne:
  {
    coordinateSystem = TSLCoordinateSystem::createByDatumName( 10003, "CS", "Spherical Earth: Mean Radius" );
    if( coordinateSystem )
    {
      setOK = coordinateSystem->setProjectionBonne( 20.0, newProjectionCentreLon );
    }
    break;
  }

  case Cassini:
  {
    coordinateSystem = TSLCoordinateSystem::createByDatumName( 10003, "CS", "Spherical Earth: Mean Radius" );
    if( coordinateSystem )
    {
      setOK = coordinateSystem->setProjectionCassini( newProjectionCentreLat, newProjectionCentreLon );
    }
    break;
  }

  case None:
  {
    return false;
  }

  case LayerManager::GnomicSphericalEarth:
  {
    coordinateSystem = TSLCoordinateSystem::createByDatumName( 10002, "CS", "Spherical Earth: Mean Radius" );  // this CS needs spherical datum
    if( coordinateSystem )
    {
      setOK = coordinateSystem->setProjectionGnomonic( newProjectionCentreLat, newProjectionCentreLon );
    }

    //Minimum Map Unit width the surface will zoom to
    minimumRange = 15000000;

    break;
  }

  case TransverseMercatorUSGS:
  {
    coordinateSystem = TSLCoordinateSystem::createByDatumName( 10004, "CS", "World Geodetic System 1984" );

    if( coordinateSystem )
    {
      setOK = coordinateSystem->setProjectionTransverseMercator( newProjectionCentreLat, newProjectionCentreLon, 1.0, TSLCoordinateSystem::TSLTransverseMercatorFormulaUSGS );
    }
    
    //Minimum Map Unit width the surface will zoom to
    minimumRange = 5000000;

    break;
  }

  case Mercator:
  {
    coordinateSystem = TSLCoordinateSystem::createByDatumName( 10005, "CS", "World Geodetic System 1984" );

    if( coordinateSystem )
    {
      setOK = coordinateSystem->setProjectionMercator( newProjectionCentreLon, 0.0 );
    }

    //Minimum Map Unit width the surface will zoom to
    minimumRange = 25000000;

    break;
  }

  case Stereographic:
  default:
  {
    // Stereographic
    coordinateSystem = TSLCoordinateSystem::createByDatumName( 10001, "CS", "World Geodetic System 1984" );
    if( coordinateSystem )
    {
      setOK = coordinateSystem->setProjectionStereographic( newProjectionCentreLat, newProjectionCentreLon, 1.0 );
    }

    //Minimum Map Unit width the surface will zoom to
    minimumRange = 25000000;

    break;
  }
  }

  if (!coordinateSystem)
  {
	  // Display any errors that have occurred
	  QMessageBox::critical(NULL, "Set projection error", "Invalid Null coordinate system.");
	  return false;
  }
  if (!setOK)
  {
	  // Display any errors that have occurred
	  QMessageBox::critical(NULL, "Set projection error", "Failed to set projection.");
	  coordinateSystem->destroy();
	  return false;
  }

#ifdef ENABLE_PROJECTION
  if( m_mapLayer && m_loadedBackground )
  {
	TSLErrorStack::clear();
    // set the projection synchronously
    m_mapLayer->clearRuntimeProjection();
	if (!m_mapLayer->setRuntimeProjection(coordinateSystem, m_greatCircleDistance, NULL, NULL))
	{
		if(m_currentCoordinateSystem)
			m_mapLayer->setRuntimeProjection(m_currentCoordinateSystem, m_greatCircleDistance, NULL, NULL);
		
		// Display any errors that have occurred
		const char *errorMsg = TSLErrorStack::errorString();
		if (errorMsg)
		{
			QMessageBox::critical(NULL, "Error while setting run time projection.", errorMsg);
			TSLErrorStack::clear();
		}
		coordinateSystem->destroy();
		return false;
	}
    if (m_mapLayer1 && m_loadedBackground2)
    {
      m_mapLayer1->clearRuntimeProjection();
      if( !m_mapLayer1->setRuntimeProjection(coordinateSystem, m_greatCircleDistance, NULL, NULL ) )
	  {
		  if(m_currentCoordinateSystem)
			m_mapLayer1->setRuntimeProjection(m_currentCoordinateSystem, m_greatCircleDistance, NULL, NULL);
		  
		  if( m_mapLayer && m_loadedBackground && m_currentCoordinateSystem)
		    m_mapLayer->setRuntimeProjection(m_currentCoordinateSystem, m_greatCircleDistance, NULL, NULL);
		  
		  // Display any errors that have occurred
		  const char *errorMsg = TSLErrorStack::errorString();
		  if (errorMsg)
		  {
			  QMessageBox::critical(NULL, "Error while setting run time projection.", errorMsg);
			  TSLErrorStack::clear();
		  }
		  coordinateSystem->destroy();
		  return false;
	  }
    }
    // Update the track manager 
    if (m_mapLayer->queryMapCoordinateSystem())
    {
      TSLCoordinateSystem *trackThreadCS = m_mapLayer->queryMapCoordinateSystem()->clone(1000);
      TSLEnvelope extent;
      m_mapLayer->getTMCExtent( &extent.m_bottomLeft.m_x, &extent.m_bottomLeft.m_y, &extent.m_topRight.m_x, &extent.m_topRight.m_y );

      TrackManager::instance().setCoordinateAttributes( extent.bottomLeft().x(), extent.bottomLeft().y(), extent.topRight().x(), extent.topRight().y(), trackThreadCS );
    }

    // Update all the realtime reprojection map layers
    SurfaceMap::iterator it( m_dataLayer.begin() );
    SurfaceMap::iterator end( m_dataLayer.end() );
    while( it != end )
    {
      // SurfaceMap structure: < QString Name(first), < TSLDataLayer*(second, first), QString layerType (second, second) >
      if( it->second.second == TSLDataLayerTypeStaticMapDataLayer )
      {
        TSLStaticMapDataLayer* layer = reinterpret_cast<TSLStaticMapDataLayer*>( it->second.first );
        layer->clearRuntimeProjection();
        if( !layer->setRuntimeProjection(coordinateSystem, m_greatCircleDistance, NULL, NULL ) )
		{
			if(m_currentCoordinateSystem)
				layer->setRuntimeProjection(m_currentCoordinateSystem, m_greatCircleDistance, NULL, NULL);
			// Display any errors that have occurred
			const char *errorMsg = TSLErrorStack::errorString();
			if (errorMsg)
			{
				QMessageBox::critical(NULL, "Error while setting run time projection.", errorMsg);
				TSLErrorStack::clear();
			}
			coordinateSystem->destroy();
			return false;
		}
      }
      ++it;
    }
    double x1, y1, x2, y2;
    surface->getMUExtent( &x1, &y1, &x2, &y2 );

    // Size of display area in Map Units
    double extentWidth = ( x2 > x1 ) ? ( x2 - x1 ) : ( x1 - x2 );

    //Minimum Map Unit width the surface will zoom to
    if( extentWidth > minimumRange )
    {
      extentWidth = minimumRange;
    }

    surface->setViewedLatLongRange( newProjectionCentreLat, newProjectionCentreLon, extentWidth );

    // Force pan to centre of screen
    TSLDeviceUnits duX1, duX2, duY1, duY2;
    surface->getDUExtent(&duX1, &duY1, &duX2, &duY2);
    TSLDeviceUnits xCtr = (duX2 - duX1) / 2;
    TSLDeviceUnits yCtr = (duY2 - duY1) / 2;
    double uuXCtr, uuYCtr;
    surface->DUToUU(xCtr, yCtr, &uuXCtr, &uuYCtr);
    surface->pan(uuXCtr, uuYCtr, false);
  }
#endif

  // update the current coordinate system and projection.
  if (m_currentCoordinateSystem)
  {
	  m_currentCoordinateSystem->destroy();
	  m_currentCoordinateSystem = NULL;
  }
  m_currentCoordinateSystem = coordinateSystem;
  m_currentProjection = type;
  return true;
}


// Get Output Coordinate System
//
// Returns the currently selected output coordinate system used
// by the data layers.
TSLCoordinateSystem* LayerManager::getOutputCoordinateSystem() const
{
  return m_currentCoordinateSystem;
}

// Lock Projection Origin
//
// Pauses the current state of the projected data layers.
void LayerManager::lockProjectionOrigin( bool lock )
{
  m_lockProjectionCentre = lock;
}

// Get Layer Count
//
// Takes the passed type and returns the amount of times that type
// has been imported into the application since it was started by
// the user.
int LayerManager::getLayerCount( TSLDataLayerTypeEnum name )
{
  if( m_layerCount.find( name ) != m_layerCount.end() )
  {
    return m_layerCount.find( name )->second;
  }
  else
  {
    return ( m_layerCount[name] = 0 );
  }
}

// Increment Layer Count
//
// Takes the passed type and increments the counter for the amount
// of that layer.
void LayerManager::incrementLayerCount( TSLDataLayerTypeEnum name )
{
  std::map< TSLDataLayerTypeEnum, int >::iterator it( m_layerCount.find( name ) );
  if( it == m_layerCount.end() )
  {
    return;
  }

  ++( it->second );
}

// Remove Layer
//
// Removes the selected layer from the drawing surface and destroys
// it.
void LayerManager::removeLayer( const string& name )
{
  SurfaceMap::iterator it = m_dataLayer.find( name );
  if( it == m_dataLayer.end() )
  {
    return;
  }
#ifdef HAVE_DIRECT_IMPORT_SDK
  if( it->second.second == TSLDataLayerTypeDirectImportDataLayer )
  {
    m_directImportName = "";
  }
#endif

  it->second.first->destroy();
  m_dataLayer.erase( it );

  m_declutterModel.removeLayerFeatures( QString(name.c_str()) );
}

void LayerManager::setupRasterSymbol()
{
  TSLStyleID black = TSLDrawingSurface::getIDOfNearestColour( 0, 0, 0 );

  TSLStandardDataLayer* layer = reinterpret_cast<TSLStandardDataLayer*>( m_entityLayer );
  // Make up a feature name and numeric ID
  layer->addFeatureRendering( "Airport", 123 );

  // Associate some rendering with the new feature, use ID for efficiency
  layer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolStyle, 6003 );
  layer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolColour, black );
  layer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolSizeFactor, 40.0 );
  layer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolSizeFactorUnits, TSLDimensionUnitsPixels );
}

void LayerManager::setFramerateLayerVisibility( TSLOpenGLSurface *surface, bool isVisible )
{
  surface->setDataLayerProps( "fpsLayer", TSLPropertyVisible, isVisible );
}

void LayerManager::resetLayers( TSLOpenGLSurface * /*surface*/ )
{

}

#ifdef HAVE_DIRECT_IMPORT_SDK
const std::string& LayerManager::getDirectImportName() const
{
  return m_directImportName;
}

TSLDirectImportDataLayer* LayerManager::directImportLayer()
{
  if( m_directImportName.empty() )
  {
    return NULL;
  }
  TSLDataLayer* layer( getLayer( m_directImportName ) );
  return reinterpret_cast<TSLDirectImportDataLayer*>(layer);
}

void LayerManager::directImportLayer( TSLDirectImportDataLayer* layer, const std::string& layerName )
{
  if( !m_directImportName.empty() )
  {
    // There can only be one direct import layer in this sample
    // this function should never be called more than once
    return;
  }
  m_dataLayer[layerName] = std::make_pair(layer, TSLDataLayerTypeDirectImportDataLayer);
}

void LayerManager::moveDirectImportDataSetToIndex( const char* /*layerName*/, const char* scaleBandName, int rowFrom, int rowTo )
{
  TSLDirectImportDataLayer* dl( directImportLayer() );
  if( !dl || !scaleBandName )
  {
    return;
  }

  std::string scaleBandNameStr( scaleBandName );
  TSLDirectImportScaleBand* scaleBand( NULL );
  unsigned int numScaleBands( dl->numScaleBands() );
  for( unsigned int i(0); i < numScaleBands; ++i )
  {
    scaleBand = dl->getScaleBand( i );
    if( scaleBandNameStr == scaleBand->name() )
    {
      break;
    }
    scaleBand = NULL;
  }
  if( !scaleBand )
  {
    return;
  }

  scaleBand->moveData( rowFrom, rowTo );

}

void LayerManager::removeDirectImportDataSet( const char* /*layerName*/, const char* scaleBandName, int row )
{
  TSLDirectImportDataLayer* dl( directImportLayer() );
  if( !dl || !scaleBandName )
  {
    return;
  }

  std::string scaleBandNameStr( scaleBandName );
  TSLDirectImportScaleBand* scaleBand( NULL );
  unsigned int numScaleBands( dl->numScaleBands() );
  for( unsigned int i(0); i < numScaleBands; ++i )
  {
    scaleBand = dl->getScaleBand( i );
    if( scaleBandNameStr == scaleBand->name() )
    {
      break;
    }
    scaleBand = NULL;
  }
  if( !scaleBand )
  {
    return;
  }

  scaleBand->removeData( row );
  m_declutterModel.removeLayerFeatures(m_directImportName.c_str());
  m_declutterModel.addLayerFeatures(m_directImportName.c_str(), directImportLayer());
}

TSLDirectImportDataSet* LayerManager::getDirectImportDataSet( const char* /*layerName*/, const char* scaleBandName, int row )
{
  TSLDirectImportDataLayer* dl( directImportLayer() );
  if( !dl || !scaleBandName )
  {
    return NULL;
  }

  std::string scaleBandNameStr( scaleBandName );
  TSLDirectImportScaleBand* scaleBand( NULL );
  unsigned int numScaleBands( dl->numScaleBands() );
  for( unsigned int i(0); i < numScaleBands; ++i )
  {
    scaleBand = dl->getScaleBand( i );
    if( scaleBandNameStr == scaleBand->name() )
    {
      break;
    }
    scaleBand = NULL;
  }
  if( !scaleBand )
  {
    return NULL;
  }

  return scaleBand->getDataSet( row );
}

#endif

TSLDataLayer* LayerManager::getLayer( const string& name )
{
  if( name == "EntityLayer" )
  {
    return m_entityLayer;
  }
  else
  {
    SurfaceMap::const_iterator it = m_dataLayer.find( name );
    return (it != m_dataLayer.end()) ? it->second.first : NULL;
  }
}

int LayerManager::getCacheSize( const std::string& layerName ) const
{
  SurfaceMap::const_iterator it = m_dataLayer.find( layerName );

  if( it != m_dataLayer.end() )
  {
    TSLDataLayerTypeEnum fileType = it->second.second;

    if( fileType == TSLDataLayerTypeStandardDataLayer )
    {
      //TSLStandardDataLayer* layer = reinterpret_cast<TSLStandardDataLayer*>( it->second.first );
      // No cache
    }
    else if( fileType == TSLDataLayerTypeMapDataLayer )
    {
      TSLMapDataLayer* layer = reinterpret_cast<TSLMapDataLayer*>( it->second.first );
      return layer->cacheSize();
    }
    else if( fileType == TSLDataLayerTypeStaticMapDataLayer )
    {
      TSLStaticMapDataLayer* layer = reinterpret_cast<TSLStaticMapDataLayer*>( it->second.first );
      return layer->drawCacheSize();
    }
    else if( fileType == TSLDataLayerTypeKMLDataLayer )
    {
      //TSLKMLDataLayer* layer = reinterpret_cast<TSLKMLDataLayer*>( it->second.first );
      // No cache
    }
#ifdef HAVE_DIRECT_IMPORT_SDK
    else if( fileType == TSLDataLayerTypeDirectImportDataLayer )
    {
      TSLDirectImportDataLayer* layer = reinterpret_cast<TSLDirectImportDataLayer*>( it->second.first );
      return layer->maxMemoryCacheSize();
    }
# endif
  }
  else if( layerName == "map" )
  {
    return m_mapLayer->drawCacheSize();
  }
  else if( layerName == "map1" )
  {
    return m_mapLayer1->drawCacheSize();
  }
  return 0;
}

void LayerManager::editCacheSize(const std::string& layerName, int cacheVal )
{
  SurfaceMap::iterator it = m_dataLayer.find( layerName );

  if( it != m_dataLayer.end() )
  {

    TSLDataLayerTypeEnum fileType = it->second.second;

    if( fileType == TSLDataLayerTypeStandardDataLayer )
    {
      //TSLStandardDataLayer* layer = reinterpret_cast<TSLStandardDataLayer*>( it->second.first );
      // No cache
    }
    else if( fileType == TSLDataLayerTypeMapDataLayer )
    {
      TSLMapDataLayer* layer = reinterpret_cast<TSLMapDataLayer*>( it->second.first );
      layer->cacheSize( cacheVal );
    }
    else if( fileType == TSLDataLayerTypeStaticMapDataLayer )
    {
      TSLStaticMapDataLayer* layer = reinterpret_cast<TSLStaticMapDataLayer*>( it->second.first );
      layer->drawCacheSize( cacheVal );
    }
    else if( fileType == TSLDataLayerTypeKMLDataLayer )
    {
      // TSLKMLDataLayer* layer = reinterpret_cast<TSLKMLDataLayer*>( it->second.first );
      // No cache
    }
#ifdef HAVE_DIRECT_IMPORT_SDK
    else if( fileType == TSLDataLayerTypeDirectImportDataLayer )
    {
      TSLDirectImportDataLayer* layer = reinterpret_cast<TSLDirectImportDataLayer*>( it->second.first );
      layer->maxMemoryCacheSize( cacheVal );
    }
# endif
  }
  else if( layerName == "map" )
  {
    m_mapLayer->drawCacheSize( cacheVal );
  }
  else if( layerName == "map1" )
  {
    m_mapLayer1->drawCacheSize( cacheVal );
  }
}

void LayerManager::useSharedCache(TSLDataLayer& dataLayer)
{
  m_sharedCache->setSharedCache(dataLayer);
}

void LayerManager::setUsedCacheSize(int cacheSize)
{
  // cacheSize parameter is in KB, shared cache size is specified in MB
  m_sharedCache->setCacheSize(cacheSize / 1024);
}
