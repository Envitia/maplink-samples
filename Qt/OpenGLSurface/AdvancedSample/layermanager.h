/****************************************************************************
Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/
#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include "layers/decluttermodel.h"
#include <time.h>

#ifdef HAVE_DIRECT_IMPORT_SDK
# include "MapLinkDirectImport.h"
# include "featureconfiguration/tslfeatureclassconfig.h"
#endif

#include "tslkmldatalayer.h"
#include "tsldatalayertypeenum.h"
#include "datalayers/tslopenglsinglethreadcache.h"

#include <QMutex>
#include <QWaitCondition>

#include <map>

#ifdef None
# undef None
#endif

// TSLFeatureID's have to > 0.
#define POLYGON_FC 10000
#define VSYMBOL_FC 10001
#define RSYMBOL_FC 10002
#define POLYLINE_FC 10003
#define TEXT_FC 10004
#define ARC_FC 10005
#define ELLIPSE_FC 10006


class TSLMapDataLayer;
class TSLStaticMapDataLayer;
class TSLKMLDataLayer;
class TSLRasterDataLayer;
class TSLFileLoader;
class TSLCustomDataLayer;
class TSLOpenGLSurface;
class TSLDataLayer;
class TSLStandardDataLayer;
class TSLFeatureClassConfig;

class TSLCoordinateSystem;
class TrackCustomDataLayer;
class FPSLayer;
class AddWaypointInteractionMode;
class MapLinkWidget;
class TSLFeatureList;
class ScaleBandsTable;
class TrackLayer;

typedef std::map< std::string, std::pair< TSLDataLayer*, TSLDataLayerTypeEnum > > SurfaceMap;

// We would use a unique_ptr to track the TSLOpenGLSingleThreadCache class
// however this is not available with all compilers we are targeting.
template <class T>
class envitia_ptr {
  T *m_object;
public:
  envitia_ptr(T *obj) : m_object(obj) { }
  ~envitia_ptr() {
     if (m_object)
       m_object->destroy();
  }
  T* operator->() {
    return m_object;
  }
};

// The drawing surface and various layers used by the application are 
// encapsulated in this class.
#ifdef HAVE_DIRECT_IMPORT_SDK
class LayerManager : public TSLDirectImportDataLayerAnalysisCallbacks 
#else
class LayerManager 
#endif
{

public:
  // Available types of projection
  enum ProjectionType
  {
    /* Supported Vector and Raster Realtime reprojection Projections */
    Stereographic,
    GnomicSphericalEarth,
    TransverseMercatorUSGS,
    Mercator,
    /* not supported at present */
    Orthographic,
    ObliqueCylindricalEqualAreaPoint,
    LambertAzimuthal,
    AlbersEqualArea,
    Bonne,
    Cassini,
    None
  };

  LayerManager();
  ~LayerManager();

  void configureDrawingSurface( TSLOpenGLSurface *surface, AddWaypointInteractionMode *waypointMode );
  void configureDataLayer( TSLOpenGLSurface* surface, const std::string& name );
  void configureEntityLayer( TSLOpenGLSurface* surface );
  void configureTracksLayer( TSLOpenGLSurface* surface );
  bool projectionMapsLoaded() const;
  bool loadMap( const char *path, TSLOpenGLSurface* surface, bool useSharedCache );
  bool loadLayer( const std::string& path, const std::string& name, bool useSharedCache, int cacheSize = 0 );
  bool loadRaster( const char *path );
  void updateLayers();
  void setTimeAccelerationFactor( double factor );
  bool setProjection( ProjectionType type, TSLOpenGLSurface *surface );
  void setCurrentProjection( ProjectionType type );
  ProjectionType getCurrentProjection();
  void lockProjectionOrigin( bool lock );
  int getLayerCount( TSLDataLayerTypeEnum name );
  bool backgroundMapLoaded() const;

  void setFramerateLayerVisibility( TSLOpenGLSurface *surface, bool isVisible );
  void resetLayers( TSLOpenGLSurface *surface );

  void incrementLayerCount( TSLDataLayerTypeEnum name );
  void removeLayer( const std::string& name );
  void setupRasterSymbol();
  TSLCoordinateSystem* getOutputCoordinateSystem() const;
  TSLDataLayer* getLayer( const std::string& name );

  void editCacheSize( const std::string& layerName, int cacheVal );
  int getCacheSize( const std::string& layerName ) const;

  DeclutterModel& declutterModel();

  // Sets the given data layer to use the shared cache
  void useSharedCache(TSLDataLayer& dataLayer);
  void setUsedCacheSize(int cacheSize);

