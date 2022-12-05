/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/


#include <QtGui>
#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>

#include <osgEarth/TerrainEngineNode>
#include <osgEarthUtil/ExampleResources>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/LODBlending>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/cache_filesystem/FileSystemCache>

// Avoid including the MapLink drawing surface and X11 headers
// at this point to avoid conflicts with Qt headers included
// through mainwindow.h
#define MAPLINK_NO_DRAWING_SURFACE
#include <osgEarthMapLink/MilitarySymbols.h>
#include <osgEarthMapLink/Symbols.h>
#include <osgEarthMapLink/DataLayerTileSource.h>
#include <osgEarthMapLink/TerrainTileSource.h>

#include "mainwindow.h"
#include "viewereventfilter.h"

#include <MapLink.h>
#include <MapLinkDrawing.h>
#include "maplinktrackmanager.h"
#include <MapLinkTerrain.h>
#ifdef MAPLINK_HAVE_KML
# include <tslkmldatalayer.h>
#endif

using namespace osgEarth;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , m_osgEarthMap( NULL )
  , m_osgViewer( NULL )
  , m_earthViewer( NULL )
  , m_mapNode( NULL )
  , m_rootNode( NULL )
  , m_trackManager( NULL )
  , m_skyNode( NULL )
  , m_skyBoxMoonEnabled( false )
  , m_skyBoxAnimationEnabled( false )
{
  // Construct the window
  setupUi( this );

  // Connect the actions for the toolbars and menus to the slots that deal with them
  connect( actionOpen, SIGNAL( triggered() ), this, SLOT( openMapLinkData() ) );
  connect( actionExit, SIGNAL( triggered() ), this, SLOT( exit() ) );
  connect( actionAbout, SIGNAL( triggered() ), this, SLOT( showAboutBox() ) );
  connect( actionLighting, SIGNAL( toggled( bool ) ), this, SLOT( setLighting( bool ) ) );

  connect( actionSkyMoon, SIGNAL( toggled( bool ) ), this, SLOT( setSkyBoxMoon( bool ) ) );
  connect( actionSkyAnimation, SIGNAL( toggled( bool ) ), this, SLOT( setSkyBoxAnimated( bool ) ) );

  connect( actionSimulation_Decluttering, SIGNAL( toggled( bool ) ), this, SLOT( setDecluttering( bool ) ) );
  connect( actionSimulation_Options, SIGNAL( triggered() ), this, SLOT( showSimulationOptions() ) );
}

MainWindow::~MainWindow()
{
  if( m_earthViewer )
  {
    delete m_earthViewer;
  }
}

bool MainWindow::initMapLink()
{
  // Get the MapLink home directory
  const char *mapLinkHome = TSLUtilityFunctions::getMapLinkHome();

  // Load the standard MapLink resource files.
  if( !TSLDrawingSurface::loadStandardConfig(NULL) )
  {
    QMessageBox::critical( NULL, "Maplink Initialisation Failed", "Error: Failed to initialise Maplink: Failed to load standard config" );
    return false;
  }

  // Load the coordinate system information.
  std::string transforms;
  if (NULL != mapLinkHome)
  {
    transforms = mapLinkHome;
    transforms += "/config/tsltransforms.dat";
    if( !TSLCoordinateSystem::loadCoordinateSystems(transforms.c_str()) )
    {
      QMessageBox::critical( NULL, "Maplink Initialisation Failed", "Error: Failed to initialise Maplink: Failed to load coordinate systems" );
      return false;
    }
  }

  // Load the APP6a symbols file
  std::string symbolsFile;
  if ( NULL != mapLinkHome )
  {
    // Get the specific symbols.dat file
    symbolsFile = mapLinkHome ;
    symbolsFile += "/config/tslsymbolsAPP6A.dat" ;
  }
  else
  {
    symbolsFile = "tslsymbolsAPP6A.dat";
  }

  // Set the symbols to include APP6A/2525B symbols.
  if( !TSLDrawingSurface::setupSymbols( symbolsFile.c_str() ) )
  {
    QMessageBox::critical( NULL, "Maplink Initialisation Failed", "Error: Failed to initialise Maplink: Failed to setup APP6A symbols" );
    return false;
  }
  
  // Unlock support for the CADRG data layer
  // TSLUtilityFunctions::unlockSupport(TSLKeyedCADRGDataLayer, "unlockcode");
  
  return true;
}

