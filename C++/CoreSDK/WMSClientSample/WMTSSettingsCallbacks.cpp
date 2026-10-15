#include "stdafx.h"
#include "WMTSSettingsCallbacks.h"
#include "OpenWMTSDlg.h"
#include "CRSDlg.h"

WMTSSettingsCallbacks::WMTSSettingsCallbacks()
  : m_dlg(NULL)
  , m_returnEvent(NULL)
{
}

WMTSSettingsCallbacks::~WMTSSettingsCallbacks()
{
  if ( m_returnEvent )
  {
    delete m_returnEvent;
    m_returnEvent = 0;
  }
}


bool WMTSSettingsCallbacks::onCapabilitiesLoaded (TSLWMTSServiceInfo* serviceInfo)
{
  ASSERT(m_dlg);
  //The dialog will lock the event and maintain reference for unlocking when ready
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_WMTS_CAPABILITIES_LOADED, (WPARAM)m_returnEvent, (LPARAM)&serviceInfo);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  delete m_returnEvent;
  m_returnEvent = 0;
  return serviceInfo != 0;
}

void WMTSSettingsCallbacks::onCapabilitiesLoadFailure (CapabilitiesLoadFailureReason reason)
{
  ASSERT(m_dlg);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_WMTS_CAPABILITIES_LOAD_FAIL, (WPARAM)reason, 0);
}

bool WMTSSettingsCallbacks::onNoVisibleLayers (TSLWMTSServiceInfo* serviceInfo)
{
  //Should not happen
  ASSERT(false);
  return false;
}

bool WMTSSettingsCallbacks::onTileMatrixSetNotSelected (TSLWMTSServiceInfo* serviceInfo, TSLWMTSServiceLayer* layer)
{
  //Should not happen
  ASSERT(false);
  return false;
}

bool WMTSSettingsCallbacks::onNoCommonCRSinVisibleLayers (TSLWMTSServiceInfo* serviceInfo)
{
  ASSERT(m_dlg);
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_WMTS_NO_COMMON_CRS, (WPARAM)m_returnEvent, (LPARAM)&serviceInfo);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  delete m_returnEvent;
  m_returnEvent = 0;
  return serviceInfo != 0;
}

bool WMTSSettingsCallbacks::onSelectedCRSNotSupported (TSLWMTSServiceInfo* serviceInfo)
{
  ASSERT(m_dlg);
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_WMTS_NO_SUPPORTED_CRS, (WPARAM)m_returnEvent, (LPARAM)&serviceInfo);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  delete m_returnEvent;
  m_returnEvent = 0;
  return serviceInfo != 0;
}

int WMTSSettingsCallbacks::onChoiceOfRequestFormats (TSLWMTSServiceLayer* layer, const char **formatChoices, int numFormatChoices)
{
  for ( int i = 0; i < numFormatChoices; ++i )
  {
    if ( strcmp(formatChoices[i], "image/png") == 0 ) 
      return i;
  }
  return 0;
}

int WMTSSettingsCallbacks::onChoiceOfServiceCRSs (const char** crsChoices, int noOfChoices)
{
  int crs = 0;
  if (noOfChoices > 1)
  {
    ASSERT(m_dlg);
    m_returnEvent = new CEvent(0, true, 0, 0);
    CRSData data(crsChoices, noOfChoices);
    ::PostMessage(m_dlg->GetSafeHwnd(), WM_WMTS_CRS_CHOICE, (WPARAM)m_returnEvent, (LPARAM)&data);
    m_returnEvent->Lock();
    m_returnEvent->Unlock();
    delete m_returnEvent;
    m_returnEvent = 0;
    crs = data.m_chosenCRS;
  }
  return crs;
}


bool WMTSSettingsCallbacks::onUserLinearTransformInvalid ()
{
  //Should not happen
  ASSERT(false);
  return false;
}

bool WMTSSettingsCallbacks::onServiceSettingsComplete ()
{
  ASSERT(m_dlg);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_WMTS_CAPABILITIES_COMPLETE, 0, 0);
  return true;
}