#ifdef HAVE_DIRECT_IMPORT_SDK
  TSLDirectImportDataLayer* createDirectImportLayer( const std::string& name, int cacheSize, int numProcessingThreads,
                                                     const std::string& diskCacheDir, bool diskCacheFlushOnExit, int diskCacheSize,
                                                     const TSLCoordinateSystem* cs, TSLDirectImportDataLayerCallbacks* layerCallbacks );
  bool loadDataSetIntoDirectImportLayer( TSLDirectImportDataSet* dataSet, TSLDirectImportScaleBand* band = NULL, TSLFeatureClassConfig* featureConfig = NULL );

  TSLFeatureClassConfig* getDefaultFeatureConfig(const TSLDirectImportDataSet* dataSet);

  const std::string& getDirectImportName() const;
  TSLDirectImportDataLayer* directImportLayer();
  void directImportLayer( TSLDirectImportDataLayer* layer, const std::string& layerName );

  void moveDirectImportDataSetToIndex( const char* layerName, const char* scaleBandName, int rowFrom, int rowTo );
  void removeDirectImportDataSet( const char* layerName, const char* scaleBandName, int row );
  TSLDirectImportDataSet* getDirectImportDataSet( const char* layerName, const char* scaleBandName, int row );

  // TSLDirectImportDataLayerAnalysisCallbacks
  virtual void onAnalysisStarted( const TSLDirectImportDataSet* dataSet );
  virtual void onAnalysisCancelled( const TSLDirectImportDataSet* dataSet );
  virtual void onAnalysisFailed( const TSLDirectImportDataSet* dataSet );
  virtual void onAnalysisComplete( const TSLDirectImportDataSet* dataSet, const TSLFeatureList* featureList );
#endif

private:


  TSLOpenGLSurface *m_surface;

  TSLStaticMapDataLayer* m_mapLayer;
  TSLStaticMapDataLayer* m_mapLayer1;
  TSLStaticMapDataLayer* m_mapLayer2;

  TSLFileLoader* m_mapLayerLoader;
  TSLFileLoader* m_mapLayerLoader1;
  TSLFileLoader* m_mapLayerLoader2;

  bool m_loadedBackground;
  bool m_loadedBackground2;

  SurfaceMap m_dataLayer;
  std::map< TSLDataLayerTypeEnum, int > m_layerCount;

  TSLRasterDataLayer* m_rasterLayer;
  TSLStandardDataLayer* m_entityLayer;

  TSLCustomDataLayer* m_trackLayer;
  TrackCustomDataLayer* m_trackClient;

  TSLCoordinateSystem* m_currentCoordinateSystem;

  TSLCustomDataLayer* m_fpsLayer;
  FPSLayer* m_fpsClient;

  ProjectionType m_currentProjection;

  bool m_lockProjectionCentre;
  double m_timeMultiplier; // Multiplier to apply to elapsed time for apparent simulation speedups
  double m_greatCircleDistance;

  TSLCustomDataLayer *m_framerateCL;

  // A custom data layer that displays the tracks from the track manager
  TrackLayer *m_tracksLayer;
  TSLCustomDataLayer *m_trackCL;
  std::string m_tracksLayerName;

  // Qt model implementation for mapping layer features into a tree view
  DeclutterModel m_declutterModel;

  // Single threaded cache that can be used by more than one layer
  // We would normally use a unique_ptr however we are targeting many
  // different compilers and as such unique_ptr may not be available.
  //   For Reference: std::unique_ptr<TSLOpenGLSingleThreadCache, void(*)(TSLOpenGLSingleThreadCache*)> m_sharedCache;
  envitia_ptr<TSLOpenGLSingleThreadCache> m_sharedCache;

  // Timing information for frame updates
  //

#ifdef _MSC_VER
  clock_t m_lastFrameTime;
#else
  double m_lastFrameTime;
#endif

#ifdef HAVE_DIRECT_IMPORT_SDK
  //! This method will setup a feature configuration for the direct import layer.
  //! 
  //! This will set basic rendering attributes for all features in the data
  //!
  //! A feature configuration may also be specified as an SLD through the direct import wizard
  void setupFeatureConfig( const std::string& filename, TSLDirectImportDataLayer* layer );

  //! Used during onAnalysisComplete in order to receive feature information from the direct import layer
  TSLFeatureClassConfig m_featureClassConfig;
  QWaitCondition m_featureClassConfigWaitCondition;
  QMutex m_featureClassConfigMutex;
  std::string m_directImportName;
  bool m_directImportAnalysisComplete;
#endif

};


inline DeclutterModel& LayerManager::declutterModel()
{
  return m_declutterModel;
}

inline bool LayerManager::backgroundMapLoaded() const
{
  return m_loadedBackground;
}

#endif
