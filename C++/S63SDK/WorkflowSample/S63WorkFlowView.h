// S63WorkFlowView.h : interface of the S63WorkFlowView class
//
/////////////////////////////////////////////////////////////////////////////
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#if !defined(AFX_S63WorkFlowVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_S63WorkFlowVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class S63Workflow;
class S63ErrorLayer;
class TSLS52DynamicRenderer;
class TSLS52NoDataLayer;
class S52StateObject;
class S52UserRenderingProcedure;
class S63WorkFlowDoc;
class ZoomScale;
class ScaleBand;

#include "dialogs/ProgressDialog.h"
#include "dialogs/TestProgressDialog.h"
#include "Dialogs/StateConfiguration.h"

class S63WorkFlowView : public CView, public DrawingSurfaceCallback, private TSLInteractionModeRequest, public TSLMapLinkErrorCallback
{
protected: // create from serialization only
	S63WorkFlowView();
	DECLARE_DYNCREATE(S63WorkFlowView)

// Attributes
public:
	S63WorkFlowDoc* GetDocument();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(S63WorkFlowView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

  // Worker thread message handler function
  void OnWorkerThreadMessage( WPARAM wParam, LPARAM lParam );

// Implementation
public:
	virtual ~S63WorkFlowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

  // Returns the current active view if it is of this View class.
  static S63WorkFlowView * GetView();

  // Tells the view to use the OpenGL 2D drawing surface instead of the NT drawing surface.
  // This is set by a command line flag on startup
  static void useOpenGLSurface(bool useGL)
  {
    m_useOpenGLSurface = useGL;
  }

  // load the S57 data store and enable S52 rendering
  void loadS57DataStore(const string &dataStore, const double latitude = 0.0, const double longitude = 0.0, const int scale = -1);

  S63Workflow * workFlow() const
  {
    return m_workFlow ;
  }
  void simpleInterface()
  {
    m_simpleInterface = true;
  }

  int cacheSize() const
  {
    return m_cacheSize;
  }
  void cacheSize(int cacheSize)
  {
    m_cacheSize = cacheSize;
  }
  void updateScale(CCmdUI *pCmdUI = NULL);

  // MapLink error stack callback
  bool error(TSLErrorCategory category, int errorCode, const char* errorNote, const char* additionalInformation);

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(S63WorkFlowView)
	afx_msg void OnToolsZoomin();
	afx_msg void OnToolsZoomout();
	afx_msg void OnToolsReset();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateToolsMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsAction(CCmdUI* pCmdUI);
	afx_msg void OnToolsPan();
	afx_msg void OnToolsZoom();
	afx_msg void OnEditCopy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnToolsGrab();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	//}}AFX_MSG
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
        afx_msg void OnUpdateCurrentScale(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()

private:

  // Store the WorkFlow class
  S63Workflow * m_workFlow ;

  // Store the error handling layer
  vector< S63ErrorLayer *> m_s63ErrorLayers;
  vector< TSLCustomDataLayer *> m_s63CustomErrorLayers;

  // Store a pointer to the current s63 layer
  TSLS63DataLayer * m_s63Layer;
  
  // Store a pointer to the current s63 layer
  TSLS52NoDataLayer * m_s52NoDataLayer;

  // Store a pointer to the current s52 dynamic renderer
  TSLS52DynamicRenderer * m_s52DynamicRenderer;
    
  // Store a pointer to the current s63 layer
  S52StateObject * m_s52StateObject;
  
  // Handle to drawing surface
  TSLDrawingSurface * m_drawingSurface ;

  // Interaction manager
  TSLInteractionModeManagerGeneric * m_modeManager ;

  // Flag to say whether valid map is loaded.
  S52UserRenderingProcedure *m_userRenderingProcedure;

  bool m_mapLoaded ;

  // current scale 1 : muPerScreenMetre
  int m_muPerScreenMetre;

  // Flag to say whether valid S63 is loaded.
  bool m_s63Loaded ;

  // Flag to say whether to enable S52 rendering.
  bool m_enableS52 ;

  // Stored extent to print, captured in OnFilePrint and OnFilePrintPreview
  TSLEnvelope  m_extentToPrint ;

  //Configuration Values
  std::string m_s57ConfigFile;
  std::string m_logFile;
  std::string m_unlockKey;
  std::string m_hardwareID;
  bool m_hardwareIDChanged;
  int m_cacheSize;
  long long m_configurationTime;

  // Set as a static so it can be referenced from the NotifyError callback
  static bool m_disableWarnings;

  // Controls whether the viewer will use the OpenGL drawing surface, or the NT surface
  static bool m_useOpenGLSurface;

  // Flag to set whether or not to display the 'Ingest Cell' dialog.
  bool m_displayIngestDialog;

  //Pointer to combo box storing the zoom setting
  ZoomScale *m_zoomScalePtr;

  //
  ScaleBand *m_scaleBandPtr;

  // ENC MapLink Viewer - simple interface.
  bool m_simpleInterface;

  // scale band locking option
  int m_scaleBandOption;

  //TUT Values
  std::string m_dataLocation;

  std::string m_tut_resultsDir;
  std::string m_tut_configDir;

  long long m_tutTime;
    
  //helpers
  void installCertificate(bool dataServerCertificate);

  void displayS63(const string& dataStore, vector< pair< const TSLS63CellPermit*, string > > &cellPermits);
  void displayS57(const string& dataStore, const double latitude = 0.0, const double longitude = 0.0, const int scale = -1);

  // does the work for displayS63/displayS57
  void display(const string& dataStore, const double latitude, const double longitude, const int scale, vector< pair< const TSLS63CellPermit*, string > > &cellPermits, bool isS63);

  void setScale(int scale, bool updateReadout = true);

  void enableS52Rendering(bool  enableS52);
  void loadS52Rendering();
  void loadS63Rendering();
  bool updateScaleBands(int id);


  //callbacks
  static bool cancelledCellCallback(const char* cellName, const char* dataServerID, const char ** cellReplacements, int numberOfCells);
  static bool catalogueCallback(string &catalogueLocation);
  static void notifyErrorCallback(int s63ErrorCode, const char * cellName, const char * dataServerID, const char* additionalInformation, void * arg);
  static bool ingestCellCallback(const TSLSimpleString& cellName, const char* dataServerID, const TSLGeodeticExtent& cellExtent, int cellEdition, int cellUpdateNumber);
  static bool progressCallback(double percentOfExchangeSetDone, double percentOfCurrentCellDone, void *arg);

  //progress dialog for media ingest
  static ProgressDialog *m_ingestProgressDialog;

  //progress dialog for IHO tests
  static TestProgressDialog m_testProgressDialog;

  UINT_PTR m_nTimerID;

public:
  afx_msg void OnToolsMagnify();
  afx_msg void OnZoomToData();

  void resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU);
  void viewChanged(TSLDrawingSurface* drawingSurface);
  afx_msg void OnSavedviewsGoto1();
  afx_msg void OnSavedviewsGoto2();
  afx_msg void OnSavedviewsGoto3();
  afx_msg void OnSavedviewsReset();
  afx_msg void OnSavedviewsSet1();
  afx_msg void OnUpdateSavedviewsReset(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsGoto3(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsGoto2(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsGoto1(CCmdUI *pCmdUI);
  afx_msg void OnUpdateSavedviewsSet1(CCmdUI *pCmdUI);
  afx_msg void OnSavedviewsSet2();
  afx_msg void OnUpdateSavedviewsSet2(CCmdUI *pCmdUI);
  afx_msg void OnSavedviewsSet3();
  afx_msg void OnUpdateSavedviewsSet3(CCmdUI *pCmdUI);
  afx_msg void OnViewstackNextview();
  afx_msg void OnUpdateViewstackNextview(CCmdUI *pCmdUI);
  afx_msg void OnViewstackPreviousview();
  afx_msg void OnUpdateViewstackPreviousview(CCmdUI *pCmdUI);
  afx_msg void OnUpdateTestSetup(CCmdUI *pCmdUI);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

  // S63 Mandated Tests
  afx_msg void OnS63TestSetup();
  afx_msg void OnS63TestRunEdition_1_1();
  afx_msg void OnS63TestRunEdition_1_2();

  // Configuration Dialog 
  afx_msg void OnConfiguration();

  // State Configuration Dialog 
  afx_msg void OnStateConfiguration();

  // User Permits
  afx_msg void OnGenerateUserPermit();
  afx_msg void OnUpdateUserPermit(CCmdUI *pCmdUI);

  // Certificates
  afx_msg void OnInstallSACertificate();
  afx_msg void OnInstallDSCertificate();
  afx_msg void OnRemoveSACertificate();
  afx_msg void OnRemoveDSCertificate();

  afx_msg void OnUpdateInstallSACertificate(CCmdUI *pCmdUI);
  afx_msg void OnUpdateInstallDSCertificate(CCmdUI *pCmdUI);
  afx_msg void OnUpdateRemoveSACertificate(CCmdUI *pCmdUI);
  afx_msg void OnUpdateRemoveDSCertificate(CCmdUI *pCmdUI);

  // Media Ingest
  afx_msg void OnInstallUserPermit();
  afx_msg void OnInstallCellPermit();
  afx_msg void OnLoadS57Media();
  afx_msg void OnLoadMedia();
  afx_msg void OnLoadLargeMedia();

  afx_msg void OnUpdateLoadMedia(CCmdUI *pCmdUI);

  //Load S63
  afx_msg void OnLoadS63();
  afx_msg void OnLoadS57();
  afx_msg void OnConfigureScaleBands();

  // Lat/Long grid layer
  afx_msg void OnViewShowLat();
  afx_msg void OnUpdateViewShowLat(CCmdUI *pCmdUI);

  //MGRS grid layer
  afx_msg void OnViewShowMGRS();
  afx_msg void OnUpdateViewShowMGRS(CCmdUI *pCmdUI);

  //Generic
  afx_msg void OnUpdateNeverEnabled(CCmdUI *pCmdUI);
  afx_msg void OnUpdateAlwaysEnabled(CCmdUI *pCmdUI);
  afx_msg void OnUpdateViewerEnabled(CCmdUI *pCmdUI);
  afx_msg void OnUpdateConfigurationDialog(CCmdUI *pCmdUI);
  virtual BOOL PreTranslateMessage(MSG* pMsg);

  afx_msg void OnViewScaleBandFixDefault();
  afx_msg void OnViewScaleBandFixIgnore();
  afx_msg void OnViewScaleBandFixOverview();
  afx_msg void OnViewScaleBandFixGeneral();
  afx_msg void OnViewScaleBandFixCoastal();
  afx_msg void OnViewScaleBandFixApproach();
  afx_msg void OnViewScaleBandFixHarbour();
  afx_msg void OnViewScaleBandFixBerthing();

  afx_msg void OnViewScaleBandFixUpdate(CCmdUI *pCmdUI);
  
  afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
  afx_msg void OnRemoveS57Stores();

  // callbacks from state control
  virtual void noDataLayerVisible(bool toggle);
  virtual void setupDynamicColours(string file);
};

#ifndef _DEBUG  // debug version in S63WorkFlowView.cpp
inline S63WorkFlowDoc* S63WorkFlowView::GetDocument()
   { return (S63WorkFlowDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_S63WorkFlowVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
