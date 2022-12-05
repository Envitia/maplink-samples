/****************************************************************************
  Copyright (c) 2017 by Envitia Group PLC.
 ****************************************************************************/

#ifndef WMSLAYERPREVIEW_H
#define WMSLAYERPREVIEW_H

#include "services/servicelayerpreview.h"
#include "tsltmsapi.h"
#include "tslwmsservicesettingscallbacks.h"

class TSLWMSDataLayer;

// A simple class that shows a single layer from a WMS. Used to provide layer previews
// in the SelectLayersPage wizard page for WMS services.
namespace Services
{

  class WMSLayerPreview : public ServiceLayerPreview, public TSLWMSServiceSettingsCallbacks
  {
    public:
      WMSLayerPreview( const std::string &serviceURL );
      virtual ~WMSLayerPreview();

      virtual void showLayerPreview( const QModelIndex &layerIndex, const char *styleName );

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
      TSLWMSDataLayer *m_dataLayer;
  };

};
#endif
