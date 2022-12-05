/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "wmsservicedimensionsmodel.h"
#include "wmsservicedimensioninfomodel.h"
#include "wmslayerpreview.h"

#include "wmsservice.h"
#include "wmsservicelayermodel.h"
#include "wmsservicelayerstylesmodel.h"

#include "MapLink.h"
#include "MapLinkDrawing.h"

namespace Services
{

  static const char *g_unnamedLayerString = "[Unnamed Layer]";

  WMSService::WMSServiceLayer::WMSServiceLayer( TSLWMSServiceLayer *layer )
    : m_layer( layer )
  {
    if( layer )
    {
      // Create a lookup between the dimension name and its index for quick access
      // when altering dimension values
      int numLayerDimensions = layer->noOfDimensions();
      for( int i = 0; i < numLayerDimensions; ++i )
      {
        m_dimensionsLookup.insert( std::make_pair( layer->getDimensionAt(i)->name(), i ) );
      }
    }
  }

  WMSService::WMSServiceLayer::~WMSServiceLayer()
  {
  }

  const char* WMSService::WMSServiceLayer::displayName() const
  {
    if( !m_layer )
    {
      return NULL;
    }

    if( m_layer->title() )
    {
      return m_layer->title();
    }
    else if( m_layer->name() )
    {
      return m_layer->name();
    }

    return g_unnamedLayerString;
  }

  bool WMSService::WMSServiceLayer::getTMCExtent( TSLTMC &x1, TSLTMC &y1, TSLTMC &x2, TSLTMC &y2 ) const
  {
    if( !m_layer )
    {
      return false;
    }

    return m_layer->getTMCExtent( x1, y1, x2, y2 );
  }

  void WMSService::WMSServiceLayer::getDimensionNames( std::vector< std::string > &dimensionNames ) const
  {
    if( !m_layer )
    {
      return;
    }

    int numDimensions = m_layer->noOfDimensions();
    dimensionNames.reserve( numDimensions );
    for( int i = 0; i < numDimensions; ++i )
    {
      const TSLWMSServiceLayerDimension *dimension = m_layer->getDimensionAt( i );
      dimensionNames.push_back( dimension->name() );
    }
  }

  const char* WMSService::WMSServiceLayer::getDimensionUnits( const std::string &name ) const
  {
    if( !m_layer )
    {
      return NULL;
    }

    std::map< std::string, int >::const_iterator dimensionIndexIt( m_dimensionsLookup.find( name ) );
    if( dimensionIndexIt == m_dimensionsLookup.end() )
    {
      return NULL;
    }

    const TSLWMSServiceLayerDimension *dimension = m_layer->getDimensionAt( dimensionIndexIt->second );
    return dimension->units();
  }

  void WMSService::WMSServiceLayer::getStyleNames( std::vector< std::string > &styleNames ) const
  {
    if( !m_layer )
    {
      return;
    }

    const char *currentStyle = m_layer->getStyleValue();

    int numStyles = m_layer->noOfStyles();
    styleNames.reserve( numStyles );
    for( int i = 0; i < numStyles; ++i )
    {
      // Don't include the current style in the list as this method is used to provide a list
      // of styles that the layer could be changed to show after the initial connection.
      const char *availableStyle = m_layer->getStyleAt( i );
      if( !currentStyle || strcmp( currentStyle, availableStyle ) != 0 )
      {
        styleNames.push_back( availableStyle );
      }
    }
  }

  bool WMSService::WMSServiceLayer::setStyle( const char *styleName )
  {
    if( !m_layer )
    {
      return false;
    }

    return m_layer->setStyleValue( styleName );
  }

  WMSService::WMSService()
    : Service()
      , m_serviceRootLayer( NULL )
      , m_crsChoices( NULL )
      , m_numCRSChoices( 0 )
      , m_coordinateSystemChoice( -1 )
  {
    m_type = ServiceTypeWMS;

    m_dataLayer = new TSLWMSDataLayer( this );
    m_dataLayer->cacheSize( 128 * 1024 );

    // In order to keep the UI responsive we want all connections to occur in a background thread
    // instead of the UI thread
    m_dataLayer->setSynchronousLoading( false );
    m_dataLayer->synchronousLoadStrategy( false );
    m_dataLayer->validateGetMapRequest( true );
  }

  WMSService::~WMSService()
  {
    if( m_dataLayer )
    {
      m_dataLayer->destroy();
    }
  }

