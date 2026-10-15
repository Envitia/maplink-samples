#pragma once

class CEvent;
class CWnd;
class COpenServiceDlg;

class WMTSSettingsCallbacks : public TSLWMTSServiceSettingsCallbacks
{
public:
  WMTSSettingsCallbacks();
  virtual ~WMTSSettingsCallbacks();

  virtual bool onCapabilitiesLoaded (TSLWMTSServiceInfo* serviceInfo);

  virtual void onCapabilitiesLoadFailure (CapabilitiesLoadFailureReason reason);

  virtual bool onNoVisibleLayers (TSLWMTSServiceInfo* serviceInfo);

  virtual bool onTileMatrixSetNotSelected (TSLWMTSServiceInfo* serviceInfo, TSLWMTSServiceLayer* layer);

  virtual bool onNoCommonCRSinVisibleLayers (TSLWMTSServiceInfo* serviceInfo);

  virtual bool onSelectedCRSNotSupported (TSLWMTSServiceInfo* serviceInfo);

  virtual int onChoiceOfRequestFormats (TSLWMTSServiceLayer* layer, const char **formatChoices, int numFormatChoices);

  virtual int onChoiceOfServiceCRSs (const char** crsChoices, int noOfChoices);

  virtual bool onUserLinearTransformInvalid ();

  virtual bool onServiceSettingsComplete ();

  void setOpenDlg(CWnd * dlg)
  {
    m_dlg = dlg;
  }

private:
  CWnd *  m_dlg;
  CEvent* m_returnEvent;
};
