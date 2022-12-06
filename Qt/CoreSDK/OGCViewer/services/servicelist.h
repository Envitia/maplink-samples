/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef SERVICELIST_H
#define SERVICELIST_H

#include <vector>

#include "service.h"

#include "tslloadercallbackreturn.h"
#include "tslloaderstatus.h"
#include "tslatomic.h"
#include "tslcoord.h"
#include "tslenvelope.h"
#include "tslloaderappcallback.h"
#include "tslallloadedcallback.h"
#include "tgmapidll.h"
#include "tslsimplestring.h"
#include "tslremoteauthenticationcallback.h"


class DrawingSurfaceWidget;
class TSLFileLoaderRemote;

// This class holds all the services that are loaded into the viewer and provides
// access to some common operations that can be applied to all of them.


namespace Services
{
  class ServiceListModel;

  class ServiceList: public TSLRemoteAuthenticationCallback
  {
    public:
      ServiceList();
      ~ServiceList();

      // Adds a new service to the list, for display in the attached drawing surface
      void addService( Service *newService );

      // Removes the given service from the list and deletes it
      bool removeService( Service *removedService );

      // Sets the drawing surface that the connected services will be displayed in
      void setDrawingSurface( DrawingSurfaceWidget *surface );

      // Returns the remote loader shared between all services
      TSLFileLoaderRemote* getCommonLoader();

      // Returns an adapter that maps the contents of the service list to a Qt view
      // object for presentation in the user interface.
      ServiceListModel* getDisplayModel();

      // Called when the order of services are changed. The equivalent change needs to be
      // made to the order of the data layers in any attached drawing surfaces
      void updateLayerDrawOrder( int oldIndex, int newIndex, bool inFront );

      // This function causes a redraw to occur in the drawing thread for the attached drawing surface
      void redrawAttachedSurface();

      // Changes the view in the attached drawing surface to cover the given extent (preserving aspect ratio)
      void setViewedExtent( TSLTMC x1, TSLTMC y1, TSLTMC x2, TSLTMC y2 );

      // Updates the transparency of the data layer inside service in the attached drawing surface
      TSLPropertyValue getServiceDataLayerTransparency( Service *service );
      void setServiceDataLayerTransparency( Service *service, TSLPropertyValue transparency );

      // Sets/returns the number of concurrent connections in use in the shared remote loader
      void setNumConnections( int numConnections );
      int numConnections() const;

      // Sets/returns the size of the data layer caches used by all data layers in existance
      void setCacheSizes( int size );
      int cacheSizes() const;

      // Additional call forwards to make on file load callbacks in order to update the user interface
      void setLoadCallbackForwards( TSLLoaderAppCallback loadCallback, void *arg, TSLAllLoadedCallback allLoadedCallback, void *arg2 );

      // File loader callbacks
      static TSLLoaderCallbackReturn loadCallback( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone );
      static void allLoadedCallback( void *arg );

      virtual void onCredentialsRequired (TSLFileLoaderRemote* loader, TSLSimpleString& username, TSLSimpleString& password, const char* url);
      void clearCachedCredentials();
      void setCredentialsCallback( Service::ServiceCredentialsCallback* callback );

    private:
      std::vector< Service* > m_connectedServices;

      // Data model for the services to be used to populate the layer tree
      ServiceListModel *m_serviceListModel;

      DrawingSurfaceWidget *m_surfaceWidget;

      // Common file loader to be used by all services
      TSLFileLoaderRemote *m_commonLoader;

      Service::ServiceCredentialsCallback* m_credentialsCallback;

      int m_serviceCacheSize;

      TSLLoaderAppCallback m_loadCallbackForward;
      void *m_loadCallbackArg;
      TSLAllLoadedCallback m_allLoadedCallbackForward;
      void *m_allLoadedCallbackArg;
  };


  inline TSLFileLoaderRemote* ServiceList::getCommonLoader()
  {
    return m_commonLoader;
  }

  inline ServiceListModel* ServiceList::getDisplayModel()
  {
    return m_serviceListModel;
  }

};
#endif
