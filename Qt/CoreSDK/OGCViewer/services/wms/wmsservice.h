/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef WMSSERVICE_H
#define WMSSERVICE_H

#include <set>
#include <map>

#include "wmsservicelayermodel.h"
#include "wmsservicelayerinfomodel.h"

#include "services/service.h"

#include "tsltmsapi.h"
#include "tslwmsservicesettingscallbacks.h"

class TSLWMSDataLayer;
class TSLWMSServiceLayer;

// An implementation of the Service interface that connects to OGC Web Map Services.
namespace Services
{
  class WMSService : public Service, public TSLWMSServiceSettingsCallbacks
  {
    public:
      class WMSServiceLayer : public Service::ServiceLayer
    {
      public:
        WMSServiceLayer( TSLWMSServiceLayer *layer );
        virtual ~WMSServiceLayer();

        virtual const char* displayName() const;
        virtual void getDimensionNames( std::vector< std::string > &dimensionNames ) const;
        virtual const char* getDimensionUnits( const std::string &name ) const;
        virtual void getStyleNames( std::vector< std::string > &styleNames ) const;
        virtual bool getTMCExtent( TSLTMC &x1, TSLTMC &y1, TSLTMC &x2, TSLTMC &y2 ) const;

        virtual bool setStyle( const char *styleName );

        TSLWMSServiceLayer *layer();

      private:
        TSLWMSServiceLayer *m_layer;
        std::map< std::string, int > m_dimensionsLookup;
    };

      WMSService();
      virtual ~WMSService();

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
      // parse the layer tree to find visible layers
      void layerVisiblityChanged( TSLWMSServiceLayer *layer );
      bool anyLayersVisible() const;

      // Data layer callbacks
      virtual bool onCapabilitiesLoaded (TSLWMSServiceLayer *rootLayerInfo);
      virtual void onCapabilitiesLoadFailure (TSLWMSServiceSettingsCallbacks::CapabilitiesLoadFailureReason reason);
      virtual bool onNoVisibleLayers (TSLWMSServiceLayer *rootLayer);
      virtual bool onRequiredDimensionValueNotSet (TSLWMSServiceLayer *rootLayerInfo, TSLWMSServiceLayer *dimensionLayer, const char *dimensionName);
      virtual bool onNoCommonCRSinVisibleLayers (TSLWMSServiceLayer *rootLayer);
      virtual bool onNoSupportedCRSinVisibleLayers (TSLWMSServiceLayer *rootLayer);
      virtual int onChoiceOfRequestFormats (const char **formatChoices, int noOfFormatChoices);
      virtual int onChoiceOfServiceCRSs (const char **crsChoices, int noOfCRSChoices);
      virtual bool onUserLinearTransformInvalid();
      virtual bool onServiceSettingsComplete();

    private:
      void buildSortedLayerVisibilityList( TSLWMSServiceLayer *layer );
      TSLWMSServiceLayer* findLayer( TSLWMSServiceLayer *layer, const char *name );

      // The MapLink data layer used to display the service
      TSLWMSDataLayer *m_dataLayer;

      // List of the layers that are shown and not shown
      std::set< const TSLWMSServiceLayer* > m_visibleLayers;
      std::map< int, TSLWMSServiceLayer* > m_sortedLayerVisibility;
      std::set< const TSLWMSServiceLayer* > m_invisibleLayers;

      // Storage for information that is only valid for the duration of specific callbacks in the loading sequence
      TSLWMSServiceLayer *m_serviceRootLayer;
      const char **m_crsChoices;
      size_t m_numCRSChoices;
      int m_coordinateSystemChoice;
  };

  inline TSLWMSServiceLayer* WMSService::WMSServiceLayer::layer()
  {
    return m_layer;
  }

  inline bool WMSService::anyLayersVisible() const
  {
    return !m_visibleLayers.empty();
  }
};
#endif