bool MainWindow::initOsgEarth(osg::ArgumentParser& args)
{
#ifdef _DEBUG
  if ( !getenv( "OSG_NOTIFY_LEVEL" ) )
    osg::setNotifyLevel( osg::DEBUG_FP );
#endif

  // Setup the data path so that the moon texture can be found
#ifdef WIN32
  QString dataPath("OSG_FILE_PATH=");
  dataPath += TSLUtilityFunctions::getMapLinkHome();
  dataPath += "/OSGEarth/osgearth/data";
  _wputenv(dataPath.toStdWString().c_str());
#else
  QString dataPath(TSLUtilityFunctions::getMapLinkHome());
  dataPath += "/osgearth/data";
  setenv("OSG_FILE_PATH", dataPath.toUtf8(), true );

  // Crashes have been encountered after the application has started, when trying to load osg plugins
  // The cause of this is unknown, but a workaround is to load the plugins here.
  // This was initially encountered when the stats overlay tried to load libfreetype.so, but the rest
  // are listed here just incase.
  osgDB::Registry::instance()->loadLibrary( osgDB::Registry::instance()->createLibraryNameForExtension("ttf") );
  osgDB::Registry::instance()->loadLibrary( osgDB::Registry::instance()->createLibraryNameForExtension("zip") );
  osgDB::Registry::instance()->loadLibrary( osgDB::Registry::instance()->createLibraryNameForExtension("jpeg") );
#endif

  m_osgViewer = new osgViewer::Viewer( args );

  // Need to render continuously for the track simulation
  // Some threading modes, or not calling setUpThreading and startThreading
  // can cause the tracks to flicker.
  // This is most likely due to the fact the track updates in this sample
  // are performed every frame.
  // For best performance the track locaiton updates should be performed in
  // a background thread, leaving a minimal update for each frame.
  // See MaplinkTrackManager::update
  
  // The osg/osgEarth Qt integration is more stable when running in single threaded mode.
  // This is especially important when using osgEarthQt with Qt5.
  // If multi-threaded modes are required then the Qt integration will have to be setup
  // by the application instead of using the provided osgEarthQt widgets.

  // XInitThreads() also needs to be called at the start of main(),
  // as osgQt will use X11 calls from background threads.
  m_osgViewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
  m_osgViewer->setCameraManipulator( new Util::EarthManipulator() );
  m_osgViewer->setRunFrameScheme( osgViewer::ViewerBase::CONTINUOUS );
  m_osgViewer->setUpThreading();
  m_osgViewer->startThreading();

  // Setup the caching options
  std::string cacheDir = TSLUtilityFunctions::getMapLinkUserHome();
  cacheDir += "/osgEarthSampleCache";
  if( !TSLFileHelper::createDirectory( cacheDir.c_str() ) )
  {
    QString message("Error: Failed to initialise OSGEarth: Could not create cache directory: ");
    message += cacheDir.c_str();
    QMessageBox::critical( NULL, "OSGEarth Initialisation Failed",  message );
    return false;
  }

  // Setup the cache options
  Drivers::FileSystemCacheOptions cacheOptions;
  cacheOptions.rootPath() = cacheDir.c_str();
  Drivers::MapOptions mapOptions;
  mapOptions.cache() = cacheOptions;
  m_osgEarthMap = new Map( mapOptions );

  // Initialise the osg node and views
  Drivers::TerrainOptions terrainOptions;
  // bring in the tiles earlier the higher the value
  terrainOptions.minTileRangeFactor() = 6;

  terrainOptions.verticalScale() = TERRAIN_VERTICALSCALE;
  terrainOptions.lodTransitionTime() = 0.25;
  terrainOptions.enableMipmapping() = false;
  terrainOptions.enableLighting() = false;

  terrainOptions.firstLOD() = 2;
  terrainOptions.setDriver( "mp" );

  Drivers::MapNodeOptions nodeOptions;
  nodeOptions.enableLighting() = false;
  nodeOptions.setTerrainOptions( terrainOptions );

  // The MapNode will render the Map object in the scene graph.
  m_mapNode = new MapNode( m_osgEarthMap, nodeOptions );

  // LOD blending - attempt to smoothly morph vertices and image textures
  // from one LOD to the next as you zoom in or out
  osgEarth::TerrainEngineNode* terrainEngine(m_mapNode->getTerrainEngine());
  if(!terrainEngine)
  {
    QString message("Error: Failed to initialise OSGEarth: Terrain engine has not been initialised.");
    QMessageBox::critical(NULL, "OSGEarth Initialisation Failed", message);
    return false;
  }

  Util::LODBlendingOptions effectOptions;
  effectOptions.duration() = 0.15;
  Util::LODBlending* effect = new Util::LODBlending( effectOptions );

  terrainEngine->addEffect( effect );

  // Add the MapNode to the scene
  m_rootNode = new osg::Group();

  // Stock configuration
  Util::MapNodeHelper().parse( m_mapNode, args, m_osgViewer, m_rootNode, new Util::Controls::LabelControl("MapLink OsgEarth Sample"));
  Util::MapNodeHelper().configureView( m_osgViewer );

  // Initialise the sky
  Util::SkyOptions skyOptions;
  skyOptions.ambient() = 0.4f;
  skyOptions.hours() = 0;

  m_skyNode = Util::SkyNode::create( skyOptions, m_mapNode );
  m_skyNode->setMoonVisible( false );
  m_skyNode->setDateTime( DateTime( SUN_YEAR, SUN_MONTH, SUN_DAY, 0 ) );
  m_skyNode->attach( m_osgViewer );

  m_skyNode->setUpdateCallback( NULL );

  // Setup the node structure
  // The map node must be a child of the sky node, 
  // for the lighting to work. 
  m_rootNode->addChild( m_skyNode );
  m_skyNode->addChild( m_mapNode );
  m_osgViewer->setSceneData( m_rootNode );

  // Set the Qt widget to the OSGEarth viewer
  m_earthViewer = new QtGui::ViewerWidget( m_osgViewer );

  // Multi-threaded threading models don't work with osgEarthQt when using Qt5
  // Install an event filter to prevent the user activating one
  m_earthViewer->installEventFilter( new ViewerEventFilter( m_earthViewer ) );

  setCentralWidget( m_earthViewer );

  // Setup the lighting
  setLighting( false );

  // Initialise the tracks simulation
  m_trackManager = new MaplinkTrackManager( TRACKS_INITIALNUMBER, m_rootNode, m_mapNode, true, this );
  m_osgViewer->addUpdateOperation( m_trackManager );

  // Add the background MapLink map
  std::string naturalEarthRasterMap = TSLUtilityFunctions::getMapLinkHome();
  naturalEarthRasterMap += "/maps/NaturalEarthRaster/NaturalEarthRaster.map";
  if( !addMapLinkData( naturalEarthRasterMap.c_str(), TSLDataLayerTypeMapDataLayer, false ) )
  {
    QString message( "Error: Failed to initialise OSGEarth: Could not load background map: " );
    message += naturalEarthRasterMap.c_str();
    QMessageBox::critical( NULL, "OSGEarth Initialisation Failed", message );
    return false;
  }

  return true;
}