  void WMSService::loadService( const char *address )
  {
    // Tell the data layer about any fixed linear transform parameters to use. If more than one service
    // is loaded, this ensures that each data layer will appear in the correct location relative to the
    // other layers.
    m_dataLayer->setLinearTransformParameters( !m_useFixedTransformParameters, m_muShiftX, m_muShiftY, m_tmcPerMU );

    // Start loading the service metadata in a background thread.
    m_url = address;
    m_loadCancelled = false;
    m_dataLayer->loadData( address );
  }

  Service::ServiceLayerModel* WMSService::getServiceLayerModel()
  {
    return new WMSServiceLayerModel( this, m_serviceRootLayer ? m_serviceRootLayer : m_dataLayer->rootServiceLayer() );
  }

  Service::ServiceDimensionsModel* WMSService::getDimensionsModel()
  {
    return new WMSServiceDimensionsModel( m_serviceRootLayer ? m_serviceRootLayer : m_dataLayer->rootServiceLayer() );
  }

  Service::ServiceDimensionInfoModel* WMSService::getDimensionInfoModel()
  {
    return new WMSServiceDimensionInfoModel();
  }

  Service::ServiceLayerStylesModel* WMSService::getServiceLayerStyleModel()
  {
    return new WMSServiceLayerStylesModel();
  }

  ServiceLayerPreview* WMSService::getLayerPreviewHelper()
  {
    return new WMSLayerPreview( m_url );
  }

  const char* WMSService::getServiceDisplayName() const
  {
    const TSLWMSServiceInfo *serviceInfo = m_dataLayer->serviceInformation();
    if( !serviceInfo )
    {
      return NULL;
    }

    return serviceInfo->title() ? serviceInfo->title() : serviceInfo->name();
  }

  void WMSService::getVisibleLayers( std::vector< Service::ServiceLayer* > &layers )
  {
    if( m_sortedLayerVisibility.empty() && m_dataLayer && m_dataLayer->rootServiceLayer() )
    {
      // Build the list of visible layers in the order they are drawn on first use
      buildSortedLayerVisibilityList( m_dataLayer->rootServiceLayer() );
    }

    layers.reserve( m_sortedLayerVisibility.size() );

    std::map< int, TSLWMSServiceLayer* >::iterator layerIt( m_sortedLayerVisibility.begin() );
    std::map< int, TSLWMSServiceLayer* >::iterator layerItE( m_sortedLayerVisibility.end() );
    for( ; layerIt != layerItE; ++layerIt )
    {
      layers.push_back( new WMSServiceLayer( layerIt->second ) );
    }
  }

  void WMSService::getPotentialLayersForDisplay( std::vector< std::string > &layerNames )
  {
    layerNames.reserve( m_invisibleLayers.size() );

    std::set< const TSLWMSServiceLayer* >::iterator layerIt( m_invisibleLayers.begin() );
    std::set< const TSLWMSServiceLayer* >::iterator layerItE( m_invisibleLayers.end() );
    for( ; layerIt != layerItE; ++layerIt )
    {
      const TSLWMSServiceLayer *invisibleLayer = *layerIt;
      if( invisibleLayer->supportsCRS( m_activeCRS.c_str() ) )
      {
        if( invisibleLayer->title() )
        {
          layerNames.push_back( invisibleLayer->title() );
        }
        else if( invisibleLayer->name() )
        {
          layerNames.push_back( invisibleLayer->name() );
        }
      }

      // If the layer has neither a title nor name, don't include it since it can't be presented sensibly
    }
  }

  Service::ServiceLayer* WMSService::getLayerForToken( void* token )
  {
    return new WMSServiceLayer( (TSLWMSServiceLayer*)token );
  }

  TSLDataLayer* WMSService::dataLayer()
  {
    return m_dataLayer;
  }

  size_t WMSService::numCoordSystemChoices()
  {
    // This function will only return non-zero when the callback thread is waiting
    // inside onChoiceOfServiceCRSs
    return m_numCRSChoices;
  }

  const char** WMSService::coodinateSystemChoices()
  {
    // This function will only return non-NULL when the callback thread is waiting
    // inside onChoiceOfServiceCRSs
    return m_crsChoices;
  }

  void WMSService::setCoordinateSystemChoice( int choice )
  {
    m_mutex.lock();
    m_coordinateSystemChoice = choice;
    m_mutex.unlock();
  }

  Service::ServiceLayerInfoModel* WMSService::getServiceLayerInfoModel()
  {
    return new WMSServiceLayerInfoModel();
  }

