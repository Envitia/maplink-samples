// TerrainContourOptions.cpp : implementation file
//

#include "stdafx.h"
#include "TerrainContouringOptions.h"


// TerrainContourOptions dialog

IMPLEMENT_DYNAMIC(TerrainContouringOptions, CDialog)

TerrainContouringOptions::TerrainContouringOptions(CWnd* pParent /*=NULL*/, int currentWidth,
                                                   int currentHeight, int currentLevels)
	: CDialog(TerrainContouringOptions::IDD, pParent)
        , m_width( currentWidth )
        , m_height( currentHeight )
        , m_levels( currentLevels )
{
}

TerrainContouringOptions::~TerrainContouringOptions()
{
}

void TerrainContouringOptions::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRID_HEIGHT, m_gridHeight);
  DDX_Control(pDX, IDC_GRID_WIDTH, m_gridWidth);
  DDX_Control(pDX, IDC_NUM_LEVELS, m_numContourLevels);
}

int TerrainContouringOptions::getGridWidth()
{
  return m_width;
}

int TerrainContouringOptions::getGridHeight()
{
  return m_height;
}

int TerrainContouringOptions::getNumberOfContourLevels()
{
  return m_levels;
}

BEGIN_MESSAGE_MAP(TerrainContouringOptions, CDialog)
  ON_BN_CLICKED(IDOK, &TerrainContouringOptions::OnBnClickedOk)
//  ON_WM_CREATE()
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// TerrainContourOptions message handlers

void TerrainContouringOptions::OnBnClickedOk()
{
  // Attempt to convert the data entered by the user into numerical format

  // First convert the grid width
  CString enteredData;
  m_gridWidth.GetWindowText( enteredData );
  m_width = _ttoi( enteredData );

  if( m_width <= 0 )
  {
    // Non-numeric or bad width entered. Notify the user.
    AfxMessageBox( _T("Invalid grid width entered."), MB_ICONERROR );
    return;
  }

  // Then convert the grid height
  m_gridHeight.GetWindowText( enteredData );
  m_height = _ttoi( enteredData );

  if( m_height <= 0 )
  {
    // Non-numeric or bad width entered. Notify the user.
    AfxMessageBox( _T("Invalid grid height entered."), MB_ICONERROR );
    return;
  }

  // Finally do the number of contouring levels
  m_numContourLevels.GetWindowText( enteredData );
  m_levels = _ttoi( enteredData );

  if( m_levels <= 0 )
  {
    // Non-numeric or bad width entered. Notify the user.
    AfxMessageBox( _T("Invalid number of contour levels entered."), MB_ICONERROR );
    return;
  }

  OnOK();
}

void TerrainContouringOptions::OnShowWindow(BOOL bShow, UINT nStatus)
{
  CDialog::OnShowWindow(bShow, nStatus);

  // Set the contents of the edit controls to the current values for the width, height and number of
  // levels
  TCHAR formattedData[10];

  _sntprintf( formattedData, 10, _T("%i"), m_height );
  m_gridHeight.SetWindowText( formattedData );

  _sntprintf( formattedData, 10, _T("%i"), m_width );
  m_gridWidth.SetWindowText( formattedData );

  _sntprintf( formattedData, 10, _T("%i"), m_levels );
  m_numContourLevels.SetWindowText( formattedData );
}
