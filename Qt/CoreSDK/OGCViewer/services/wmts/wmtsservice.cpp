/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "wmtsservicedimensionsmodel.h"


#include "wmtsservice.h"
#include "wmtsservicelayermodel.h"
#include "wmtsservicelayerstylesmodel.h"
#include "wmtsservicedimensioninfomodel.h"
#include "wmtslayerpreview.h"

namespace Services
{

  static const char *g_unnamedLayerString = "[Unnamed Layer]";

  WMTSService::WMTSServiceLayer::WMTSServiceLayer( TSLWMTSServiceLayer *layer )
    : m_layer( layer )
  {
    if( layer )
    {
      // Create a lookup between the dimension name and its index for quick access
      // when altering dimension values
      int numLayerDimensions = layer->numDimensions();
      for( int i = 0; i < numLayerDimensions; ++i )
      {
        m_dimensionsLookup.insert( std::make_pair( layer->getDimensionAt(i)->identifier(), i ) );
      }
    }
  }

  WMTSService::WMTSServiceLayer::~WMTSServiceLayer()
  {
  }

  const char* WMTSService::WMTSServiceLayer::displayName() const
  {
    if( !m_layer )
    {
      return NULL;
    }

    if( m_layer->identifier() )
    {
      return m_layer->identifier();
    }

    return g_unnamedLayerString;
  }

  bool WMTSService::WMTSServiceLayer::getTMCExtent( TSLTMC &x1, TSLTMC &y1, TSLTMC &x2, TSLTMC &y2 ) const
  {
    if( !m_layer )
    {
      return false;
    }

    return m_layer->getTMCExtent( x1, y1, x2, y2 );
  }

  void WMTSService::WMTSServiceLayer::getDimensionNames( std::vector< std::string > &dimensionNames ) const
  {
    if( !m_layer )
    {
      return;
    }

    int numDimensions = m_layer->numDimensions();
    dimensionNames.reserve( numDimensions );
    for( int i = 0; i < numDimensions; ++i )
    {
      const TSLWMTSServiceDimension *dimension = m_layer->getDimensionAt( i );
      dimensionNames.push_back( dimension->identifier() );
    }
  }

  const char* WMTSService::WMTSServiceLayer::getDimensionUnits( const std::string &name ) const
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

