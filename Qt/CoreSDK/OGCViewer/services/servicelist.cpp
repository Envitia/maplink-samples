/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#include "ui/drawingsurfacewidget.h"

#include "servicelistmodel.h"

#include "servicelist.h"

#include "MapLink.h"
#include "MapLinkDrawing.h"

namespace Services
{
  static uint32_t g_dataLayerCounter = 0;

  ServiceList::ServiceList()
    : m_serviceListModel( new ServiceListModel( this ) )
      , m_surfaceWidget( NULL )
      , m_commonLoader( new TSLFileLoaderRemote( 8 ) ) // Default to 8 simultaneous connections
      , m_credentialsCallback( NULL )
      , m_serviceCacheSize( 128 ) // Default cache size is 128Mb
      , m_loadCallbackForward( NULL )
      , m_loadCallbackArg( NULL )
      , m_allLoadedCallbackForward( NULL )
      , m_allLoadedCallbackArg( NULL )
  {
    m_commonLoader->remoteAuthenticationCallback( (TSLRemoteAuthenticationCallback*)this );
  }

  ServiceList::~ServiceList()
  {
    // Clean up all services that were created
    std::vector< Service* >::iterator servicesIt( m_connectedServices.begin() );
    std::vector< Service* >::iterator servicesItE( m_connectedServices.end() );
    for( ; servicesIt != servicesItE; ++servicesIt )
    {
      delete *servicesIt;
    }

    if( m_commonLoader )
    {
      m_commonLoader->destroy();
    }

    delete m_serviceListModel;
  }

  void ServiceList::addService( Service *newService )
  {
    m_connectedServices.push_back( newService );

    // Tell the model about the new service so any connected views are updated
    m_serviceListModel->addService( newService );

    // Add the data layer contained in the service to the attached drawing surface
    // so that it will be displayed
    if( m_surfaceWidget && m_surfaceWidget->drawingSurface() )
    {
      TSLDrawingSurface *surface = m_surfaceWidget->drawingSurface();

      char layerIdentifier[32];
      TSL_SNPRINTF( layerIdentifier, sizeof( layerIdentifier ), "%u", g_dataLayerCounter++ );

      newService->setCacheSize( m_serviceCacheSize );
      surface->addDataLayer( newService->dataLayer(), layerIdentifier );
      newService->setDrawingSurfaceName( layerIdentifier );
      surface->reset( false );

      // Request a redraw
      emit m_surfaceWidget->signalRefreshView();
    }
  }

  bool ServiceList::removeService( Service *removedService )
  {
    // Remove and delete the service from the list
    std::vector< Service* >::iterator serviceEntry( find( m_connectedServices.begin(), m_connectedServices.end(), removedService ) );
    if( serviceEntry != m_connectedServices.end() )
    {
      m_connectedServices.erase( serviceEntry );
      delete removedService;

      // Request a redraw as we just removed a data layer from the drawing surface
      emit m_surfaceWidget->signalRefreshView();

      return true;
    }

    return false;
  }

  void ServiceList::setDrawingSurface( DrawingSurfaceWidget *surface )
  {
    m_surfaceWidget = surface;
  }

  void ServiceList::updateLayerDrawOrder( int oldIndex, int newIndex, bool inFront )
  {
    if( m_surfaceWidget )
    {
      // Move the layer that is currently at oldIndex in the drawing surface to newIndex,
      // either behind or in front of any layer currently at newIndex based on the value of inFront.
      TSLDrawingSurface *surface = m_surfaceWidget->drawingSurface();

      TSLDataLayer *movedLayer = NULL, *targetLayer = NULL;
      const char *movedLayerName = NULL, *targetLayerName = NULL;
      bool redrawRequired = false;
      if( surface->getDataLayerInfo( oldIndex, &movedLayer, &movedLayerName ) )
      {
        if( newIndex < surface->getNumDataLayers() )
        {
          if( surface->getDataLayerInfo( newIndex, &targetLayer, &targetLayerName ) )
          {
            if( inFront )
            {
              redrawRequired = surface->bringInFrontof( movedLayerName, targetLayerName );
            }
            else
            {
              redrawRequired = surface->sendToBackOf( movedLayerName, targetLayerName );
            }
          }
        }
        else
        {
          // The layer was moved to the top of the view stack
          redrawRequired = surface->bringToFront( movedLayerName );
        }
      }

      if( redrawRequired )
      {
        // The layer order was changed, so redraw the view to reflect the new layer order
        redrawAttachedSurface();
      }
    }
  }

  void ServiceList::redrawAttachedSurface()
  {
    if( m_surfaceWidget )
    {
      emit m_surfaceWidget->update();
    }
  }

