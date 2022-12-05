/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "drawingsurfacewidget.h"

#include "servicelayerpreview.h"
namespace Services
{
  static const char *g_loadMessageLayerName = "messageLayer";

  ServiceLayerPreview::ServiceLayerPreview( const std::string &serviceURL )
    : m_surfaceWidget( NULL )
      , m_serviceURL( serviceURL )
      , m_loadCancelled( false )
      , m_loadFailed( false )
      , m_customLayer( new TSLCustomDataLayer() )
      , m_messageLayer( new PreviewLoadingLayer() )
  {
    m_customLayer->setClientCustomDataLayer( m_messageLayer );
  }

  ServiceLayerPreview::~ServiceLayerPreview()
  {
    if( m_customLayer )
    {
      m_customLayer->destroy();
    }
    delete m_messageLayer;
  }

  void ServiceLayerPreview::setSurfaceWidget( DrawingSurfaceWidget *previewWidget )
  {
    m_surfaceWidget = previewWidget;

    m_surfaceWidget->drawingSurface()->addDataLayer( m_customLayer, g_loadMessageLayerName );
    m_surfaceWidget->drawingSurface()->setDataLayerProps( g_loadMessageLayerName, TSLPropertyVisible, 0 );
  }

  void ServiceLayerPreview::showLoadMessageLayer()
  {
    m_messageLayer->setLoadFailStatus( false );
    m_surfaceWidget->drawingSurface()->setDataLayerProps( g_loadMessageLayerName, TSLPropertyVisible, 1 );
    emit m_surfaceWidget->signalRefreshView();
  }

  // File loader callbacks
  TSLLoaderCallbackReturn ServiceLayerPreview::loadCallback( void* arg, const char* /*filename*/, TSLEnvelope extent,
      TSLLoaderStatus status, int percentDone )
  {
    ServiceLayerPreview *preview = reinterpret_cast< ServiceLayerPreview* >( arg );
    if( status == TSLLoadingOK && percentDone == 100 )
    {
      emit preview->m_surfaceWidget->signalRefreshView();
    }

    return TSLContinue;
  }

  void ServiceLayerPreview::allLoadedCallback( void *arg )
  {
    ServiceLayerPreview *preview = reinterpret_cast< ServiceLayerPreview* >( arg );
    preview->m_surfaceWidget->drawingSurface()->setDataLayerProps( g_loadMessageLayerName, TSLPropertyVisible, 0 );
    emit preview->m_surfaceWidget->signalRefreshView();
  }

  ServiceLayerPreview::DeletionThread::DeletionThread( TSLDataLayer *layer )
    : m_layer( layer )
  {
    connect( this, SIGNAL(finished()), this, SLOT(deleteLater()) );
  }

  ServiceLayerPreview::DeletionThread::~DeletionThread()
  {
  }

  void ServiceLayerPreview::DeletionThread::run()
  {
    m_layer->destroy();
  }

  ServiceLayerPreview::PreviewLoadingLayer::PreviewLoadingLayer()
    : m_showFailMessage( false )
  {
  }

  ServiceLayerPreview::PreviewLoadingLayer::~PreviewLoadingLayer()
  {
  }

  bool ServiceLayerPreview::PreviewLoadingLayer::drawLayer( TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& /*layerHandler*/)
  {
    renderingInterface->setupTextAttributes( 1, TSLComposeRGB( 0, 0, 0 ),
        16.0, TSLDimensionUnitsPoints, 1, 2000, 0.0, 0.0, TSLDimensionUnitsPixels, 0.0,
        TSLHorizontalAlignmentCentre, TSLVerticalAlignmentMiddle );
    if( !m_showFailMessage )
    {
      return renderingInterface->drawText( extent->centre(), "Loading..." );
    }
    else
    {
      return renderingInterface->drawText( extent->centre(), "Preview unavailable" );
    }
  }
};
