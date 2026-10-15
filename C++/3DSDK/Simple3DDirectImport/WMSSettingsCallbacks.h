////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2017 by Envitia Group PLC
////////////////////////////////////////////////////////
#pragma once

class COpenServiceDlg;

class WMSSettingsCallbacks : public TSLWMSServiceSettingsCallbacks
{
public:
  WMSSettingsCallbacks(CWnd * dlg);
  ~WMSSettingsCallbacks(void);
  
  void setDlg(CWnd *dlg);

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

private:
  CWnd *  m_dlg;
  CEvent* m_returnEvent;
   
};
