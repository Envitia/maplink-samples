// ActiveXCPPCtrl.h : Declaration of the CActiveXCPPCtrl

#ifndef __ACTIVEXCPP_H_
#define __ACTIVEXCPP_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

class TSLViewMode;
class TSLViewModeInterfaceNT;

using std::string;
using std::vector;

/////////////////////////////////////////////////////////////////////////////
// CActiveXCPPCtrl
class ATL_NO_VTABLE CActiveXCPPCtrl : 
  public CComObjectRootEx<CComSingleThreadModel>,
  public IDispatchImpl<IActiveXCPP, &IID_IActiveXCPP, &LIBID_ACTIVEXCPPLib, 7, 0>,
  public IProvideClassInfo2Impl<&CLSID_ActiveXCPP, NULL, &LIBID_ACTIVEXCPPLib, 7, 0>,
  public CComControl<CActiveXCPPCtrl>,
  public IPersistStreamInitImpl<CActiveXCPPCtrl>,
  public IOleControlImpl<CActiveXCPPCtrl>,
  public IOleObjectImpl<CActiveXCPPCtrl>,
  public IOleInPlaceActiveObjectImpl<CActiveXCPPCtrl>,
  public IViewObjectExImpl<CActiveXCPPCtrl>,
  public IOleInPlaceObjectWindowlessImpl<CActiveXCPPCtrl>,
  public IPersistStorageImpl<CActiveXCPPCtrl>,
  public ISpecifyPropertyPagesImpl<CActiveXCPPCtrl>,
  public IQuickActivateImpl<CActiveXCPPCtrl>,
  public IDataObjectImpl<CActiveXCPPCtrl>,
  public CComCoClass<CActiveXCPPCtrl, &CLSID_ActiveXCPP>,
  public IPersistPropertyBagImpl<CActiveXCPPCtrl>
{
public:
  CActiveXCPPCtrl();
  ~CActiveXCPPCtrl();

  DECLARE_REGISTRY_RESOURCEID(IDR_ACTIVEXCPP)

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  BEGIN_COM_MAP(CActiveXCPPCtrl)
    COM_INTERFACE_ENTRY(IActiveXCPP)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IViewObjectEx)
    COM_INTERFACE_ENTRY(IViewObject2)
    COM_INTERFACE_ENTRY(IViewObject)
    COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY(IOleInPlaceObject)
    COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
    COM_INTERFACE_ENTRY(IOleControl)
    COM_INTERFACE_ENTRY(IOleObject)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
    COM_INTERFACE_ENTRY(IQuickActivate)
    COM_INTERFACE_ENTRY(IPersistStorage)
    COM_INTERFACE_ENTRY(IDataObject)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY(IPersistPropertyBag)
  END_COM_MAP()

  BEGIN_PROP_MAP(CActiveXCPPCtrl)
    PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
    PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)

    // Example entries
    PROP_ENTRY("mapName", 1, CLSID_NULL)
    PROP_ENTRY("serverMapPath", 2, CLSID_NULL)
    PROP_ENTRY("serverConfigPath", 3, CLSID_NULL)
    PROP_ENTRY("interactionMode", 4, CLSID_NULL)
    PROP_ENTRY("zoomPercentage", 5, CLSID_NULL)
    PROP_ENTRY("memoryCacheSize", 10, CLSID_NULL)
    PROP_ENTRY("persistentCacheDirectory", 13, CLSID_NULL)
    PROP_ENTRY("persistentCacheMaxSize", 14, CLSID_NULL)
  END_PROP_MAP()

  BEGIN_MSG_MAP(CActiveXCPPCtrl)
    CHAIN_MSG_MAP(CComControl<CActiveXCPPCtrl>)
    DEFAULT_REFLECTION_HANDLER()
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(AM_LOADER_CALLBACK, OnLoaderCallback)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
    MESSAGE_HANDLER(WM_RBUTTONDBLCLK, OnRButtonDown)
    MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
  END_MSG_MAP()

  // Handler prototypes:
  //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
  //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

  BEGIN_CATEGORY_MAP(CActiveXCPPCtrl)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()


  // IViewObjectEx
  DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

  // IActiveXCPP
