// IngestCell.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "IngestCell.h"


// IngestCell dialog

IMPLEMENT_DYNAMIC(IngestCell, CDialog)

IngestCell::IngestCell(const char * cellName, const char * dataServerID, int edition, int update, const TSLGeodeticExtent &extent, CWnd* pParent /*=NULL*/)
	: CDialog(IngestCell::IDD, pParent)
        , m_cell(cellName)
        , m_dataServer(dataServerID)
        , m_cancel(false)
        , m_bl_x()
        , m_bl_y()
        , m_blx()
        , m_bly()
        , m_cellEdition()
        , m_cellName()
        , m_cellUpdate()
        , m_dataServerID()
        , m_edition()
        , m_tr_x()
        , m_tr_y()
        , m_trx()
        , m_try()
        , m_update()
{
  
  char editionStr[10];
  char updateStr[10];

  char tr_x[10];
  char tr_y[10];
  char bl_x[10];
  char bl_y[10];

  sprintf(tr_x, "%f", extent.longitudeTR());
  sprintf(tr_y, "%f", extent.latitudeTR());
  sprintf(bl_x, "%f", extent.longitudeBL());
  sprintf(bl_y, "%f", extent.latitudeBL());

  sprintf(editionStr, "%d", edition);
  sprintf(updateStr, "%d", update);

  m_edition = std::string(editionStr);
  m_update = std::string(updateStr);

  m_trx = std::string(tr_x);
  m_try = std::string(tr_y);
  m_blx = std::string(bl_x);
  m_bly = std::string(bl_y);
}

IngestCell::~IngestCell()
{
  m_cell = 0;
  m_dataServer = 0;
}

void IngestCell::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_CELL_NAME, m_cellName);
  DDX_Control(pDX, IDC_DATA_SERVER, m_dataServerID);
  DDX_Control(pDX, IDC_EDITION, m_cellEdition);
  DDX_Control(pDX, IDC_UPDATE, m_cellUpdate);

  DDX_Control(pDX, IDC_TR_X, m_tr_x);
  DDX_Control(pDX, IDC_TR_Y, m_tr_y);
  DDX_Control(pDX, IDC_BL_X, m_bl_x);
  DDX_Control(pDX, IDC_BL_Y, m_bl_y);
}

BOOL IngestCell::OnInitDialog()
{
   CDialog::OnInitDialog();

   m_cellName.SetWindowText( (LPCTSTR)TSLUTF8Decoder(m_cell) );
   m_dataServerID.SetWindowText( (LPCTSTR)TSLUTF8Decoder(m_dataServer) );
   m_cellEdition.SetWindowText( (LPCTSTR)TSLUTF8Decoder(m_edition) );
   m_cellUpdate.SetWindowText( (LPCTSTR)TSLUTF8Decoder(m_update) );

   m_tr_x.SetWindowText( (LPCTSTR)TSLUTF8Decoder(m_trx) );
   m_tr_y.SetWindowText( (LPCTSTR)TSLUTF8Decoder(m_try) );
   m_bl_x.SetWindowText( (LPCTSTR)TSLUTF8Decoder(m_blx) );
   m_bl_y.SetWindowText( (LPCTSTR)TSLUTF8Decoder(m_bly) );

   return TRUE;
}

BEGIN_MESSAGE_MAP(IngestCell, CDialog)

END_MESSAGE_MAP()