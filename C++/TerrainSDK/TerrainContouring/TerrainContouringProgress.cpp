// TerrainContouringProgress.cpp : implementation file
//

#include "stdafx.h"
#include "TerrainContouringProgress.h"

// TerrainContouringProgress dialog

IMPLEMENT_DYNAMIC(TerrainContouringProgress, CDialog)

TerrainContouringProgress::TerrainContouringProgress(CWnd* pParent /*=NULL*/)
	: CDialog(TerrainContouringProgress::IDD, pParent)
{
}

TerrainContouringProgress::~TerrainContouringProgress()
{
}

void TerrainContouringProgress::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_PROGRESS_BAR, m_progressBar);
  DDX_Control(pDX, IDC_PROGRESS_LABEL, m_progressLabel);
}


BEGIN_MESSAGE_MAP(TerrainContouringProgress, CDialog)
END_MESSAGE_MAP()

void TerrainContouringProgress::setNumberOfContourLevels( int number )
{
  m_progressBar.SetRange( 0, number );
}

void TerrainContouringProgress::setCompletedLevels( int number )
{
  m_progressBar.SetPos( number );
}

void TerrainContouringProgress::setOperationProgress( int soFar, int total )
{
  std::stringstream progress;
  progress << "Processed " << soFar << " of " << total;
  m_progressLabel.SetWindowText( (LPCTSTR)TSLUTF8Decoder( progress.str().c_str() ) );
}
