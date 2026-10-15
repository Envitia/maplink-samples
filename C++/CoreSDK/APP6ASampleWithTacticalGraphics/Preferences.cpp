// Preferences.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "Preferences.h"
#include "OpMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog


CPreferences::CPreferences()
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CPreferences)
	m_angularValue = 0.0;
	m_distanceValue = 0.0;
	m_cursorReadoutType = 0;
	//}}AFX_DATA_INIT
}


void CPreferences::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferences)
	DDX_Control(pDX, IDC_CURSOR_READOUT_TYPE, m_cursorReadoutControl);
	DDX_Text(pDX, IDC_ANGULAR_VALUE, m_angularValue);
	DDX_Text(pDX, IDC_DISTANCE_VALUE, m_distanceValue);
	DDX_CBIndex(pDX, IDC_CURSOR_READOUT_TYPE, m_cursorReadoutType);
	//}}AFX_DATA_MAP
}

bool CPreferences::initialiseAndCreate( CFrameWnd * frame )
{
  if ( !Create( frame, IDD_PREFERENCES, WS_CHILD |CBRS_LEFT, IDD_PREFERENCES ) )
    return false ;
  
  SetWindowText( _T("Preferences") );
  EnableDocking( CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT );
  
  frame->DockControlBar ( this, CBRS_ALIGN_LEFT );

  return true;
}

LRESULT CPreferences::OnInitDialog( WPARAM wParam, LPARAM lParam )
{  
  HandleInitDialog(wParam, lParam);

  if (!UpdateData(FALSE))
  {
    TRACE0( "Warning: UpdateData failed during preferences dialog init.\n" ) ;
    return FALSE ;
  }

  m_cursorReadoutControl.AddString( _T("TMC Absolute" ) );
  m_cursorReadoutControl.AddString( _T("TMC Relative" ) );
  m_cursorReadoutControl.AddString( _T("MU Absolute" ) );
  m_cursorReadoutControl.AddString( _T("MU Relative" ) );
  m_cursorReadoutControl.AddString( _T("UU Absolute" ) );
  m_cursorReadoutControl.AddString( _T("UU Relative" ) );
  m_cursorReadoutControl.AddString( _T("Lat/Long Decimal" ) );
  m_cursorReadoutControl.AddString( _T("Lat/Long Angular" ) );
  m_cursorReadoutControl.AddString( _T("Angle/TMC Distance" ) );
  m_cursorReadoutControl.AddString( _T("Bearing/TMC Distance" ) );
  m_cursorReadoutControl.AddString( _T("Angle/MU Distance" ) );
  m_cursorReadoutControl.AddString( _T("Bearing/MU Distance" ) );
  m_cursorReadoutControl.AddString( _T("Angle/UU Distance" ) );
  m_cursorReadoutControl.AddString( _T("Bearing/UU Distance" ) );

  m_cursorReadoutControl.SetCurSel( 0 ) ;

  OnSelchangeCursorReadoutType() ;
  
  return TRUE;
}

void CPreferences::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
  UpdateDialogControls(pTarget, FALSE);
} 


BEGIN_MESSAGE_MAP(CPreferences, CDialogBar)
	//{{AFX_MSG_MAP(CPreferences)
	ON_WM_DRAWITEM()
	ON_EN_CHANGE(IDC_ANGULAR_VALUE, OnChangeAngularValue)
	ON_EN_CHANGE(IDC_DISTANCE_VALUE, OnChangeDistanceValue)
	ON_CBN_SELCHANGE(IDC_CURSOR_READOUT_TYPE, OnSelchangeCursorReadoutType)
	//}}AFX_MSG_MAP
ON_MESSAGE( WM_INITDIALOG, OnInitDialog )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreferences message handlers

void CPreferences::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
  CDC dc;
  CRect highlight = lpDrawItemStruct->rcItem ;
  highlight.DeflateRect( 1, 1, 0, 0 ) ;
  
  dc.Attach(lpDrawItemStruct->hDC);
  dc.SetBkMode( TRANSPARENT ) ;
  
  if ( lpDrawItemStruct->itemState & ODS_SELECTED )
  {
    CPen pen( 0, 2, RGB( 0, 0, 0 ) ) ;
    dc.SelectObject( pen ) ;
    dc.SelectObject( ::GetStockObject( NULL_BRUSH ) ) ;
    dc.Rectangle( &highlight ) ;
  }
  
  dc.Detach();
}

void CPreferences::OnChangeAngularValue() 
{
  UpdateData(TRUE) ;	
  COpMgr::angular( ID_CONSTRAINT_ANGULAR, m_angularValue ) ;
}

void CPreferences::OnChangeDistanceValue() 
{
  UpdateData(TRUE) ;	
  COpMgr::distance( ID_CONSTRAINT_DISTANCE, m_distanceValue ) ;
}

void CPreferences::OnSelchangeCursorReadoutType() 
{
  UpdateData(TRUE) ;

  switch ( m_cursorReadoutType )
  {
    case 0 :
      COpMgr::cursorReadoutType( TSLCursorMovementTMCAbsolute, "Pos : X %.0f   Y %.0f" ) ;
      break ;

    case 1 :
      COpMgr::cursorReadoutType( TSLCursorMovementTMCRelative, "Pos : X %.0f   Y %.0f" ) ;
      break ;

    case 2 :
      COpMgr::cursorReadoutType( TSLCursorMovementMUAbsolute, "Pos : X %.0f   Y %.0f" ) ;
      break ;

    case 3 :
      COpMgr::cursorReadoutType( TSLCursorMovementMURelative, "Pos : X %.0f   Y %.0f" ) ;
      break ;

    case 4 :
      COpMgr::cursorReadoutType( TSLCursorMovementUUAbsolute, "Pos : X %.0f   Y %.0f" ) ;
      break ;

    case 5 :
      COpMgr::cursorReadoutType( TSLCursorMovementUURelative, "Pos : X %.0f   Y %.0f" ) ;
      break ;

    case 6 :
      COpMgr::cursorReadoutType( TSLCursorMovementLatLongDecimal, "Pos : Lat %.3f   Long %.3f" ) ;
      break ;

    case 7 :
      COpMgr::cursorReadoutType( TSLCursorMovementLatLongAngular, "Pos : Lat %s   Long %s" ) ;
      break ;

    case 8 :
      COpMgr::cursorReadoutType( TSLCursorMovementAngleTMCDistance, "Pos : Angle %3.3f   Distance %.0f" ) ;
      break ;

    case 9 :
      COpMgr::cursorReadoutType( TSLCursorMovementBearingTMCDistance, "Pos : Brg %3.3f   Distance %.0f" ) ;
      break ;

    case 10 :
      COpMgr::cursorReadoutType( TSLCursorMovementAngleMUDistance, "Pos : Angle %3.3f   Distance %.0f" ) ;
      break ;

    case 11 :
      COpMgr::cursorReadoutType( TSLCursorMovementBearingMUDistance, "Pos : Brg %3.3f   Distance %.0f" ) ;
      break ;

    case 12 :
      COpMgr::cursorReadoutType( TSLCursorMovementAngleUUDistance, "Pos : Angle %3.3f   Distance %.0f" ) ;
      break ;

    case 13 :
      COpMgr::cursorReadoutType( TSLCursorMovementBearingUUDistance, "Pos : Brg %3.3f   Distance %.0f" ) ;
      break ;
  }	
}