public:
  STDMETHOD(print)();
  STDMETHOD(get_persistentCacheMaxSize)(/*[out, retval]*/ long *pVal);
  STDMETHOD(put_persistentCacheMaxSize)(/*[in]*/ long newVal);
  STDMETHOD(get_persistentCacheDirectory)(/*[out, retval]*/ BSTR *pVal);
  STDMETHOD(put_persistentCacheDirectory)(/*[in]*/ BSTR newVal);
  STDMETHOD(get_mapLayerName)(/*[out, retval]*/ BSTR *pVal);
  STDMETHOD(get_drawingSurfacePtr)(/*[out, retval]*/ long *pVal);
  STDMETHOD(get_memoryCacheSize)(/*[out, retval]*/ long *pVal);
  STDMETHOD(put_memoryCacheSize)(/*[in]*/ long newVal);
  STDMETHOD(setView)(/*[in]*/ double x, /*[in]*/ double y, /*[in]*/ double scale, /*[out, retval]*/ VARIANT_BOOL *rslt);
  STDMETHOD(zoomOut)(/*[out, retval]*/ VARIANT_BOOL *rslt);
  STDMETHOD(zoomIn)(/*[out, retval]*/ VARIANT_BOOL *rslt);
  STDMETHOD(resetView)(/*[out, retval]*/ VARIANT_BOOL *rslt);
  STDMETHOD(get_zoomPercentage)(/*[out, retval]*/ short *pVal);
  STDMETHOD(put_zoomPercentage)(/*[in]*/ short newVal);
  STDMETHOD(get_interactionMode)(/*[out, retval]*/ BSTR *pVal);
  STDMETHOD(put_interactionMode)(/*[in]*/ BSTR newVal);
  STDMETHOD(get_serverConfigPath)(/*[out, retval]*/ BSTR *pVal);
  STDMETHOD(put_serverConfigPath)(/*[in]*/ BSTR newVal);
  STDMETHOD(get_serverMapPath)(/*[out, retval]*/ BSTR *pVal);
  STDMETHOD(put_serverMapPath)(/*[in]*/ BSTR newVal);
  STDMETHOD(get_mapName)(/*[out, retval]*/ BSTR *pVal);
  STDMETHOD(put_mapName)(/*[in]*/ BSTR newVal);

  HRESULT OnDraw(ATL_DRAWINFO& di);

  LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  LRESULT OnLoaderCallback(UINT uMsg, WPARAM status, LPARAM percentDone, BOOL& bHandled);

  LRESULT OnLButtonDown(UINT uMsg, WPARAM nFlags, LPARAM pt, BOOL& bHandled);
  LRESULT OnLButtonUp(UINT uMsg, WPARAM nFlags, LPARAM pt, BOOL& bHandled);
  LRESULT OnMouseMove(UINT uMsg, WPARAM nFlags, LPARAM pt, BOOL& bHandled);
  LRESULT OnRButtonDown(UINT uMsg, WPARAM nFlags, LPARAM pt, BOOL& bHandled);
  LRESULT OnRButtonUp(UINT uMsg, WPARAM nFlags, LPARAM pt, BOOL& bHandled);

  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    // TODO : Add Code for message handler. Call DefWindowProc if necessary.
    return 0;
  }

  LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
  static TSLLoaderCallbackReturn staticLoaderCallback( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone );

  void checkForCacheClear();

  static const char * m_mapLayerName ;

  std::string m_mapFileName;
  std::string m_serverMapPath;
  std::string m_serverConfigPath;

  std::string m_interactionModeName;

  std::string m_persistentCacheDirectory;
  long m_persistentCacheMaxSize;

  TSLNTSurface        * m_drawingSurface;
  TSLMapDataLayer     * m_mapDataLayer;
  TSLFileLoaderRemote * m_remoteFileLoader;

  // Current interaction mode
  TSLViewMode	      * m_currentMode ;

  // Instance of OS specific interaction mode interface
  TSLViewModeInterfaceNT * m_modeInterface ;

  short m_zoomPercentage;

  vector<string> m_errors;

  long m_memoryCacheSize;

  void getOSType();
  bool m_bIsWindows9X;

  HCURSOR m_hPanCursor;
  HCURSOR m_hZoomCursor;
  HCURSOR m_hGrabUpCursor;
  HCURSOR m_hGrabDownCursor;

};

#endif //__ACTIVEXCPP_H_
