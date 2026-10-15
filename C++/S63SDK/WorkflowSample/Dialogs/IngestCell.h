#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

////////////////////////////////////////////////
////      TEST PLAN REFERENCE: 3.4.2.3      ////
////////////////////////////////////////////////

class IngestCell : public CDialog
{
  DECLARE_DYNAMIC(IngestCell)

public:

  IngestCell(const char * cellName, const char * dataServerID, int edition, int update, const TSLGeodeticExtent &extent, CWnd* pParent = NULL);   // standard constructor
  virtual ~IngestCell();

  //Overrides
  BOOL OnInitDialog();

  //ID
  enum { IDD = IDD_INGEST_DIALOG };

  //Dialog controls
  CStatic m_cellName;
  CStatic m_dataServerID;
  CStatic m_cellEdition;
  CStatic m_cellUpdate;

  CStatic m_tr_x;
  CStatic m_tr_y;
  CStatic m_bl_x;
  CStatic m_bl_y;

  bool m_cancel;

  // Events
  afx_msg void OnBnClickedCancelIngest();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  IngestCell(); // not required

  const char * m_cell;
  const char * m_dataServer;

  std::string m_edition;
  std::string m_update;

  std::string m_trx;
  std::string m_try;
  std::string m_blx;
  std::string m_bly;

};
