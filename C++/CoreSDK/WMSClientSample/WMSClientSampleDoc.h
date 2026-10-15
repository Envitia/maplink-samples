// WMSClientSampleDoc.h : interface of the WMSClientSampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WMSClientSampleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_WMSClientSampleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WMSSettingsCallbacks.h"
#include "WMTSSettingsCallbacks.h"

struct NamedDataLayer
{
  TSLDataLayer* m_dataLayer;
  string m_name;
  int m_creationOrder;

  static int m_totalLayersCreated;

public:
  NamedDataLayer(TSLDataLayer* dataLayer, const string& name)
    : m_dataLayer(dataLayer)
    , m_name(name)
  {
    m_creationOrder = ++m_totalLayersCreated;
  }

  NamedDataLayer(TSLMapDataLayer* dataLayer)
    : m_dataLayer(dataLayer)
    , m_name("Map")
    , m_creationOrder(0)   // only one of these
  {
  }

  string genMapLinkLayerName() const
  {
    TSLDataLayerTypeEnum type = m_dataLayer->layerType();
    if (type == TSLDataLayerTypeMapDataLayer)   // only one of these
      return m_name;

    const char* layerTypeStr = type==TSLDataLayerTypeWMSDataLayer  ? "WMS layer" :
                               type==TSLDataLayerTypeWMTSDataLayer ? "WMTS layer" :
                               "Layer";
    char name[256];
    sprintf_s( name, "%s #%i - %.220s", layerTypeStr, m_creationOrder, m_name.c_str() );
    return name;
  }

  TSLDataLayer* dataLayer()
  {
    return m_dataLayer;
  }
};

class WMSClientSampleDoc : public CDocument
{
  protected: // create from serialization only
    WMSClientSampleDoc();
    DECLARE_DYNCREATE(WMSClientSampleDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    //This method adds the set of data layers contained 
    //within the document to the specified drawing surface.
    bool addToSurface( TSLDrawingSurface * surface );
  
    //This method removes the current set of data layers from 
    //the specified drawing surface.
    bool detachFromSurface( TSLDrawingSurface * surface );
  
    static void AllTilesLoadedCallback (void* arg2);
    static TSLLoaderCallbackReturn TileLoaderCallback( void* arg, const char* filename, const TSLEnvelope extent, TSLLoaderStatus status, int percentDone );
    void newTileAvailable();

    void refreshAllSettings();

    void openWMSLayer();
    void openWMTSLayer();
    NamedDataLayer* addWMSLayer();
    NamedDataLayer* addWMTSLayer();
    NamedDataLayer* addMapLayer();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(WMSClientSampleDoc)
  public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
    //}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~WMSClientSampleDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(WMSClientSampleDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:
    void setupLinearTransformParameters(TSLDataLayer *layer);
    void refreshLayerSettings(TSLDataLayer* layer);

    // The data layer containing the map
    NamedDataLayer* m_mapDataLayer ;

    // The data layers containing the WMS/WMTS data
    vector<NamedDataLayer> m_dataLayers;

    WMSSettingsCallbacks m_wmsCallbacks;
    WMTSSettingsCallbacks m_wmtsCallbacks;

    TSLFileLoaderRemote * m_remoteLoader;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WMSClientSampleDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
