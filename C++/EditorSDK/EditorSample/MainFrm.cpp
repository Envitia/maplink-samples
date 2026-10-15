// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"

#include "EditorSample.h"
#include "EditorSampleView.h"

#include "MainFrm.h"
#include "OpMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CEditorSampleApp theApp ;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
  //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_COMMAND(ID_PRIMITIVES_POLYGON, OnPrimitivesPolygon)
    ON_COMMAND(ID_CONSTRAINT_EQUAL, OnConstraintEqual)
    ON_COMMAND(ID_CONSTRAINT_HORIZONTAL, OnConstraintHorizontal)
    ON_COMMAND(ID_CONSTRAINT_UNEQUAL, OnConstraintUnequal)
    ON_COMMAND(ID_CONSTRAINT_VERTICAL, OnConstraintVertical)
    ON_COMMAND(ID_EDIT_BACKUP, OnEditBackup)
    ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
    ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
    ON_COMMAND(ID_PRIMITIVES_CHANGETEXT, OnPrimitivesChangetext)
    ON_COMMAND(ID_PRIMITIVES_CIRCLE, OnPrimitivesCircle)
    ON_COMMAND(ID_PRIMITIVES_MARKER, OnPrimitivesMarker)
    ON_COMMAND(ID_PRIMITIVES_POLYLINE, OnPrimitivesPolyline)
    ON_COMMAND(ID_PRIMITIVES_TEXT, OnPrimitivesText)
    ON_COMMAND(ID_SELECT_SELECTALL, OnSelectSelectall)
    ON_COMMAND(ID_SELECT_SELECTBYPOLYGON, OnSelectSelectbypolygon)
    ON_COMMAND(ID_SELECT_SELECTBYRECTANGLE, OnSelectSelectbyrectangle)
    ON_COMMAND(ID_TRANSFORMATION_ADDPOINT, OnTransformationAddpoint)
    ON_COMMAND(ID_TRANSFORMATION_BACK, OnTransformationBack)
    ON_COMMAND(ID_TRANSFORMATION_DELETEPOINT, OnTransformationDeletepoint)
    ON_COMMAND(ID_TRANSFORMATION_FRONT, OnTransformationFront)
    ON_COMMAND(ID_TRANSFORMATION_GROUP, OnTransformationGroup)
    ON_COMMAND(ID_TRANSFORMATION_MOVE, OnTransformationMove)
    ON_COMMAND(ID_TRANSFORMATION_MOVEPOINT, OnTransformationMovepoint)
    ON_COMMAND(ID_TRANSFORMATION_ROTATE, OnTransformationRotate)
    ON_COMMAND(ID_TRANSFORMATION_SCALE, OnTransformationScale)
    ON_COMMAND(ID_TRANSFORMATION_UNGROUP, OnTransformationUngroup)
	ON_COMMAND(ID_VIEW_ATTRIBUTE_PANEL, OnViewAttributePanel)
    ON_COMMAND(ID_VIEW_PREFERENCES, OnViewPreferences)
	ON_COMMAND(ID_VIEW_TOOLBARS_MAIN, OnViewToolbarsMain)
	ON_COMMAND(ID_VIEW_TOOLBARS_TOOLS, OnViewToolbarsTools)
	ON_COMMAND(ID_VIEW_TOOLBARS_POINT_OPERATIONS, OnViewToolbarsPointOperations)
	ON_COMMAND(ID_VIEW_TOOLBARS_PRIMITIVES, OnViewToolbarsPrimitives)
	ON_COMMAND(ID_VIEW_TOOLBARS_SELECT, OnViewToolbarsSelect)
	ON_COMMAND(ID_VIEW_TOOLBARS_TRANSFORMATIONS, OnViewToolbarsTransformations)
	ON_COMMAND(ID_VIEW_TOOLBARS_EDIT, OnViewToolbarsEdit)
	ON_COMMAND(ID_FILE_SAVELAYOUT, OnFileSavelayout)
	ON_COMMAND(ID_VIEW_TOOLBARS_CONSTRAINTS, OnViewToolbarsConstraint)
	ON_COMMAND(ID_CONSTRAINT_ANGULAR, OnConstraintAngular)
	ON_COMMAND(ID_CONSTRAINT_DISTANCE, OnConstraintDistance)
	//}}AFX_MSG_MAP
  // Do this here, since Class Wizard doesn't support ranges
  ON_UPDATE_COMMAND_UI_RANGE( ID_EDIT_BACKUP, ID_TOOLS_LENGTH, OnUpdateOperations )
  ON_UPDATE_COMMAND_UI_RANGE( ID_CONSTRAINT_EQUAL, ID_CONSTRAINT_DISTANCE,   OnUpdateConstraints)
  ON_UPDATE_COMMAND_UI_RANGE( ID_VIEW_TOOLBARS_CONSTRUCTION, ID_VIEW_TOOLBARS_CONSTRAINTS, OnUpdateViewToolbars)
  ON_UPDATE_COMMAND_UI( ID_NUM_SELECTED, OnUpdateSelected )
  ON_UPDATE_COMMAND_UI( ID_MOUSE_POS, OnUpdateMousePos )

