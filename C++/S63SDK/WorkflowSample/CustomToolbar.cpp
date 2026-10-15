// CustomToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "CustomToolbar.h"


// CCustomToolbar

IMPLEMENT_DYNAMIC(CCustomToolbar, CToolBar)

CCustomToolbar::CCustomToolbar()
{
  m_font.CreatePointFont( 80,_T("MS Sans Serif") );
}

CCustomToolbar::~CCustomToolbar()
{
}

bool CCustomToolbar::createComboBox( int toolbarID, int comboID )
{
  // Only create one combo box
  if ( ::IsWindow(m_zoomCombo.m_hWnd) )
       return false;

  // Create the combo box - make it larger so we can read the text
  int nIdx = CommandToIndex(toolbarID);
  ASSERT( nIdx >= 0 );
  SetButtonInfo(nIdx, comboID, TBBS_SEPARATOR, 150);
  const int nDropHeight = 100;
  CRect rect;
  GetItemRect(nIdx, &rect);
  rect.top = 1;
  rect.bottom = rect.top + nDropHeight;
  rect.right = rect.left + 150;

  if (!m_zoomCombo.Create(CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_TABSTOP | WS_CHILD | WS_VSCROLL, rect, this, comboID))
  {
      return false;
  }

  // update the combo with a font.
  m_zoomCombo.SetFont( &m_font );

  m_zoomCombo.ShowWindow( SW_SHOWNORMAL );

  return true;
}

bool CCustomToolbar::createScaleBandComboBox( int toolbarID, int comboID )
{
  // Only create one combo box
  if ( ::IsWindow(m_scaleBandCombo.m_hWnd) )
       return false;

  // Create the combo box - make it larger so we can read the text
  int nIdx = CommandToIndex(toolbarID);
  ASSERT( nIdx >= 0 );
  SetButtonInfo(nIdx, comboID, TBBS_SEPARATOR, 250);
  const int nDropHeight = 100;
  CRect rect;
  GetItemRect(nIdx, &rect);
  rect.top = 1;
  rect.bottom = rect.top + nDropHeight;
  rect.right = rect.left + 250;

  if (!m_scaleBandCombo.Create(CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_TABSTOP | WS_CHILD | WS_VSCROLL, rect, this, comboID))
  {
      return false;
  }

  // update the combo with a font.

  m_scaleBandCombo.SetFont( &m_font );
  m_scaleBandCombo.SetCurSel(0);

  m_scaleBandCombo.ShowWindow( SW_SHOWNORMAL );

  return true;
}

BEGIN_MESSAGE_MAP(CCustomToolbar, CToolBar)
END_MESSAGE_MAP()



// CCustomToolbar message handlers
