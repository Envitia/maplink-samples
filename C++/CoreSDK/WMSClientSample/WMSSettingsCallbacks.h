#pragma once

#include "TSLWMSECPServiceSettingsCallback.h"

class CEvent;
class CWnd;
class COpenServiceDlg;

class WMSSettingsCallbacks : public TSLWMSECPServiceSettingsCallbacks
{
public:
  WMSSettingsCallbacks();
  virtual ~WMSSettingsCallbacks();

  //Not currently used
  virtual bool onAuthenticationRequired(TSLSimpleString& username, TSLSimpleString& password);

  virtual bool onCapabilitiesLoaded (TSLWMSServiceLayer *rootLayerInfo);

  virtual void onCapabilitiesLoadFailure (CapabilitiesLoadFailureReason reason);

  virtual bool onNoVisibleLayers (TSLWMSServiceLayer *rootLayer);

  virtual bool onRequiredDimensionValueNotSet (TSLWMSServiceLayer *rootLayerInfo, TSLWMSServiceLayer *dimensionLayer, const char *dimensionName);

  virtual bool onNoCommonCRSinVisibleLayers (TSLWMSServiceLayer *rootLayer);

  virtual bool onNoSupportedCRSinVisibleLayers (TSLWMSServiceLayer *rootLayer);

  virtual int onChoiceOfServiceCRSs (const char **crsChoices, int noOfCRSChoices);

  virtual int onChoiceOfRequestFormats (const char **formatChoices, int noOfFormatChoices);

  virtual bool onUserLinearTransformInvalid ();

  virtual bool onServiceSettingsComplete ();

  virtual int onChoiceOfIDP (unsigned int numberOfIDPs, const IDPOrganisation* organisations[], const char* urls[], const char* entityIds[]);

  virtual bool onCredentialsRequired (TSLSimpleString& username, TSLSimpleString& password, const IDPOrganisation* organisation, const char* entityId);

  void setOpenDlg(CWnd * dlg)
  {
    m_dlg = dlg;
  }

private:
  CWnd *  m_dlg;
  CEvent* m_returnEvent;
   
};