bool MainWindow::addMapLinkData( const char* fileName, TSLDataLayerTypeEnum layerType, bool limitZoomDisplay )
{
  // To keep things simple this sample creates a new OSGEarth imagery layer for each Maplink datalayer.
  // If lots of datalayers, with a common coordinate system are to be loaded, they should be added to 
  // a single OSGEarth imagery layer to boost performance.
  //
  // Maps loaded via this sample should use either dynamic arc, or a lat/lon coordinate systems
  // coordinate systems such as British National Grid may load, but will have rendering issues.

  QProgressDialog progress("Loading Data...", "Cancel",0, 5, this);
  progress.setWindowModality(Qt::WindowModal);

  progress.show();

  progress.setValue(1);

  // Create a new Maplink tile source
  envitia::MapLink::DataLayerTileSourceOptions options;
  // Set the tile size used by the maplink drawing surface
  //
  // This will affect the percieved quality of the drawn map
  // and the scaling of any maplink entity that was specified in
  // points or pixels.
  //
  // This may also heavily affect performance, if a lot of data
  // has been loaded via maplink
  //
  // The value used will always be odd, to support LOD blending.
  options.tileSize() = IMAGERY_TILESIZE;
  
  envitia::MapLink::DataLayerTileSource *imagery =
    new envitia::MapLink::DataLayerTileSource(options);

  TSLDataLayer *dataLayer = NULL;
  switch( layerType )
  {
    case TSLDataLayerTypeMapDataLayer:
      dataLayer = new TSLMapDataLayer;
      break;
#ifdef MAPLINK_HAVE_KML
    case TSLDataLayerTypeKMLDataLayer:
      {
        // The KML DataLayer doesn't provide its own coordinate system.
        // One must be created as its being displayed in its own OSGEarth layer
        TSLKMLDataLayer* kmlLayer = new TSLKMLDataLayer;
        kmlLayer->setCoordinateSystem( TSLCoordinateSystem::findByName("Dynamic ARC Grid (Greenwich)") );

        dataLayer = (TSLDataLayer*)kmlLayer;
      }
      break;
#endif
    case TSLDataLayerTypeCADRGDataLayer:
      dataLayer = new TSLCADRGDataLayer;
      break;
    default:
      return false;
      break;
  }
  
  progress.setValue(2);

  if( !dataLayer->loadData( fileName ) )
  {
    return false;
  }

  // Add the MapLink map to the TileSource.
  if( !imagery->addDataLayer(fileName, dataLayer) )
  {
    return false;
  }

  progress.setValue(3);

  // Initialise the TileSource
  // This should be called after adding datalayers to the TileSource
  // to ensure the coordinate system and extent provided to osgEarth
  // are correct
  imagery->open();

  // Set the properties for the datalayer such that it will only display once 
  // the view has been zoomed in a defined amount.
  
  // When the maps extent(in pixels) is greater than tileSize * mapScaleFraction
  // the map will be displayed.
  if( limitZoomDisplay )
  {
    double mapScaleFraction = MAPLINKIMAGERY_MINIMUMZOOMFACTOR;
    int tileSize = IMAGERY_TILESIZE; //TODO: Define in header etc
    
    TSLTMC x1 = 0;
    TSLTMC x2 = 0;
    TSLTMC y1 = 0;
    TSLTMC y2 = 0;

    bool validExtent = false;
#ifdef MAPLINK_HAVE_KML
    // For the KML DataLayer, use the extent of the vector data
    if( layerType == TSLDataLayerTypeKMLDataLayer )
    {
      TSLKMLDataLayer* kmlLayer = (TSLKMLDataLayer*)dataLayer;
      validExtent = kmlLayer->getLayer(0)->getTMCExtent( &x1, &y1, &x2, &y2 );
    }
    else
#endif
    {
      validExtent = dataLayer->getTMCExtent(&x1, &y1, &x2, &y2);
    }

    if( validExtent )
    {
      int width  = x2 - x1;
      int height = y2 - y1;
      int mapSizeTMCs = width > height ? width : height;

      // As the view is zoomed in, the TMCs per pixel value decreases, as such we must set
      // the TSLPropertyMaxZoomDisplay, to define the minimum zoom that the map is displayed for
      int maxZoomDisplay = mapSizeTMCs / (mapScaleFraction * tileSize);

      // Set the calculated factor on the datalayer, via the tilesource's drawing surface
      imagery->lock();
      imagery->drawingSurface()->setDataLayerProps(fileName, TSLPropertyMaxZoomDisplay, maxZoomDisplay );
      imagery->unlock();  
    }
  }
  
  // Create an osgEarth ImageLayer.
  Drivers::Config layerDriverConf;
  layerDriverConf.add( "default_tile_size", IMAGERY_TILESIZE_STR );
  Drivers::ImageLayerOptions maplinkImageLayerOptions( layerDriverConf );
  maplinkImageLayerOptions.name() = fileName;

  // Specify the ImageLayer options
  optional<CachePolicy> &cachePolicy = 
  maplinkImageLayerOptions.cachePolicy();
  cachePolicy = CachePolicy::NO_CACHE;

  progress.setValue(4);

  optional<float>& opacity = maplinkImageLayerOptions.opacity();
  opacity = 1.0;

  // Attach the MapLink 2D TileSource to the ImageLayer
  ImageLayer *maplinkImageLayer = new ImageLayer(maplinkImageLayerOptions, imagery);

  // Add the ImageLayer to the Scene Graph.
  m_osgEarthMap->addImageLayer( maplinkImageLayer );

  progress.setValue(5);

  return true;
}

