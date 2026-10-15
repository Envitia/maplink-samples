////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2017 by Envitia Group PLC
////////////////////////////////////////////////////////
#include "stdafx.h"
#include "WMSSettingsCallbacks.h"
#include "OpenServiceDlg.h"

WMSSettingsCallbacks::WMSSettingsCallbacks(CWnd * dlg)
  : m_dlg(dlg)
  , m_returnEvent(0)
{
}

WMSSettingsCallbacks::~WMSSettingsCallbacks(void)
{
  if ( m_returnEvent )
  {
    delete m_returnEvent;
    m_returnEvent = 0;
  }
}

void WMSSettingsCallbacks::setDlg(CWnd *dlg)
{
  m_dlg = dlg;
}


bool WMSSettingsCallbacks::onCapabilitiesLoaded (TSLWMSServiceLayer *rootLayerInfo)
{
  if (!m_dlg)
    return false;

  //The dialog will lock the event and maintain reference for unlocking when ready
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_CAPABILITIES_LOADED, (WPARAM)m_returnEvent, (LPARAM)&rootLayerInfo);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  if ( m_returnEvent )
  {
    delete m_returnEvent;
    m_returnEvent = 0;
  }
  return rootLayerInfo != 0;
}

void WMSSettingsCallbacks::onCapabilitiesLoadFailure (CapabilitiesLoadFailureReason reason)
{
  if (!m_dlg)
    return ;

  ::PostMessage(m_dlg->GetSafeHwnd(), WM_CAPABILITIES_LOAD_FAIL, (WPARAM)reason, 0);
}

bool WMSSettingsCallbacks::onNoVisibleLayers (TSLWMSServiceLayer *rootLayer)
{
  //Should not happen
  ASSERT(false);
  return false;
}

bool WMSSettingsCallbacks::onRequiredDimensionValueNotSet (TSLWMSServiceLayer *rootLayerInfo, TSLWMSServiceLayer *dimensionLayer, const char *dimensionName)
{
  if (!m_dlg)
    return false;
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_SHOW_DS_DLG, (WPARAM)m_returnEvent, (LPARAM)&rootLayerInfo);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  if ( m_returnEvent )
  {
    delete m_returnEvent;
    m_returnEvent = 0;
  }
  return rootLayerInfo != 0;
}

bool WMSSettingsCallbacks::onNoCommonCRSinVisibleLayers (TSLWMSServiceLayer *rootLayer)
{
  if (!m_dlg)
    return false;
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_NO_COMMON_CRS, (WPARAM)m_returnEvent, (LPARAM)&rootLayer);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  if ( m_returnEvent )
  {
    delete m_returnEvent;
    m_returnEvent = 0;
  }
  return rootLayer != 0;
}

bool WMSSettingsCallbacks::onNoSupportedCRSinVisibleLayers (TSLWMSServiceLayer *rootLayer)
{
  if (!m_dlg)
    return false;
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_NO_SUPPORTED_CRS, (WPARAM)m_returnEvent, (LPARAM)&rootLayer);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  if ( m_returnEvent )
  {
    delete m_returnEvent;
    m_returnEvent = 0;
  }
  return rootLayer != 0;
}

int WMSSettingsCallbacks::onChoiceOfServiceCRSs (const char **crsChoices, int noOfCRSChoices)
{
  return 0;
}

int WMSSettingsCallbacks::onChoiceOfRequestFormats (const char **formatChoices, int noOfFormatChoices)
{
  for ( int i = 0; i < noOfFormatChoices; ++i )
  {
    if ( strcmp(formatChoices[i], "image/png") == 0 ) 
      return i;
  }
  return 0;
}

bool WMSSettingsCallbacks::onUserLinearTransformInvalid ()
{
  //Should not happen
  ASSERT(false);
  return false;
}

bool WMSSettingsCallbacks::onServiceSettingsComplete ()
{
  if (!m_dlg)
    return false;
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_CAPABILITIES_COMPLETE, 0, 0);
  return true;
}
