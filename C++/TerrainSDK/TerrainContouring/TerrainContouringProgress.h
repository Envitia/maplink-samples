#pragma once

// TerrainContouringProgress dialog

#include "resource.h"       // main symbols


class TerrainContouringProgress : public CDialog
{
  DECLARE_DYNAMIC(TerrainContouringProgress)

public:
  TerrainContouringProgress(CWnd* pParent = NULL);   // standard constructor
  virtual ~TerrainContouringProgress();

  // Sets the target number of levels, and thus how to control the progress bar
  void setNumberOfContourLevels( int number );

  // Sets how many complete contour levels have been processed
  void setCompletedLevels( int number );

  // Updates the label on the dialog with the state of contouring for the current operation
  void setOperationProgress( int soFar, int total );

  // Dialog Data
  enum { IDD = IDD_PROGRESS_DIALOG };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:
  // MFC control that shows total progress for this contour operation
  CProgressCtrl m_progressBar;
  CStatic m_progressLabel;
};