bool MainWindow::addMapLinkTerrainData( const char* fileName )
{
  QProgressDialog progress("Loading Data...", "Cancel",0, 5, this);
  progress.setWindowModality(Qt::WindowModal);

  progress.show();

  progress.setValue(1);

  // MapLink Pro Tile Source Options
  envitia::MapLink::TerrainTileSourceOptions terrainOptions;
  terrainOptions.tileSize() = TERRAIN_TILESIZE;

  // MapLink Terrain Tile Source
  envitia::MapLink::TerrainTileSource *terrainTileSource = 
    new envitia::MapLink::TerrainTileSource(terrainOptions);

  progress.setValue(2);

  // Maplink Terrain Database
  TSLTerrainDatabase *terrainDB = new TSLTerrainDatabase;
  if( terrainDB->open(fileName) != TSLTerrain_OK )
  {
    return false;
  }

  terrainTileSource->addLayer(terrainDB);

  progress.setValue(3);

  // Initialise the TileSource
  // This should be called after adding datalayers to the TileSource
  // to ensure the coordinate system and extent provided to osgEarth
  // are correct
  terrainTileSource->open();

  // Create an osgEarth TerrainLayer.
  Drivers::TerrainLayerOptions terrainLayerOptions;
  terrainLayerOptions.name() = fileName;

  // Specify the TerrainLayer options
  optional<CachePolicy> &cachePolicy = terrainLayerOptions.cachePolicy();
  cachePolicy = CachePolicy::NO_CACHE;

  progress.setValue(4);

  optional<unsigned int> &maxLevel = terrainLayerOptions.maxLevel();
  maxLevel = TERRAIN_LAYER_MAXLEVEL;

  // Attach the MapLink Terrain TileSource to the TerrainLayer
  ElevationLayer* terrainImageLayer =
    new ElevationLayer(terrainLayerOptions, terrainTileSource);

  // Add the ImageLayer to the Scene Graph.
  m_osgEarthMap->addElevationLayer( terrainImageLayer  );

  progress.setValue(5);

  return true;
}

