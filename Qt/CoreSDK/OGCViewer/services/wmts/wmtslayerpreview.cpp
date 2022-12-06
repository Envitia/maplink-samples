/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "ui/drawingsurfacewidget.h"
#include "wmtsservicelayermodel.h"
#include "wmtslayerpreview.h"

namespace Services
{

  WMTSLayerPreview::WMTSLayerPreview( const std::string &serviceURL )
    : ServiceLayerPreview( serviceURL )
      , m_dataLayer( NULL )
  {
  }

  WMTSLayerPreview::~WMTSLayerPreview()
  {
    if( m_dataLayer )
    {
      m_dataLayer->destroy();
    }
  }

  void WMTSLayerPreview::showLayerPreview( const QModelIndex &layerIndex, const char *styleName )
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

    m_dataLayer = new TSLWMTSDataLayer( this );
    m_dataLayer->setDefaultLoaderCallbacks( &ServiceLayerPreview::loadCallback, this,
        &ServiceLayerPreview::allLoadedCallback, this );

    // In order to keep the UI responsive we want all connections to occur in a background thread
    // instead of the UI thread
    m_dataLayer->setSynchronousLoading( false );
    m_dataLayer->synchronousLoadStrategy( false );

    m_surfaceWidget->drawingSurface()->addDataLayer( m_dataLayer, "preview" );

    WMTSServiceLayerModel::WMTSLayerNodeInfo *layerNode = reinterpret_cast< WMTSServiceLayerModel::WMTSLayerNodeInfo* >( layerIndex.internalPointer() );
    if( layerNode->m_layer->identifier() )
    {
      m_layerName = layerNode->m_layer->identifier();
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
  bool WMTSLayerPreview::onCapabilitiesLoaded (TSLWMTSServiceInfo *rootLayerInfo)
  {
    // Find the layer we want in the service's layer list
    int numLayers = rootLayerInfo->numLayers();
    for( int i = 0; i < numLayers; ++i )
    {
      TSLWMTSServiceLayer *layer = rootLayerInfo->getLayerAt(i);
      if( m_layerName.compare( layer->identifier() ) == 0 )
      {
        // This is the layer we want, make it visible
        layer->setVisibility( true );
        layer->setStyleValue( m_layerStyle.c_str() );
        return true;
      }
    }

    return true;
  }

  void WMTSLayerPreview::onCapabilitiesLoadFailure (TSLWMTSServiceSettingsCallbacks::CapabilitiesLoadFailureReason /*reason*/)
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
  }

  bool WMTSLayerPreview::onNoVisibleLayers (TSLWMTSServiceInfo* /*rootLayer*/)
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
    return false;
  }

  int WMTSLayerPreview::onChoiceOfServiceCRSs (const char** /*crsChoices*/, int /*noOfChoices*/)
  {
    return 0;
  }

  bool WMTSLayerPreview::onTileMatrixSetNotSelected (TSLWMTSServiceInfo* /*serviceInfo*/, TSLWMTSServiceLayer* /*layer*/)
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
    return false;
  }

  bool WMTSLayerPreview::onNoCommonCRSinVisibleLayers (TSLWMTSServiceInfo* /*serviceInfo*/)
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
    return false;
  }

  bool WMTSLayerPreview::onSelectedCRSNotSupported (TSLWMTSServiceInfo* /*serviceInfo*/)
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
    return false;
  }

  int WMTSLayerPreview::onChoiceOfRequestFormats (TSLWMTSServiceLayer* /*layer*/, const char** /*formatChoices*/, int /*noOfFormatChoices*/)
  { 
    return 0;
  }

  bool WMTSLayerPreview::onUserLinearTransformInvalid()
  {
    m_messageLayer->setLoadFailStatus( true );
    m_surfaceWidget->signalResetView();
    return false;
  }

  bool WMTSLayerPreview::onServiceSettingsComplete()
  {
    m_messageLayer->setLoadFailStatus( false );
    m_surfaceWidget->signalResetView();
    return !m_loadCancelled;
  }
};
