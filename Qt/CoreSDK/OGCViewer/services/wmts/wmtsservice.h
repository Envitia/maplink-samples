/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef WMTSSERVICE_H
#define WMTSSERVICE_H

#include <set>
#include <map>
#include "services/service.h"

#include "MapLink.h"
#include "MapLinkWMTSDataLayer.h"
#include "wmtsservicelayermodel.h"
#include "wmtsservicelayerinfomodel.h"

// An implementation of the Service interface that connects to OGC Web Map Tile Services.
namespace Services
{
  class WMTSService : public Service, public TSLWMTSServiceSettingsCallbacks
  {
    public:
      class WMTSServiceLayer : public Service::ServiceLayer
      {
        public:
          WMTSServiceLayer( TSLWMTSServiceLayer *layer );
          virtual ~WMTSServiceLayer();

          virtual const char* displayName() const;
          virtual void getDimensionNames( std::vector< std::string > &dimensionNames ) const;
          virtual const char* getDimensionUnits( const std::string &name ) const;
          virtual void getStyleNames( std::vector< std::string > &styleNames ) const;
          virtual bool getTMCExtent( TSLTMC &x1, TSLTMC &y1, TSLTMC &x2, TSLTMC &y2 ) const;

          virtual bool setStyle( const char *styleName );

          TSLWMTSServiceLayer *layer();

        private:
          TSLWMTSServiceLayer *m_layer;
          std::map< std::string, int > m_dimensionsLookup;
      };

      WMTSService();
      virtual ~WMTSService();

      virtual void loadService( const char *address );
      void advanceConnectionSequence();

      virtual ServiceLayerModel* getServiceLayerModel();
      virtual ServiceLayerInfoModel* getServiceLayerInfoModel();
      virtual ServiceDimensionsModel* getDimensionsModel();
      virtual ServiceDimensionInfoModel* getDimensionInfoModel();
      virtual ServiceLayerStylesModel* getServiceLayerStyleModel();
      virtual ServiceLayerPreview* getLayerPreviewHelper();

      virtual const char* getServiceDisplayName() const;
      virtual void getVisibleLayers( std::vector< Service::ServiceLayer* > &layers );
      virtual void getPotentialLayersForDisplay( std::vector< std::string > &layerNames );
      virtual ServiceLayer* getLayerForToken( void* token );

      virtual size_t numCoordSystemChoices();
      virtual const char** coodinateSystemChoices();
      virtual void setCoordinateSystemChoice( int choice );

      virtual TSLDataLayer* dataLayer();
      virtual void updateLayerVisibilityOrder( int originalIndex, int newIndex );
      virtual void* setLayerVisibility( const char *layerName, bool visible );
      virtual void setLayerVisibility( ServiceLayer *layer, bool visible );

      // Changes the cache size of the enclosed data layer in the service
      virtual void setCacheSize( int size );

      // This function is used to record which layers are visible to avoid having to continuously
      // parse the layer list to find visible layers
      void layerVisiblityChanged( TSLWMTSServiceLayer *layer );
      bool anyLayersVisible() const;

      // Data layer callbacks
      virtual bool onCapabilitiesLoaded (TSLWMTSServiceInfo *rootLayerInfo);
      virtual void onCapabilitiesLoadFailure (TSLWMTSServiceSettingsCallbacks::CapabilitiesLoadFailureReason reason);
      virtual bool onNoVisibleLayers (TSLWMTSServiceInfo *rootLayer);
      virtual int onChoiceOfServiceCRSs (const char** crsChoices, int noOfChoices);
      virtual bool onNoCommonCRSinVisibleLayers (TSLWMTSServiceInfo *serviceInfo);
      virtual bool onSelectedCRSNotSupported (TSLWMTSServiceInfo* serviceInfo);
      virtual int onChoiceOfRequestFormats (TSLWMTSServiceLayer* layer, const char **formatChoices, int noOfFormatChoices);
      virtual bool onUserLinearTransformInvalid();
      virtual bool onServiceSettingsComplete();

    private:
      void buildSortedLayerVisibilityList( TSLWMTSServiceInfo *serviceInfo );
      TSLWMTSServiceLayer* findLayer( TSLWMTSServiceInfo *serviceInfo, const char *name );

      // The MapLink data layer used to display the service
      TSLWMTSDataLayer *m_dataLayer;

      // Storage for information that is only valid for the duration of specific callbacks in the loading sequence
      std::set< TSLWMTSServiceLayer* > m_visibleLayers;
      std::map< int, TSLWMTSServiceLayer* > m_sortedLayerVisibility;
      std::set< TSLWMTSServiceLayer* > m_invisibleLayers;

      // Storage for information that is only valid for the duration of specific callbacks in the loading sequence
      TSLWMTSServiceInfo *m_serviceInfo;
      const char **m_crsChoices;
      size_t m_numCRSChoices;
      int m_coordinateSystemChoice;
  };

  inline TSLWMTSServiceLayer* WMTSService::WMTSServiceLayer::layer()
  {
    return m_layer;
  }

  inline bool WMTSService::anyLayersVisible() const
  {
    return !m_visibleLayers.empty();
  }
};
#endif