END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_MOUSE_POS,
  ID_NUM_SELECTED
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
  char layoutFile[ 1024 ] ;
  TSLProfileHelper::setDefaultSection( "paths" ) ;
  TSLProfileHelper::lookupProfile( "layoutFile", layoutFile, sizeof( layoutFile ), "layout.dat" ) ;
  m_layoutFile = theApp.resolveConfigPath( layoutFile ) ;
      }

CMainFrame::~CMainFrame()
{
}

bool CMainFrame::createToolBar(CToolBar *pToolBar,UINT resourceID, UINT titleID, UINT menuID)
{
  if (   !pToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_TOP, CRect(6,3,6,3), resourceID) 
      || !pToolBar->LoadToolBar(resourceID))
  {
    TRACE0("Failed to create toolbar\n");
    return false;      // fail to create
  }

  CString title;
  title.LoadString(titleID);
  pToolBar->SetWindowText(title);
  pToolBar->SetBarStyle( pToolBar->GetBarStyle() |
      CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC );

  pToolBar->EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(pToolBar);

  m_toolBarsMap.insert( make_pair(menuID, pToolBar) );

  return true;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;
 
  EnableDocking(CBRS_ALIGN_ANY);

  // initiliase the toolbars
  createToolBar( &m_wndToolBar,		      IDR_MAINFRAME, IDS_MAIN_TOOLBAR, ID_VIEW_TOOLBARS_MAIN);
  createToolBar( &m_wndEditToolBar,	      IDR_EDIT, IDS_EDIT_TOOLBAR, ID_VIEW_TOOLBARS_EDIT);
  createToolBar( &m_wndPointOperationsToolBar,IDR_POINT_OPERATIONS, IDS_POINT_OPERATIONS_TOOLBAR, ID_VIEW_TOOLBARS_POINT_OPERATIONS);
  createToolBar( &m_wndPrimitivesToolBar,     IDR_PRIMITIVES, IDS_PRIMITIVES_TOOLBAR, ID_VIEW_TOOLBARS_PRIMITIVES);
  createToolBar( &m_wndSelectToolBar,	      IDR_SELECT, IDS_SELECT_TOOLBAR, ID_VIEW_TOOLBARS_SELECT);
  createToolBar( &m_wndTransformationsToolBar,IDR_TRANSFORMATIONS, IDS_TRANSFORMATIONS_TOOLBAR, ID_VIEW_TOOLBARS_TRANSFORMATIONS);
  createToolBar( &m_wndToolsToolBar,	      IDR_TOOLS, IDS_TOOLS_TOOLBAR, ID_VIEW_TOOLBARS_TOOLS );
  createToolBar( &m_wndConstraintToolBar,     IDR_CONSTRAINTS, IDS_CONSTRAINT_TOOLBAR, ID_VIEW_TOOLBARS_CONSTRAINTS );

  if (   !m_wndStatusBar.Create(this)
      || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }

  // Setup the status bars
  m_wndStatusBar.SetPaneInfo( 1, ID_MOUSE_POS,    SBPS_NORMAL, 325 ) ;
  m_wndStatusBar.SetPaneInfo( 2, ID_NUM_SELECTED, SBPS_NORMAL, 120 ) ;
 
  if ( !m_attributePanel.initialiseAndCreate( this ) )
  {
    TRACE0("Failed to create attribute panel\n");
    return -1;
  }

  if ( !m_preferencesPanel.initialiseAndCreate( this ) )
  {
    TRACE0("Failed to create preferences panel\n");
    return -1;
  }

  // put the parent of the attribute panel to the bottom of the zorder so that the status bar 
  // reaches all the way across the frame. The DockBars have fixed ID's
  CWnd *wnd = GetDlgItem(AFX_IDW_DOCKBAR_LEFT);
  if( wnd )
    wnd->SetWindowPos(&m_wndStatusBar,0,0,0,0,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);

  wnd = GetDlgItem(AFX_IDW_DOCKBAR_RIGHT);
  if( wnd )
    wnd->SetWindowPos(&m_wndStatusBar,0,0,0,0,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);

  wnd = GetDlgItem(AFX_IDW_DOCKBAR_BOTTOM);
  if( wnd )
    wnd->SetWindowPos(&m_wndStatusBar,0,0,0,0,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);

  TSLProfileHelper::setDefaultSection( "initialAttributes" ) ;

  SetDockState( m_dockState ) ;

  return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  CFile myFile;
  CFileException fileException;

  if ( !myFile.Open( (LPCTSTR)TSLUTF8Decoder(m_layoutFile), CFile::modeRead, &fileException ) )
  {
    TRACE( "Can't open layout file %s, error = %u\n",
      m_layoutFile.c_str(), fileException.m_cause );
  }
  else
  {
    CArchive ar( &myFile, CArchive::load ) ;
    CWordArray placement ;

    placement.Serialize( ar ) ;
    m_dockState.Serialize( ar ) ;

    // Read the show command
    theApp.m_nCmdShow = placement[0] ;

    RECT rect ;

    rect.left   = placement[1] ;
    rect.bottom = placement[2] ;
    rect.top    = placement[3] ;
    rect.right  = placement[4] ;

    cs.cx = rect.right  - rect.left ;
    cs.cy = rect.bottom - rect.top  ;
    cs.x  = rect.left ;
    cs.y  = rect.top ;
  }
  
  return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
  CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
  CFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnUpdateConstraints(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( COpMgr::queryAllowEdit() ) ;
  pCmdUI->SetCheck( COpMgr::queryConstraint() == pCmdUI->m_nID ) ;
}

