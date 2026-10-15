#pragma once

#include "resource.h"       // main symbols

// TerrainContourOptions dialog

class TerrainContouringOptions : public CDialog
{
  DECLARE_DYNAMIC(TerrainContouringOptions)

public:
  TerrainContouringOptions(CWnd* pParent = NULL, int currentWidth = 100, int currentHeight = 100,
    int currentLevels = 10);   // standard constructor
  virtual ~TerrainContouringOptions();

  int getGridWidth();
  int getGridHeight();
  int getNumberOfContourLevels();

  // Dialog Data
  enum { IDD = IDD_CONTOUR_CONTROL };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

  // MFC controls for retrieving user input
private:
  CEdit m_gridHeight;
  CEdit m_gridWidth;
  CEdit m_numContourLevels;

  // Numerical versions of the data entered by the user
  int m_height;
  int m_width;
  int m_levels;
public:
  afx_msg void OnBnClickedOk();
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
