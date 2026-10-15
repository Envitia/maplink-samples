// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "attributepanel.h"
#include "preferences.h"

//This component is the main frame window for the  
//application.  It maintains the various control bars 
//and provides many of the menu and toolbar handlers. 
//
//The actual functionality driving the editor and maintaining 
//attribute and parameter information is held within the 
//COpMgr class.  This provides a layer of separation from the MMI.
class CMainFrame : public CFrameWnd
{
private:
	//This attribute is the name of the file used to save and 
	//load the toolbar layout.
	 std::string   m_layoutFile;

	 // This attribute holds the docking bar status
	 CDockState    m_dockState ;

	//This attribute contains the ID of the last menu handler 
	//to invoke a command.  It is used in the operation 
	//invocation handlers, to pass the current active 
	//operation ID to the COpMgr class.  This 
	//stores it and allows a single UI handler to be used to 
	//update the status of all operation menubar entries 
	//using an ON_UPDATE_COMMAND_UI_RANGE macro.
	int m_cmdID ;

protected: // create from serialization only
	//Contructor for main frame window.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	//This method overrides the default frame window 
	//implementation, to allow for user positioning of the 
	//window.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//This method overrides the default OnCmdMsg handler to 
	//store the ID of the last menu handler to invoke a 
	//command.  It is used in the operation invocation 
	//handlers, to pass the current active operation ID to 
	//the COpMgr class.  This stores it and allows 
	//a single UI handler to be used to update the status of 
	//all operation menubar entries using an 
	//ON_UPDATE_COMMAND_UI_RANGE macro.
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	//Destructor for frame window.
	virtual ~CMainFrame();
#ifdef _DEBUG
	//Standard MFC debug method.
	virtual void AssertValid() const;
	//Standard MFC debug method.
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	//The Status Bar instantiation.
	CStatusBar  m_wndStatusBar;
	//The default dockable Tool Bar instantiation.
	CToolBar    m_wndToolBar;

	//The dockable Tools Tool Bar instantiation.
	CToolBar m_wndToolsToolBar;
	CToolBar m_wndConstraintToolBar;
	CToolBar m_wndPointOperationsToolBar;
	CToolBar m_wndTransformationsToolBar;
	CToolBar m_wndEditToolBar;
	CToolBar m_wndPrimitivesToolBar;
	CToolBar m_wndSelectToolBar;

	typedef std::map<UINT,CToolBar*> ToolBarsMap;
	//A map from menu item ID to CToolBar* used by the 
	//OnUpdateViewToolbars handler to update the toolbar menu 
	//items
	ToolBarsMap m_toolBarsMap;

	//The dockable Attribute Panel Dialog Bar instantiation.
	CAttributePanel      m_attributePanel ;

	//The dockable Preferences Panel Dialog Bar instantiation
	CPreferences      m_preferencesPanel ;