  void WMSService::updateLayerVisibilityOrder( int originalIndex, int newIndex )
  {
    std::map< int, TSLWMSServiceLayer* >::iterator changedLayerIt( m_sortedLayerVisibility.find( originalIndex ) );
    if( changedLayerIt != m_sortedLayerVisibility.end() )
    {
      changedLayerIt->second->setVisibility( true, newIndex );

      // Rebuild the layer visibility index since the order has now changed
      m_sortedLayerVisibility.clear();
      m_invisibleLayers.clear();
      buildSortedLayerVisibilityList( m_dataLayer->rootServiceLayer() );
      m_dataLayer->clearCache();

      // Since the layer has now been modified, set the changed flag so it will be updated next draw
      m_dataLayer->notifyChanged();
    }
  }

  void* WMSService::setLayerVisibility( const char *layerName, bool visible )
  {
    TSLWMSServiceLayer *matchedLayer = findLayer( m_dataLayer->rootServiceLayer(), layerName );
    if( !matchedLayer )
    {
      return NULL;
    }

    // Change the layer visibility as requested
    matchedLayer->setVisibility( visible );

    m_sortedLayerVisibility.clear();
    m_invisibleLayers.clear();
    buildSortedLayerVisibilityList( m_dataLayer->rootServiceLayer() );
    m_dataLayer->clearCache();

    // Since the layer has now been modified, set the changed flag so it will be updated next draw
    m_dataLayer->notifyChanged();

    return matchedLayer;
  }

  void WMSService::setLayerVisibility( ServiceLayer *layer, bool visible )
  {
    WMSServiceLayer *wmsLayer = reinterpret_cast< WMSServiceLayer* >( layer );
    wmsLayer->layer()->setVisibility( visible );

    m_sortedLayerVisibility.clear();
    m_invisibleLayers.clear();
    buildSortedLayerVisibilityList( m_dataLayer->rootServiceLayer() );
    m_dataLayer->clearCache();

    // Since the layer has now been modified, set the changed flag so it will be updated next draw
    m_dataLayer->notifyChanged();
  }

  void WMSService::setCacheSize( int size )
  {
    // We are given the size in MB, the data layer takes the size in Kb.
    m_dataLayer->cacheSize( size * 1024 );
  }

  void WMSService::layerVisiblityChanged( TSLWMSServiceLayer *layer )
  {
    bool derived = false;
    if( layer->getVisibility( &derived ) && !derived )
    {
      m_visibleLayers.insert( layer );
    }
    else
    {
      m_visibleLayers.erase( layer );
    }
  }

  bool WMSService::onCapabilitiesLoaded (TSLWMSServiceLayer *rootLayerInfo)
  {
    if( !m_loadCancelled )
    {
      m_mutex.lock();
      m_serviceRootLayer = rootLayerInfo;

      if( !m_callbacks.empty() )
      {
        m_callbacks.top()->onNextSequenceAction();
      }

      m_waitCond.wait( &m_mutex );

      m_serviceRootLayer = NULL;
      m_visibleLayers.clear();
      m_mutex.unlock();
      return true;
    }
    else
    {
      if( !m_callbacks.empty() )
      {
        m_callbacks.top()->onNextSequenceAction();
      }
    }

    return false;
  }

  void WMSService::onCapabilitiesLoadFailure (TSLWMSServiceSettingsCallbacks::CapabilitiesLoadFailureReason reason)
  {
    if( !m_callbacks.empty() )
    {
      TSLSimpleString errorMessage;
      TSLThreadedErrorStack::errorString( errorMessage );

      std::string fullMessage;
      if( !errorMessage.empty() )
      {
        fullMessage = errorMessage;
      }
      else
      {
        switch( reason )
        {
          case LoadingFailedLoadFailed:
            fullMessage = "Failed to load service, is the service endpoint valid?";
            break;

          case LoadingFailedParseFailure:
            fullMessage = "Failed to parse service capabilities, is the service endpoint valid?";
            break;

          case LoadingFailedServiceUnsupported:
            fullMessage = "This service is not supported by this viewer. Please report this to <a href=\"mailto:support@envitia.com?Subject=Unsupported WMS with WMS/WMTS viewer\">Envitia support</a>";
            break;

          case LoadingFailedUnableToDetermineExtent:
            fullMessage = "Cannot determine the extent of any layers in the service. Is the service endpoint valid?";
            break;

          case LoadingFailedToFitInTMCSpace:
            fullMessage = "Internal error.";
            break;

          default:
            fullMessage = "Unknown error.";
            break;
        }
      }
      m_callbacks.top()->onError( fullMessage );

      // Clear the error stack to avoid reporting the same error multiple times
      TSLThreadedErrorStack::clear();
    }
  }

