#pragma once
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

class CancelledCellDialog : public CDialog
{
  DECLARE_DYNAMIC(CancelledCellDialog)

public:

  explicit CancelledCellDialog(const char * cellName, const char * dataServerID, CWnd* pParent = NULL);   // standard constructor
  virtual ~CancelledCellDialog();

  //Overrides
  BOOL OnInitDialog();

  //ID
  enum { IDD = IDD_CANCELLED_CELL_DIALOG };

  //Dialog controls
  CStatic m_cellName;
  CStatic m_dataServerID;

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  const char * m_cell;
  const char * m_dataServer;

  CancelledCellDialog(); // not required

};