	//This method is a helper function used by the OnCreate 
	//method to create all the toolbars
	//
	//Argument list description:
	//
	//pToolBar: A pointer to the member variable of the 
	//toolbar to create
	//
	//resourceID: Resource identifier of the toolbar resource 
	//to use for this toolbar.
	//
	//titleID: Resource identifier of the string to use for 
	//the title of this toolbar.
	//
	//menuID: ID of the menu item that will toggle the 
	//visibility of this toolbar
	bool createToolBar(CToolBar *pToolBar,UINT resourceID, UINT titleID, UINT menuID);
	//This method is a helper function used by the View | 
	//Toolbars sub-menu menu item handlers.
	//
	//It toggles the visibility of the specified tool bar and 
	//then recalculates the layout of all the applications 
	//control bars.
	//
	//Argument list description:
	//
	//pToolBar: A pointer to the toolbar to toggle the 
	//visibility of
	void toggleToolBar( CToolBar *pToolBar );

// Generated message map functions
protected:

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPrimitivesPolygon();
	afx_msg void OnConstraintEqual();
	afx_msg void OnConstraintHorizontal();
	afx_msg void OnConstraintUnequal();
	afx_msg void OnConstraintVertical();
	afx_msg void OnEditBackup();
	afx_msg void OnEditClear();
	afx_msg void OnEditCopy();
	afx_msg void OnEditDelete();
	afx_msg void OnEditUndo();
	afx_msg void OnPrimitivesChangetext();
	afx_msg void OnPrimitivesCircle();
	afx_msg void OnPrimitivesMarker();
	afx_msg void OnPrimitivesPolyline();
	afx_msg void OnPrimitivesText();
	afx_msg void OnSelectSelectall();
	afx_msg void OnSelectSelectbypolygon();
	afx_msg void OnSelectSelectbyrectangle();
	afx_msg void OnTransformationAddpoint();
	afx_msg void OnTransformationBack();
	afx_msg void OnTransformationDeletepoint();
	afx_msg void OnTransformationFront();
	afx_msg void OnTransformationGroup();
	afx_msg void OnTransformationMove();
	afx_msg void OnTransformationMovepoint();
	afx_msg void OnTransformationRotate();
	afx_msg void OnTransformationScale();
	afx_msg void OnTransformationUngroup();
	afx_msg void OnViewAttributePanel();
	afx_msg void OnViewPreferences();
	afx_msg void OnViewToolbarsMain();
	afx_msg void OnViewToolbarsTools();
	afx_msg void OnViewToolbarsPointOperations();
	afx_msg void OnViewToolbarsPrimitives();
	afx_msg void OnViewToolbarsSelect();
	afx_msg void OnViewToolbarsTransformations();
	afx_msg void OnUpdateViewToolbars(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarsEdit();
	afx_msg void OnFileSavelayout();
	afx_msg void OnViewToolbarsConstraint();
	afx_msg void OnConstraintAngular();
	afx_msg void OnConstraintDistance();
	//}}AFX_MSG
	//Common update handler for all operation invocation 
	//options.
	//
	//This queries the current operation id from 
	//COpMgr class to determine whether the 
	//specified menu item should be ticked.
	afx_msg void OnUpdateOperations(CCmdUI* pCmdUI);

	//Common update handler for all constraints.
	//
	//This queries the current constraint from 
	//COpMgr class to determine whether the 
	//specified menu item should be ticked.
	afx_msg void OnUpdateConstraints(CCmdUI* pCmdUI);

	//Update handler for the status bar.
	//
	//This method ensures that the prompt and select list 
	//status are displayed in the status bar.
	afx_msg void OnUpdateSelected( CCmdUI * pCmdUI ) ;

	//This method ensures that the cursor readout 
	//status is displayed in the status bar.
	afx_msg void OnUpdateMousePos( CCmdUI * pCmdUI ) ;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnApp6aPalette();
  afx_msg void OnApp6aProperties();
  afx_msg void OnApp6aSelector();
  virtual BOOL DestroyWindow();
  afx_msg void OnFriendyGroundmainaxis();
  afx_msg void OnAxisFriendlygroundsupportingattack();
  afx_msg void OnAxisFriendlyplannedattack();
  afx_msg void OnAxisFeint();
  afx_msg void OnAxisCounterattack();
  afx_msg void OnAxisEnemyconfirmed();
  afx_msg void On3pointattacksAttackbyfireposition();
  afx_msg void On3pointattacksSurpriseattack();
  afx_msg void OnAxisEnemysuspected();
  afx_msg void OnDirectionAttackforfeint();
  afx_msg void On3Supportbyfireposition();
  afx_msg void OnFollowFollowandassume();
  afx_msg void OnFollowFollowandsupport();
  afx_msg void OnTacticalgraphicsContain();
  afx_msg void OnTacticalgraphicsDisrupt();
  afx_msg void OnBypassBypasseasy();
  afx_msg void OnBypassBypassdifficult();
  afx_msg void OnBypassBypassimpossible();
  afx_msg void OnConvoyMoving();
  afx_msg void OnConvoyHalted();
  afx_msg void OnDirectionFriendlymainattack();
  afx_msg void OnDirectionFriendlysupportingattack();
  afx_msg void OnDirectionFriendlyplannedattack();
  afx_msg void OnTacticalgraphicsBlock();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__CF7BD0CA_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
