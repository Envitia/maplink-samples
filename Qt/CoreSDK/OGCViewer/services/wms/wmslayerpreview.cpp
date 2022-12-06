/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "ui/drawingsurfacewidget.h"
#include "wmsservicelayermodel.h"

#include "wmslayerpreview.h"

#include "MapLink.h"

namespace Services
{

  WMSLayerPreview::WMSLayerPreview( const std::string &serviceURL )
    : ServiceLayerPreview( serviceURL )
      , m_dataLayer( NULL )
  {
  }

  WMSLayerPreview::~WMSLayerPreview()
  {
    if( m_dataLayer )
    {
      m_dataLayer->destroy();
    }
  }

  void WMSLayerPreview::showLayerPreview( const QModelIndex &layerIndex, const char *styleName )
  {
    if( !layerIndex.isValid() )
    {
      return;
    }

    if( m_dataLayer )
    {
      m_surfaceWidget->drawingSurface()->removeDataLayer( "preview" );

      // Delete the layer in another thread to avoid blocking the UI
      DeletionThread *deleter = new DeletionThread( m_dataLayer );
      deleter->start();
    }

    m_dataLayer = new TSLWMSDataLayer( this );
    m_dataLayer->setDefaultLoaderCallbacks( &ServiceLayerPreview::loadCallback, this,
        &ServiceLayerPreview::allLoadedCallback, this );

    // In order to keep the UI responsive we want all connections to occur in a background thread
    // instead of the UI thread
    m_dataLayer->setSynchronousLoading( false );
    m_dataLayer->synchronousLoadStrategy( false );
    m_dataLayer->validateGetMapRequest( true );
    m_dataLayer->tileRequests( false );

    m_surfaceWidget->drawingSurface()->addDataLayer( m_dataLayer, "preview" );

    WMSServiceLayerModel::WMSLayerNodeInfo *layerNode = reinterpret_cast< WMSServiceLayerModel::WMSLayerNodeInfo* >( layerIndex.internalPointer() );
    if( layerNode->m_layer->name() )
    {
      m_layerName = layerNode->m_layer->name();
    }
    else
    {
      m_layerName.clear();
    }

    if( styleName )
    {
      m_layerStyle = styleName;
    }
    else
    {
      m_layerStyle.clear();
    }

    m_dataLayer->loadData( m_serviceURL.c_str() );
    showLoadMessageLayer();
  }

  // Data layer callbacks
  bool WMSLayerPreview::onCapabilitiesLoaded (TSLWMSServiceLayer *rootLayerInfo)
  {
    // Find the layer in the tree that matches the name we were asked to show
    if( rootLayerInfo->name() && m_layerName.compare( rootLayerInfo->name() ) == 0 )
    {
      // This is the layer we want, make it visible
      rootLayerInfo->setVisibility( true );
      rootLayerInfo->setStyleValue( m_layerStyle.c_str() );
      return !m_loadCancelled;
    }

    // Recurse to child layers if this isn't the layer we wanted
    int numChildLayers = rootLayerInfo->noOfSubLayers();
    for( int i = 0; i < numChildLayers; ++i )
    {
      onCapabilitiesLoaded( rootLayerInfo->getSubLayerAt(i) );
    }

    return !m_loadCancelled;
  }

  void WMSLayerPreview::onCapabilitiesLoadFailure (TSLWMSServiceSettingsCallbacks::CapabilitiesLoadFailureReason /*reason*/)
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
  }

  bool WMSLayerPreview::onNoVisibleLayers (TSLWMSServiceLayer* /*rootLayer*/)
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
    return false;
  }

  bool WMSLayerPreview::onRequiredDimensionValueNotSet (TSLWMSServiceLayer* /*rootLayerInfo*/, TSLWMSServiceLayer* /*dimensionLayer*/, const char* /*dimensionName*/)
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
    return false;
  }

  bool WMSLayerPreview::onNoCommonCRSinVisibleLayers (TSLWMSServiceLayer* /*rootLayer*/)
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
    return false;
  }

  bool WMSLayerPreview::onNoSupportedCRSinVisibleLayers (TSLWMSServiceLayer* /*rootLayer*/)
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
    return false;
  }

  int WMSLayerPreview::onChoiceOfRequestFormats (const char** /*formatChoices*/, int /*noOfFormatChoices*/)
  {
    return 0;
  }

  int WMSLayerPreview::onChoiceOfServiceCRSs (const char** crsChoices, int noOfCRSChoices)
  {
    return 0;
  }

  bool WMSLayerPreview::onUserLinearTransformInvalid()
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
    return false;
  }

  bool WMSLayerPreview::onServiceSettingsComplete()
  {
    m_messageLayer->setLoadFailStatus( false );
    m_surfaceWidget->signalResetView();
    return !m_loadCancelled;
  }

};