  void ServiceList::setViewedExtent( TSLTMC x1, TSLTMC y1, TSLTMC x2, TSLTMC y2 )
  {
    if( !m_surfaceWidget )
    {
      return;
    }

    // Make the drawing surface show the requested TMC extent, preserving the
    // aspect ratio of the view relative to the window.
    TSLDrawingSurface *surface = m_surfaceWidget->drawingSurface();

    double uuX1, uuY1, uuX2, uuY2;
    surface->TMCToUU( x1, y1, &uuX1, &uuY1 );
    surface->TMCToUU( x2, y2, &uuX2, &uuY2 );
    surface->resize( uuX1, uuY1, uuX2, uuY2, false, true );

    // Request a redraw of the drawing surface through the widget
    emit m_surfaceWidget->update();
  }

  TSLPropertyValue ServiceList::getServiceDataLayerTransparency( Service *service )
  {
    if( !m_surfaceWidget )
    {
      return 255;
    }

    TSLDrawingSurface *surface = m_surfaceWidget->drawingSurface();
    TSLPropertyValue transparencyValue = 0;
    surface->getDataLayerProps( service->drawingSurfaceName().c_str(), TSLPropertyTransparency, &transparencyValue );
    return transparencyValue;
  }

  void ServiceList::setServiceDataLayerTransparency( Service *service, TSLPropertyValue transparency )
  {
    if( !m_surfaceWidget )
    {
      return;
    }

    TSLDrawingSurface *surface = m_surfaceWidget->drawingSurface();

    surface->setDataLayerProps( service->drawingSurfaceName().c_str(), TSLPropertyTransparency, transparency );

    // Redraw the layer in the drawing surface to show the new transparency setting
    service->dataLayer()->notifyChanged();

    emit m_surfaceWidget->update();
  }

  void ServiceList::setNumConnections( int numConnections )
  {
    if( m_commonLoader )
    {
      m_commonLoader->maximumConcurrentRequests( numConnections );
    }
  }

  void ServiceList::setCacheSizes( int size )
  {
    m_serviceCacheSize = size;

    // Apply the new size to all services already in existance
    std::vector< Service* >::iterator servicesIt( m_connectedServices.begin() );
    std::vector< Service* >::iterator servicesItE( m_connectedServices.end() );
    for( ; servicesIt != servicesItE; ++servicesIt )
    {
      (*servicesIt)->setCacheSize( size );
    }
  }

  int ServiceList::numConnections() const
  {
    return m_commonLoader->maximumConcurrentRequests();
  }

  int ServiceList::cacheSizes() const
  {
    return m_serviceCacheSize;
  }

  void ServiceList::setLoadCallbackForwards( TSLLoaderAppCallback loadCallback, void *arg, TSLAllLoadedCallback allLoadedCallback, void *arg2 )
  {
    m_loadCallbackForward = loadCallback;
    m_loadCallbackArg = arg;
    m_allLoadedCallbackForward = allLoadedCallback;
    m_allLoadedCallbackArg = arg2;
  }

  TSLLoaderCallbackReturn ServiceList::loadCallback( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone )
  {
    ServiceList *serviceList = reinterpret_cast< ServiceList* >( arg );

    if( status == TSLLoadingOK && percentDone == 100 )
    {
      // A new tile has finished loading, request a redraw of the attached drawing surface so that
      // it will be displayed
      emit serviceList->m_surfaceWidget->signalRefreshView();
    }

    if( serviceList->m_loadCallbackForward )
    {
      // Forward on the callback if we have somewhere to forward it to.
      (*serviceList->m_loadCallbackForward)( serviceList->m_loadCallbackArg, filename, extent, status, percentDone );
    }

    return TSLContinue;
  }

  void ServiceList::allLoadedCallback( void *arg )
  {
    ServiceList *serviceList = reinterpret_cast< ServiceList* >( arg );
    emit serviceList->m_surfaceWidget->signalRefreshView();

    // All outstanding tiles have finished loading, request a redraw of the attached drawing surface so that
    // the view will reflect the data that has been loaded.
    if( serviceList->m_allLoadedCallbackForward )
    {
      (*serviceList->m_allLoadedCallbackForward)( serviceList->m_allLoadedCallbackArg );
    }
  }

  void ServiceList::onCredentialsRequired (TSLFileLoaderRemote* loader, TSLSimpleString& username, TSLSimpleString& password, const char* url)
  {
    if( m_credentialsCallback && loader )
    {
      m_credentialsCallback->onCredentialsRequired( username, password );
      std::string domain( url );

      size_t lastDot = domain.find_last_of('.');
      size_t firstSlash = domain.find_first_of('/', lastDot );
      domain = domain.substr( 0, firstSlash );

      loader->addCredentials( username.c_str(), password.c_str(), domain.c_str() );
    }
  }

  void ServiceList::clearCachedCredentials()
  {
    m_commonLoader->clearCachedCredentials();
  }

  void ServiceList::setCredentialsCallback( Service::ServiceCredentialsCallback* callback )
  {
    m_credentialsCallback = callback;
  }

};
