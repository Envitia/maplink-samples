/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef SERVICELAYERPREVIEW_H
#define SERVICELAYERPREVIEW_H

#include <string>
#include <QThread>

#include "MapLink.h"
#include "MapLinkDrawing.h"

class DrawingSurfaceWidget;
class QModelIndex;
class TSLDataLayer;

// Abstract class for use by the SelectLayersPage in order to hide implementation details
// for showing the appearance of a WMS or WMTS layer in the layer preview pane.
namespace Services
{
  class ServiceLayerPreview
  {
    public:
      ServiceLayerPreview( const std::string &serviceURL );
      virtual ~ServiceLayerPreview();

      void setSurfaceWidget( DrawingSurfaceWidget *previewWidget );
      void cancelLoad();

      // Tells the preview helper to show a preview for the given layer using the named style
      virtual void showLayerPreview( const QModelIndex &layerIndex, const char *styleName ) = 0;

      // File loader callbacks
      static TSLLoaderCallbackReturn loadCallback( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone );
      static void allLoadedCallback( void *arg );

    protected:
      void showLoadMessageLayer();

      // Utility class to move data layer deletion to another thread. This avoids blocking
      // the UI thread when changing between layers faster than the preview can update
      // as deleting the preview data layer will block the calling thread until background tasks
      // are completed.
      // Qt threads are used so C++11 std::thread support is not required to build this application
      class DeletionThread : public QThread
      {
        public:
          DeletionThread( TSLDataLayer *layer );
          virtual ~DeletionThread();

          virtual void run();

        private:
          TSLDataLayer *m_layer;
      };

      // A small custom data layer used to display a loading message
      // while waiting for services to return the preview images from a background thread.
      class PreviewLoadingLayer : public TSLClientCustomDataLayer
      {
        public:
          PreviewLoadingLayer();
          virtual ~PreviewLoadingLayer();

          virtual bool drawLayer (TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler);

          void setLoadFailStatus( bool failed )
          {
            m_showFailMessage = failed;
          }

        private:
          bool m_showFailMessage;
      };

      DrawingSurfaceWidget *m_surfaceWidget;
      std::string m_layerName;
      std::string m_layerStyle;
      std::string m_serviceURL;
      bool m_loadCancelled;
      bool m_loadFailed;

      TSLCustomDataLayer *m_customLayer;
      PreviewLoadingLayer *m_messageLayer;
  };

  inline void ServiceLayerPreview::cancelLoad()
  {
    m_loadCancelled = true;
  }
};
#endif
