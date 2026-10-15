#include "stdafx.h"
#include "WMSSettingsCallbacks.h"
#include "OpenServiceDlg.h"
#include "CRSDlg.h"

WMSSettingsCallbacks::WMSSettingsCallbacks()
  : m_dlg(NULL)
  , m_returnEvent(NULL)
{
}

WMSSettingsCallbacks::~WMSSettingsCallbacks()
{
  if ( m_returnEvent )
  {
    delete m_returnEvent;
    m_returnEvent = 0;
  }
}


bool WMSSettingsCallbacks::onAuthenticationRequired(TSLSimpleString& username, TSLSimpleString& password)
{
  //This callback is not currently used by MapLink and is reserved for future use.
  return false;
}

bool WMSSettingsCallbacks::onCapabilitiesLoaded (TSLWMSServiceLayer *rootLayerInfo)
{
  ASSERT(m_dlg);
  //The dialog will lock the event and maintain reference for unlocking when ready
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_CAPABILITIES_LOADED, (WPARAM)m_returnEvent, (LPARAM)&rootLayerInfo);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  delete m_returnEvent;
  m_returnEvent = 0;
  return rootLayerInfo != 0;
}

void WMSSettingsCallbacks::onCapabilitiesLoadFailure (CapabilitiesLoadFailureReason reason)
{
  ASSERT(m_dlg);
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
  ASSERT(m_dlg);
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_SHOW_DS_DLG, (WPARAM)m_returnEvent, (LPARAM)&rootLayerInfo);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  delete m_returnEvent;
  m_returnEvent = 0;
  return rootLayerInfo != 0;
}

bool WMSSettingsCallbacks::onNoCommonCRSinVisibleLayers (TSLWMSServiceLayer *rootLayer)
{
  ASSERT(m_dlg);
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_NO_COMMON_CRS, (WPARAM)m_returnEvent, (LPARAM)&rootLayer);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  delete m_returnEvent;
  m_returnEvent = 0;
  return rootLayer != 0;
}

bool WMSSettingsCallbacks::onNoSupportedCRSinVisibleLayers (TSLWMSServiceLayer *rootLayer)
{
  ASSERT(m_dlg);
  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_NO_SUPPORTED_CRS, (WPARAM)m_returnEvent, (LPARAM)&rootLayer);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  delete m_returnEvent;
  m_returnEvent = 0;
  return rootLayer != 0;
}

int WMSSettingsCallbacks::onChoiceOfServiceCRSs (const char **crsChoices, int noOfCRSChoices)
{
  int crs = 0;
  if (noOfCRSChoices > 1)
  {
    ASSERT(m_dlg);
    m_returnEvent = new CEvent(0, true, 0, 0);
    CRSData data(crsChoices, noOfCRSChoices);
    ::PostMessage(m_dlg->GetSafeHwnd(), WM_CRS_CHOICE, (WPARAM)m_returnEvent, (LPARAM)&data);
    m_returnEvent->Lock();
    m_returnEvent->Unlock();
    delete m_returnEvent;
    m_returnEvent = 0;
    crs = data.m_chosenCRS;
  }
  return crs;
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
  ASSERT(m_dlg);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_CAPABILITIES_COMPLETE, 0, 0);
  return true;
}

int WMSSettingsCallbacks::onChoiceOfIDP (unsigned int numberOfIDPs, const IDPOrganisation* organisations[], const char* urls[], const char* entityIds[])
{
  ASSERT(m_dlg);

  void** param = new void*[3];
  int returnValue = 0;
  param[0] = &numberOfIDPs;
  param[1] = organisations;
  param[2] = &returnValue;

  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_IDP_CHOICE, (WPARAM)m_returnEvent, (LPARAM)param);
  
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  delete m_returnEvent;
  m_returnEvent = 0;

  return returnValue;
}

bool WMSSettingsCallbacks::onCredentialsRequired (TSLSimpleString& username, TSLSimpleString& password, const IDPOrganisation* organisation, const char* entityId)
{
  ASSERT(m_dlg);

  void** vs = new void*[3];
  vs[0] = &username;
  vs[1] = &password;
  vs[2] = (void*)organisation;

  m_returnEvent = new CEvent(0, true, 0, 0);
  ::PostMessage(m_dlg->GetSafeHwnd(), WM_CREDENTIALS_REQUIRED, (WPARAM)m_returnEvent, (LPARAM)vs);
  m_returnEvent->Lock();
  m_returnEvent->Unlock();
  delete m_returnEvent;
  m_returnEvent = 0;

  //The OpenServiceDlg will clear the value set in the first
  //array entry if the call was unsuccessful
  bool retval = vs[0] != 0;
  delete [] vs;
  return retval;
}