    const TSLWMTSServiceDimension *dimension = m_layer->getDimensionAt( dimensionIndexIt->second );
    return dimension->uom();
  }

  void WMTSService::WMTSServiceLayer::getStyleNames( std::vector< std::string > &styleNames ) const
  {
    if( !m_layer )
    {
      return;
    }

    const char *currentStyle = m_layer->getStyleValue();

    int numStyles = m_layer->numStyles();
    styleNames.reserve( numStyles );
    for( int i = 0; i < numStyles; ++i )
    {
      // Don't include the current style in the list as this method is used to provide a list
      // of styles that the layer could be changed to show after the initial connection.
      const TSLWMTSServiceStyle *availableStyle = m_layer->getStyleAt( i );
      if( !currentStyle || strcmp( currentStyle, availableStyle->identifier() ) != 0 )
      {
        styleNames.push_back( availableStyle->identifier() );
      }
    }
  }

  bool WMTSService::WMTSServiceLayer::setStyle( const char *styleName )
  {
    if( !m_layer )
    {
      return false;
    }

    return m_layer->setStyleValue( styleName );
  }

  WMTSService::WMTSService()
    : Service()
      , m_serviceInfo( NULL )
      , m_crsChoices( NULL )
      , m_numCRSChoices( 0 )
      , m_coordinateSystemChoice( -1 )
  {
    m_type = ServiceTypeWMTS;

    m_dataLayer = new TSLWMTSDataLayer( this );
    m_dataLayer->cacheSize( 128 * 1024 );

    // In order to keep the UI responsive we want all connections to occur in a background thread
    // instead of the UI thread
    m_dataLayer->setSynchronousLoading( false );
    m_dataLayer->synchronousLoadStrategy( false );
  }

  WMTSService::~WMTSService()
  {
    if( m_dataLayer )
    {
      m_dataLayer->destroy();
    }
  }

  void WMTSService::loadService( const char *address )
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

  Service::ServiceLayerModel* WMTSService::getServiceLayerModel()
  {
    return new WMTSServiceLayerModel( this, m_serviceInfo ? m_serviceInfo : m_dataLayer->serviceInformation() );
  }

  Service::ServiceDimensionsModel* WMTSService::getDimensionsModel()
  {
    return new WMTSServiceDimensionsModel( m_serviceInfo ? m_serviceInfo : m_dataLayer->serviceInformation() );
  }

  Service::ServiceDimensionInfoModel* WMTSService::getDimensionInfoModel()
  {
    return new WMTSServiceDimensionInfoModel();
  }

  Service::ServiceLayerStylesModel* WMTSService::getServiceLayerStyleModel()
  {
    return new WMTSServiceLayerStylesModel();
  }

  ServiceLayerPreview* WMTSService::getLayerPreviewHelper()
  {
    return new WMTSLayerPreview( m_url );
  }

  const char* WMTSService::getServiceDisplayName() const
  {
    const TSLWMTSServiceInfo *serviceInfo = m_dataLayer->serviceInformation();
    if( !serviceInfo )
    {
      return NULL;
    }

    return serviceInfo->title();
  }

  void WMTSService::getVisibleLayers( std::vector< Service::ServiceLayer* > &layers )
  {
    if( m_sortedLayerVisibility.empty() && m_dataLayer && m_dataLayer->serviceInformation() )
    {
      // Build the list of visible layers in the order they are drawn on first use
      buildSortedLayerVisibilityList( m_dataLayer->serviceInformation() );
    }

    layers.reserve( m_sortedLayerVisibility.size() );

    std::map< int, TSLWMTSServiceLayer* >::iterator layerIt( m_sortedLayerVisibility.begin() );
    std::map< int, TSLWMTSServiceLayer* >::iterator layerItE( m_sortedLayerVisibility.end() );
    for( ; layerIt != layerItE; ++layerIt )
    {
      layers.push_back( new WMTSServiceLayer( layerIt->second ) );
    }
  }

  void WMTSService::getPotentialLayersForDisplay( std::vector< std::string > &layerNames )
  {
    layerNames.reserve( m_invisibleLayers.size() );

    std::set< TSLWMTSServiceLayer* >::iterator layerIt( m_invisibleLayers.begin() );
    std::set< TSLWMTSServiceLayer* >::iterator layerItE( m_invisibleLayers.end() );

    for( ; layerIt != layerItE; ++layerIt )
    {
      const TSLWMTSServiceLayer *invisibleLayer = *layerIt;
      if( invisibleLayer->identifier() && invisibleLayer->supportsCRS( m_dataLayer->activeCRS() ) )
      {
        layerNames.push_back( invisibleLayer->identifier() );
      }

      // If the layer doesn't support the current coordinate system or has no identifier, don't include it
      // as it can't be displayed
    }
  }

  Service::ServiceLayer* WMTSService::getLayerForToken( void* token )
  {
    return new WMTSServiceLayer( (TSLWMTSServiceLayer*)token );
  }

  TSLDataLayer* WMTSService::dataLayer()
  {
    return m_dataLayer;
  }

  size_t WMTSService::numCoordSystemChoices()
  {
    // This function will only return non-zero when the callback thread is waiting
    // inside onChoiceOfServiceCRSs
    return m_numCRSChoices;
  }

  const char** WMTSService::coodinateSystemChoices()
  {
    // This function will only return non-NULL when the callback thread is waiting
    // inside onChoiceOfServiceCRSs
    return m_crsChoices;
  }

  void WMTSService::setCoordinateSystemChoice( int choice )
  {
    m_mutex.lock();
    m_coordinateSystemChoice = choice;
    m_mutex.unlock();
  }

  Service::ServiceLayerInfoModel* WMTSService::getServiceLayerInfoModel()
  {
    return new WMTSServiceLayerInfoModel();
  }

  void WMTSService::updateLayerVisibilityOrder( int originalIndex, int newIndex )
  {
    std::map< int, TSLWMTSServiceLayer* >::iterator changedLayerIt( m_sortedLayerVisibility.find( originalIndex ) );
    if( changedLayerIt != m_sortedLayerVisibility.end() )
    {
      changedLayerIt->second->setVisibility( true, newIndex );

      // Rebuild the layer visibility index since the order has now changed
      m_sortedLayerVisibility.clear();
      m_invisibleLayers.clear();
      buildSortedLayerVisibilityList( m_dataLayer->serviceInformation() );

      // It is not necessary to clear the tile cache for the WMTS data layer as each layer within the service
      // is held independently

      // Since the layer has now been modified, set the changed flag so it will be updated next draw
      m_dataLayer->notifyChanged();
    }
  }

  void* WMTSService::setLayerVisibility( const char *layerName, bool visible )
  {
    TSLWMTSServiceLayer *matchedLayer = findLayer( m_dataLayer->serviceInformation(), layerName );
    if( !matchedLayer )
    {
      return NULL;
    }

    // Change the layer visibility as requested
    matchedLayer->setVisibility( visible );

    m_sortedLayerVisibility.clear();
    m_invisibleLayers.clear();
    buildSortedLayerVisibilityList( m_dataLayer->serviceInformation() );

    // It is not necessary to clear the tile cache for the WMTS data layer as each layer within the service
    // is held independently

    // Since the layer has now been modified, set the changed flag so it will be updated next draw
    m_dataLayer->notifyChanged();

    return (void*)matchedLayer;
  }

  void WMTSService::setLayerVisibility( ServiceLayer *layer, bool visible )
  {
    WMTSServiceLayer *wmtsLayer = reinterpret_cast< WMTSServiceLayer* >( layer );
    wmtsLayer->layer()->setVisibility( visible );

    m_sortedLayerVisibility.clear();
    m_invisibleLayers.clear();
    buildSortedLayerVisibilityList( m_dataLayer->serviceInformation() );

    // It is not necessary to clear the tile cache for the WMTS data layer as each layer within the service
    // is held independently

    // Since the layer has now been modified, set the changed flag so it will be updated next draw
    m_dataLayer->notifyChanged();
  }

  void WMTSService::setCacheSize( int size )
  {
    // We are given the size in MB, the data layer takes the size in Kb.
    m_dataLayer->cacheSize( size * 1024 );
  }


  void WMTSService::layerVisiblityChanged( TSLWMTSServiceLayer *layer )
  {
    if( layer->getVisibility() )
    {
      m_visibleLayers.insert( layer );
    }
    else
    {
      m_visibleLayers.erase( layer );
    }
  }

  bool WMTSService::onCapabilitiesLoaded(TSLWMTSServiceInfo *serviceInfo)
  {
    if( !m_loadCancelled )
    {
      m_mutex.lock();
      m_serviceInfo = serviceInfo;

      if( !m_callbacks.empty() )
      {
        m_callbacks.top()->onNextSequenceAction();
      }

      m_waitCond.wait( &m_mutex );

      m_serviceInfo = NULL;
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

  void WMTSService::onCapabilitiesLoadFailure (TSLWMTSServiceSettingsCallbacks::CapabilitiesLoadFailureReason reason)
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
            fullMessage = "This service is not supported by this viewer. Please report this to <a href=\"mailto:support@envitia.com?Subject=Unsupported WMTS with WMS/WMTS viewer\">Envitia support</a>";
            break;

          case LoadingFailedUnableToDetermineExtent:
            fullMessage = "Cannot determine the extent of any layers in the service. Is the service endpoint valid?";
            break;

          default:
            fullMessage = "Unknown error";
            break;
        }
      }
      m_callbacks.top()->onError( fullMessage );

      // Clear the error stack to avoid reporting the same error multiple times
      TSLThreadedErrorStack::clear();
    }
  }

  bool WMTSService::onNoVisibleLayers (TSLWMTSServiceInfo* /*rootLayer*/)
  {
    if( !m_callbacks.empty() && !m_loadCancelled )
    {
      m_callbacks.top()->onError( "Configuration error: No layers are visible" );
    }
    return false;
  }

  int WMTSService::onChoiceOfServiceCRSs (const char** crsChoices, int noOfChoices)
  {
    m_mutex.lock();
    m_crsChoices = crsChoices;
    m_numCRSChoices = noOfChoices;

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

  bool WMTSService::onNoCommonCRSinVisibleLayers (TSLWMTSServiceInfo* /*serviceInfo*/)
  {
    if( !m_callbacks.empty() )
    {
      m_callbacks.top()->onError( "Internal error: No common supported coordinate system for the selected layers." );
    }
    return false;
  }

  bool WMTSService::onSelectedCRSNotSupported (TSLWMTSServiceInfo* /*serviceInfo*/)
  {
    if( !m_callbacks.empty() )
    {
      m_callbacks.top()->onError( "Internal error: Unsupported coordinate system encountered" );
    }
    return false;
  }

  int WMTSService::onChoiceOfRequestFormats(TSLWMTSServiceLayer* /*layer*/, const char **formatChoices, int noOfFormatChoices)
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

  bool WMTSService::onUserLinearTransformInvalid()
  {
    if( !m_callbacks.empty() )
    {
      m_callbacks.top()->onError( "Internal error: Insufficient coordinate precision to display. If more than one service is loading, try adding this service first." );
    }

    return false;
  }

  bool WMTSService::onServiceSettingsComplete()
  {
    m_activeCRS = m_dataLayer->activeCRS();

    if( !m_callbacks.empty() )
    {
      m_callbacks.top()->onNextSequenceAction();
    }

    return !m_loadCancelled;
  }

  void WMTSService::buildSortedLayerVisibilityList( TSLWMTSServiceInfo *serviceInfo )
  {
    int numLayers = serviceInfo->numLayers();
    for( int i = 0; i < numLayers; ++i )
    {
      TSLWMTSServiceLayer *layer = serviceInfo->getLayerAt(i);
      if( layer->getVisibility() )
      {
        m_sortedLayerVisibility.insert( std::make_pair( layer->getVisibilityOrderIndex(), layer ) );
      }
      else
      {
        m_invisibleLayers.insert( layer );
      }
    }
  }

  TSLWMTSServiceLayer* WMTSService::findLayer( TSLWMTSServiceInfo *serviceInfo, const char *name )
  {
    int numLayers = serviceInfo->numLayers();
    for( int i = 0; i < numLayers; ++i )
    {
      TSLWMTSServiceLayer *layer = serviceInfo->getLayerAt(i);
      if( strcmp( layer->identifier(), name ) == 0 )
      {
        return layer;
      }
    }

    return NULL;
  }
};
