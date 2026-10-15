////////////////////////////////////////////////////////
// Copyright (c) 1998 to 2017 by Envitia Group PLC
////////////////////////////////////////////////////////
// Simple3DDirectImportDoc.h : interface of the Simple3DDirectImportDoc class
//
/////////////////////////////////////////////////////////////////////////////

#include "WMSSettingsCallbacks.h"

#if !defined(AFX_Simple3DDirectImportDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_Simple3DDirectImportDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class Simple3DDirectImportDoc : public CDocument
{
protected: // create from serialization only
  Simple3DDirectImportDoc();
  DECLARE_DYNCREATE(Simple3DDirectImportDoc)

  // Attributes
public:

  bool m_newMap ;
  string m_mapName ;
  TSL3DWinGLSurface * m_surface;

  bool m_cadrgTransparent ;
  bool m_wmsTransparent ;
  string m_onTop ;

  // Operations
public:

  void loadMap() ;

  //This method adds the set of data layers contained 
  //within the document to the specified drawing surface.
  bool addToSurface( TSL3DWinGLSurface * surface );

  //This method removes the current set of data layers from 
  //the specified drawing surface.
  bool detachFromSurface( TSLDrawingSurfaceBase * surface );

  static void AllTilesLoadedCallback (void* arg2);
  static TSLLoaderCallbackReturn TileLoaderCallback( void* arg, const char* filename, const TSLEnvelope extent, TSLLoaderStatus status, int percentDone );

  void surface(TSL3DWinGLSurface * drawingSurface) ;
  void SetTopLayerAndTransparencies() ;

  void newTileAvailable();
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(Simple3DDirectImportDoc)
public:
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName) ;
  virtual BOOL OnOpenWMSDocument(LPCTSTR lpszPathName) ;
  void OnFileLoadcadrgdata() ;
  void setupCadrgRendering() ;
  long getIDOfNearestColour( char * buffer ) ;
  virtual void DeleteContents();
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~Simple3DDirectImportDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

  // Generated message map functions
protected:
  //{{AFX_MSG(Simple3DDirectImportDoc)
  // NOTE - the ClassWizard will add and remove member functions here.
  //    DO NOT EDIT what you see in these blocks of generated code !
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

private:

  // The data layer containing the map
  TSLMapDataLayer * m_mapDataLayer ;

  // Terrain
  TSL3DTerrainDataLayer * m_terrainDataLayer ;

  // 3D Standard Data-layer
  TSL3DStandardDataLayer * m_stdDataLayer;

  // CADRG
  TSLCADRGDataLayer * m_cadrgDataLayer;

  // WMS
  WMSSettingsCallbacks * m_callbacks;
  TSLWMSDataLayer * m_wmsDataLayer ;

  // Name of my map layer
  static const char * m_mapLayerName ;
  static const char * m_terrainLayerName ;
  static const char * m_stdLayerName ;
  static const char * m_cadrgLayerName ;
  static const char * m_wmsLayerName ;

public:
  afx_msg void OnFileOpenterrain();
  afx_msg void OnCreatePolygon();
  afx_msg void OnViewCadrgontop();
  afx_msg void OnUpdateViewCadrgontop(CCmdUI *pCmdUI);
  afx_msg void OnViewWmsontop();
  afx_msg void OnUpdateViewWmsontop(CCmdUI *pCmdUI);
  afx_msg void OnViewCadrgtransparent();
  afx_msg void OnUpdateViewCadrgtransparent(CCmdUI *pCmdUI);
  afx_msg void OnViewWmstransparent();
  afx_msg void OnUpdateViewWmstransparent(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Simple3DDirectImportDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
