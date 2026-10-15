// IngestCell.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "ProgressDialog.h"


// IngestCell dialog

IMPLEMENT_DYNAMIC(ProgressDialog, CDialog)

ProgressDialog::ProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(ProgressDialog::IDD, pParent)
  , m_cancelled( false )
  , m_ingestProgress()
  , m_cellProgress()
{
}

ProgressDialog::~ProgressDialog()
{

}

void ProgressDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_INGEST_PROGRESS, m_ingestProgress);
  DDX_Control(pDX, IDC_CELL_PROGRESS, m_cellProgress);
}

BEGIN_MESSAGE_MAP(ProgressDialog, CDialog)
  ON_BN_CLICKED(IDCANCEL, &ProgressDialog::OnBnClickedCancel)
//  ON_WM_CREATE()
END_MESSAGE_MAP()

void ProgressDialog::setProgress( double percentOfExchangeSetDone, double percentOfCurrentCellDone )
{
  m_ingestProgress.SetPos( (int)percentOfExchangeSetDone );
  m_cellProgress.SetPos( (int)percentOfCurrentCellDone );
}

bool ProgressDialog::hasBeenCancelled() const
{
  return m_cancelled;
}

void ProgressDialog::OnBnClickedCancel()
{
  m_cancelled = true;

  // Disable the cancel button to acknowledge the abort instruction.
  // The process using this dialog will abort at the earliest opportunity.
  GetDlgItem( IDCANCEL )->EnableWindow( FALSE );
}

BOOL ProgressDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  // The dialog shows the percentage of progress for each cell and the total
  // progress so far for an ingest. Therefore set the valid range for the
  // progress bars from 0-100%.
  m_ingestProgress.SetRange32( 0, 100 );
  m_cellProgress.SetRange32( 0, 100 );
  m_ingestProgress.SetPos( 0 );
  m_cellProgress.SetPos( 0 );

  m_cancelled = false;
  GetDlgItem( IDCANCEL )->EnableWindow( TRUE );

  return TRUE;
}