void MainWindow::openMapLinkData()
{
#ifdef MAPLINK_HAVE_KML
  const char* formats = "Supported Data Formats (*.map *.mpc *.tdf *.kml *.kmz a.toc)";
#else
  const char* formats = "Supported Data Formats (*.map *.mpc *.tdf a.toc)";
#endif


  QString fileName = QFileDialog::getOpenFileName(this, tr("Load MapLink Data"), QString(),
                                                  tr(formats));
  
  if (!fileName.isEmpty())
  {
    std::string strFileName = (const char*)fileName.toUtf8();
    if( strFileName.find_last_of(".") != std::string::npos )
    {
      std::string extension = strFileName.substr( strFileName.find_last_of("."), strFileName.length() );
      std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

      // Determine the data type, and handle accordingly
      bool result = false;
      if( extension == ".tdf" )
        result = addMapLinkTerrainData( strFileName.c_str() );
      else if( extension == ".map" || extension == ".mpc" )
        result = addMapLinkData( strFileName.c_str(), TSLDataLayerTypeMapDataLayer );
#ifdef MAPLINK_HAVE_KML
      else if( extension == ".kml" || extension == ".kmz" )
        result = addMapLinkData( strFileName.c_str(), TSLDataLayerTypeKMLDataLayer );
#endif
      else if( extension == ".toc" )
        result = addMapLinkData( strFileName.c_str(), TSLDataLayerTypeCADRGDataLayer );

      if( !result )
      {
        QString message("Failed to process data: ");
        message += fileName;
        QMessageBox::information( this, "Failed to load data", message );
      }
    }
  }
}

void MainWindow::showAboutBox()
{
  // Display an about box
  QMessageBox::about( this, tr( "MapLink Pro OSGEarth Integration Sample" ),
            tr( "<img src=\":/images/images/envitia.png\"/>"
              "<p>Copyright &copy; 1998-2015 Envitia Group PLC. All rights reserved.</p>"
#ifdef WIN32
              "<p align=center style=\"color:#909090\">Portions developed using LEADTOOLS<br>"
              "Copyright &copy; 1991-2009 LEAD Technologies, Inc. ALL RIGHTS RESERVED</p>"
#endif
              "<p align=center style=\"color:#909090\">Please refer to the 'MapLink Pro 3<sup>rd</sup> Party Licence'<br />"
              "Documents for additional Licences</p>"
            ) );
}

void MainWindow::setSkyBoxMoon( bool enabled )
{
  m_skyNode->setMoonVisible( enabled );
}

void MainWindow::setSkyBoxAnimated( bool enabled )
{
  if( enabled )
  {
    if( !m_skyNode->getUpdateCallback() )
    {
      m_skyNode->setUpdateCallback( new AnimateSunCallback() );
    }
  }
  else
  {
    if( m_skyNode->getUpdateCallback() )
    {
      m_skyNode->setUpdateCallback(NULL);
    }
  }
}

void MainWindow::setLighting( bool enabled )
{
  osg::StateSet* state = m_mapNode->getOrCreateStateSet();
  if( enabled )
  {
    state->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
  }
  else
  {
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
  }
}

void MainWindow::setDecluttering(bool enabled)
{
  m_trackManager->decluttering(enabled);
}

void MainWindow::showSimulationOptions()
{
  m_trackManager->showSimulationOptions();
}

void MainWindow::exit()
{
  close();
}