  bool WMSService::onNoVisibleLayers (TSLWMSServiceLayer* /*rootLayer*/)
  {
    if( !m_callbacks.empty() && !m_loadCancelled )
    {
      m_callbacks.top()->onError( "Internal error: No layers are visible." );
    }
    return false;
  }

  bool WMSService::onRequiredDimensionValueNotSet( TSLWMSServiceLayer* /*rootLayerInfo*/, TSLWMSServiceLayer* /*dimensionLayer*/,
      const char* /*dimensionName*/)
  {
    if( !m_callbacks.empty() )
    {
      m_callbacks.top()->onError( "Internal error: A required dimension value was not set." );
    }
    return false;
  }

  bool WMSService::onNoCommonCRSinVisibleLayers (TSLWMSServiceLayer* /*rootLayer*/)
  {
    if( !m_callbacks.empty() )
    {
      m_callbacks.top()->onError( "Internal error: Selected layers have incompatible coordinate systems." );
    }
    return false;
  }

  bool WMSService::onNoSupportedCRSinVisibleLayers (TSLWMSServiceLayer* /*rootLayer*/)
  {
    if( !m_callbacks.empty() )
    {
      m_callbacks.top()->onError( "Internal error: Unsupported coordinate system encountered." );
    }
    return false;
  }

  int WMSService::onChoiceOfRequestFormats (const char **formatChoices, int noOfFormatChoices)
  {
    for( int i = 0; i < noOfFormatChoices; ++i )
    {
      if( strcmp( formatChoices[i], "image/png" ) == 0 )
      {
        return i;
      }
    }
    return 0;
  }

  int WMSService::onChoiceOfServiceCRSs (const char **crsChoices, int noOfCRSChoices)
  {
    m_mutex.lock();
    m_crsChoices = crsChoices;
    m_numCRSChoices = noOfCRSChoices;

    if( !m_callbacks.empty() )
    {
      m_callbacks.top()->coordinateSystemChoiceRequired();
    }

    m_waitCond.wait( &m_mutex );

    m_crsChoices = NULL;
    m_numCRSChoices = 0;
    m_mutex.unlock();

    return m_coordinateSystemChoice;
  }

  bool WMSService::onUserLinearTransformInvalid()
  {
    if( !m_callbacks.empty() )
    {
      m_callbacks.top()->onError( "Internal error: Insufficient coordinate precision to display. If more than one service is loading, try adding this service first." );
    }

    return false;
  }

  bool WMSService::onServiceSettingsComplete()
  {
    m_activeCRS = m_dataLayer->activeCRS();

    if( !m_callbacks.empty() )
    {
      m_callbacks.top()->onNextSequenceAction();
    }

    return !m_loadCancelled;
  }

  void WMSService::buildSortedLayerVisibilityList( TSLWMSServiceLayer *layer )
  {
    bool derivedVisibility = false;
    if( layer->getVisibility( &derivedVisibility ) && !derivedVisibility )
    {
      m_sortedLayerVisibility.insert( std::make_pair( layer->getVisibilityOrderIndex(), layer ) );
    }
    else
    {
      m_invisibleLayers.insert( layer );
    }

    int numChildLayers = layer->noOfSubLayers();
    for( int i = 0; i < numChildLayers; ++i )
    {
      buildSortedLayerVisibilityList( layer->getSubLayerAt(i) );
    }
  }

  TSLWMSServiceLayer* WMSService::findLayer( TSLWMSServiceLayer *layer, const char *name )
  {
    // The name we have might be either the layer's name or title, so check both to see if the
    // layer matches
    if( layer->title() && strcmp( layer->title(), name ) == 0 )
    {
      return layer;
    }
    else if( layer->name() && strcmp( layer->name(), name ) == 0 )
    {
      return layer;
    }

    // This layer didn't match, check the child layers for a match
    int numChildLayers = layer->noOfSubLayers();
    for( int i = 0; i < numChildLayers; ++i )
    {
      TSLWMSServiceLayer *matchedLayer = findLayer( layer->getSubLayerAt(i), name );
      if( matchedLayer )
      {
        return matchedLayer;
      }
    }

    return NULL;
  }

};
