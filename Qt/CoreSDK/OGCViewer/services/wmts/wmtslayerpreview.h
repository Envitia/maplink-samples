#ifndef WMTSLAYERPREVIEW_H
#define WMTSLAYERPREVIEW_H

#include "services/servicelayerpreview.h"

#include "MapLink.h"
#include "MapLinkWMTSDataLayer.h"

class TSLWMTSDataLayer;

// A simple class that shows a single layer from a WMTS. Used to provide layer previews
// in the SelectLayersPage wizard page for WMTS services.
namespace Services
{
  class WMTSLayerPreview : public ServiceLayerPreview, public TSLWMTSServiceSettingsCallbacks
  {
    public:
      WMTSLayerPreview( const std::string &serviceURL );
      virtual ~WMTSLayerPreview();

      virtual void showLayerPreview( const QModelIndex &layerIndex, const char *styleName );

      // Data layer callbacks
      virtual bool onCapabilitiesLoaded (TSLWMTSServiceInfo *rootLayerInfo);
      virtual void onCapabilitiesLoadFailure (TSLWMTSServiceSettingsCallbacks::CapabilitiesLoadFailureReason reason);
      virtual bool onNoVisibleLayers (TSLWMTSServiceInfo *rootLayer);
      virtual int onChoiceOfServiceCRSs (const char** crsChoices, int noOfChoices);
      virtual bool onTileMatrixSetNotSelected (TSLWMTSServiceInfo* serviceInfo, TSLWMTSServiceLayer* layer);
      virtual bool onNoCommonCRSinVisibleLayers (TSLWMTSServiceInfo *serviceInfo);
      virtual bool onSelectedCRSNotSupported (TSLWMTSServiceInfo* serviceInfo);
      virtual int onChoiceOfRequestFormats (TSLWMTSServiceLayer* layer, const char **formatChoices, int noOfFormatChoices);
      virtual bool onUserLinearTransformInvalid();
      virtual bool onServiceSettingsComplete();

    private:
      TSLWMTSDataLayer *m_dataLayer;
  };
};
#endif
