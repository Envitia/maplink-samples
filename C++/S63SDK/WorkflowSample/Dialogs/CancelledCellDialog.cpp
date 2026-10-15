// IngestCell.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "CancelledCellDialog.h"


// IngestCell dialog

IMPLEMENT_DYNAMIC(CancelledCellDialog, CDialog)

CancelledCellDialog::CancelledCellDialog(const char * cellName, const char * dataServerID, CWnd* pParent /*=NULL*/)
	: CDialog(CancelledCellDialog::IDD, pParent)
        , m_cell(cellName)
        , m_dataServer(dataServerID)
        , m_dataServerID()
        , m_cellName()
{
  
}

CancelledCellDialog::~CancelledCellDialog()
{
  m_cell = 0;
  m_dataServer = 0;
}

void CancelledCellDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_CELL_NAME, m_cellName);
  DDX_Control(pDX, IDC_DATA_SERVER, m_dataServerID);
}

BOOL CancelledCellDialog::OnInitDialog()
{
   CDialog::OnInitDialog();

   m_cellName.SetWindowText((LPCTSTR)TSLUTF8Decoder(m_cell));
   m_dataServerID.SetWindowText((LPCTSTR)TSLUTF8Decoder(m_dataServer));

   return TRUE;
}

BEGIN_MESSAGE_MAP(CancelledCellDialog, CDialog)

END_MESSAGE_MAP()