void CMainFrame::OnUpdateOperations(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( COpMgr::queryAllowEdit()) ;
  pCmdUI->SetCheck( COpMgr::queryActiveOperation() == pCmdUI->m_nID ) ;
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
  m_cmdID = nID ;	
  return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnPrimitivesPolygon() 
{
  COpMgr::polygon( m_cmdID ) ;
}

void CMainFrame::OnUpdateMousePos( CCmdUI * pCmdUI )
{
  pCmdUI->Enable( ) ;
  pCmdUI->SetText( (LPCTSTR)TSLUTF8Decoder(COpMgr::queryCursorReadout()) ) ;
}

void CMainFrame::OnUpdateSelected( CCmdUI * pCmdUI )
{
  int num, depth ;
  num = COpMgr::querySelected( depth ) ;
  
  TCHAR buffer[ 256 ] ;
  if ( depth )
    _stprintf( buffer, _T("Select (%d) : %d"), depth, num ) ;
  else
    _stprintf( buffer, _T("Select : %d"), num ) ;

  pCmdUI->Enable( ) ;
  pCmdUI->SetText( buffer ) ;

  CEditorSampleView * view = (CEditorSampleView *)GetActiveView( ) ;

  if ( !view->IsKindOf(RUNTIME_CLASS(CEditorSampleView)) )
  {
    // Print preview changes the active view class, so ignore this
    m_wndStatusBar.SetWindowText( _T("") ) ;
  }
  else
  {
    if ( COpMgr::queryAllowEdit() )
      m_wndStatusBar.SetWindowText( (LPCTSTR)TSLUTF8Decoder(view->queryPrompt()) ) ;
    else
      m_wndStatusBar.SetWindowText( _T("") ) ;
  }
}

void CMainFrame::OnConstraintEqual() 
{
  COpMgr::equal( m_cmdID ) ;
}

void CMainFrame::OnConstraintHorizontal() 
{
  COpMgr::horizontal( m_cmdID ) ;
}

void CMainFrame::OnConstraintUnequal() 
{
  COpMgr::unequal( m_cmdID ) ;
}

void CMainFrame::OnConstraintVertical() 
{
  COpMgr::vertical( m_cmdID ) ;
}

void CMainFrame::OnEditBackup() 
{
  COpMgr::backup( ) ;
}

void CMainFrame::OnEditClear() 
{
  COpMgr::clear( m_cmdID ) ;
}

void CMainFrame::OnEditCopy() 
{
  COpMgr::copy( m_cmdID ) ;
}

void CMainFrame::OnEditDelete() 
{
  COpMgr::deleteOp( m_cmdID ) ;
}

void CMainFrame::OnViewPreferences() 
{
  ShowControlBar( &m_preferencesPanel, m_preferencesPanel.IsWindowVisible() ? false : true, false  );
  RecalcLayout();	
}

void CMainFrame::OnEditUndo() 
{
  COpMgr::undo( ) ;
}

void CMainFrame::OnPrimitivesChangetext() 
{
  COpMgr::setText( m_cmdID ) ;
}

void CMainFrame::OnPrimitivesCircle() 
{
  COpMgr::circle( m_cmdID ) ;
}

void CMainFrame::OnPrimitivesMarker() 
{
  COpMgr::symbol( m_cmdID ) ;
}

void CMainFrame::OnPrimitivesPolyline() 
{
  COpMgr::polyline( m_cmdID ) ;
}

void CMainFrame::OnPrimitivesText() 
{
  COpMgr::text( m_cmdID ) ;
}

void CMainFrame::OnSelectSelectall() 
{
  COpMgr::selectAll( m_cmdID ) ;
}

void CMainFrame::OnSelectSelectbypolygon() 
{
  COpMgr::selectByPolygon( m_cmdID ) ;
}

void CMainFrame::OnSelectSelectbyrectangle() 
{
  COpMgr::selectByExtent( m_cmdID ) ;
}

void CMainFrame::OnTransformationAddpoint() 
{
  COpMgr::addPoint( m_cmdID ) ;
}

void CMainFrame::OnTransformationBack() 
{
  COpMgr::back( m_cmdID ) ;
}

void CMainFrame::OnTransformationDeletepoint() 
{
  COpMgr::deletePoint( m_cmdID ) ;
}

void CMainFrame::OnTransformationFront() 
{
  COpMgr::front( m_cmdID ) ;
}

void CMainFrame::OnTransformationGroup() 
{
  COpMgr::group( m_cmdID ) ;
}

void CMainFrame::OnTransformationMove() 
{
  COpMgr::move( m_cmdID ) ;
}

void CMainFrame::OnTransformationMovepoint() 
{
  COpMgr::movePoint( m_cmdID ) ;
}

void CMainFrame::OnTransformationRotate() 
{
  COpMgr::rotate( m_cmdID ) ;
}

void CMainFrame::OnTransformationScale() 
{
  COpMgr::scale( m_cmdID ) ;
}

void CMainFrame::OnTransformationUngroup() 
{
  COpMgr::ungroup( m_cmdID ) ;
}

void CMainFrame::OnViewAttributePanel() 
{
  ShowControlBar( &m_attributePanel, 
    m_attributePanel.IsWindowVisible() ? false : true, false  );
  RecalcLayout();	
}

void CMainFrame::OnViewToolbarsMain() 
{
  toggleToolBar(&m_wndToolBar);
}

void CMainFrame::OnViewToolbarsTools() 
{
  toggleToolBar( &m_wndToolsToolBar );
}

void CMainFrame::OnViewToolbarsPointOperations() 
{
  toggleToolBar( &m_wndPointOperationsToolBar);
}

void CMainFrame::OnViewToolbarsEdit() 
{
  toggleToolBar( &m_wndEditToolBar);
}

void CMainFrame::OnViewToolbarsPrimitives() 
{
  toggleToolBar( &m_wndPrimitivesToolBar);
}

void CMainFrame::OnViewToolbarsSelect() 
{
  toggleToolBar( &m_wndSelectToolBar);
}

void CMainFrame::OnViewToolbarsTransformations() 
{
  toggleToolBar( &m_wndTransformationsToolBar);
}

void CMainFrame::toggleToolBar( CToolBar *pToolBar )
{
  ShowControlBar( pToolBar, pToolBar->IsWindowVisible() ? false : true, false  );
  RecalcLayout();	
}

void CMainFrame::OnUpdateViewToolbars(CCmdUI* pCmdUI) 
{
  ToolBarsMap::iterator it = m_toolBarsMap.find( pCmdUI->m_nID );
  if( it != m_toolBarsMap.end() )
    pCmdUI->SetCheck( (*it).second->IsWindowVisible() ? 1 : 0 );
}

void CMainFrame::OnFileSavelayout() 
{
  CDockState state ;
  CWordArray placement ;

  WINDOWPLACEMENT windowplacement ;

  GetWindowPlacement( &windowplacement ) ;

  placement.Add( windowplacement.showCmd ) ;
  placement.Add( windowplacement.rcNormalPosition.left ) ;
  placement.Add( windowplacement.rcNormalPosition.bottom ) ;
  placement.Add( windowplacement.rcNormalPosition.top ) ;
  placement.Add( windowplacement.rcNormalPosition.right ) ;

  GetDockState( state ) ;

  CFile myFile;
  CFileException fileException;

  if ( !myFile.Open( (LPCTSTR)TSLUTF8Decoder(m_layoutFile), CFile::modeCreate |   
	    CFile::modeReadWrite , &fileException ) )
  {
    TCHAR buffer[ 1024 ] ;

    _stprintf( buffer, _T("Can't open layout file %s, is it readonly ?\n"),
	     m_layoutFile.c_str() );

    AfxMessageBox( buffer ) ;

    return ;
  }

  CArchive ar( &myFile, CArchive::store ) ;

  placement.Serialize( ar ) ;
  state.Serialize( ar ) ;
}

void CMainFrame::OnViewToolbarsConstraint() 
{
  toggleToolBar(&m_wndConstraintToolBar);
}

void CMainFrame::OnConstraintAngular() 
{
  COpMgr::angular( m_cmdID, this->m_preferencesPanel.m_angularValue ) ;
}

void CMainFrame::OnConstraintDistance() 
{
  COpMgr::distance( m_cmdID, this->m_preferencesPanel.m_distanceValue ) ;
}
