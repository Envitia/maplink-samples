// LoadMedia.cpp : implementation file
//
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "../stdafx.h"
#include "../S63WorkFlowSample.h"
#include "../S63/s63workflow.h"

#include "ManageCellPermitsDialog.h"

IMPLEMENT_DYNAMIC(ManageCellPermitsDialog, CDialog)

ManageCellPermitsDialog::ManageCellPermitsDialog(S63Workflow * workFlow, CWnd* pParent /*=NULL*/)
	: CDialog(ManageCellPermitsDialog::IDD, pParent)
        , m_workFlow(workFlow)
        , m_cellPermitPath()
        , m_expiryLabel()
        , m_dataServerLabel()
        , m_cellNameLabel()
        , m_cellPermitInput()
        , m_dataServerInput()
        , m_cellPermitList()
{

}

ManageCellPermitsDialog::~ManageCellPermitsDialog()
{
  m_workFlow = 0;
}

BOOL ManageCellPermitsDialog::OnInitDialog()
{
  CDialog::OnInitDialog();

  updateCellPermits();

  return TRUE;
}

void ManageCellPermitsDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Text (pDX, IDC_CELL_PERMIT, m_cellPermitPath );
  DDX_Control(pDX, IDC_CELL_PERMIT, m_cellPermitInput);
  DDX_Control(pDX, IDC_DATA_STORE, m_dataServerInput);
  DDX_Control(pDX, IDC_PERMIT_LIST, m_cellPermitList);
  DDX_Control(pDX, IDC_EXPIRY, m_expiryLabel);
  DDX_Control(pDX, IDC_DATA_SERVER, m_dataServerLabel);
  DDX_Control(pDX, IDC_CELL_NAME, m_cellNameLabel);
}


BEGIN_MESSAGE_MAP(ManageCellPermitsDialog, CDialog)

  ON_BN_CLICKED(IDC_PERMIT_BROWSE, &ManageCellPermitsDialog::OnBnClickedPermitBrowse)

  ON_BN_CLICKED(IDC_ADD_PERMIT, &ManageCellPermitsDialog::OnBnClickedAddPermit)
  ON_LBN_SELCHANGE(IDC_PERMIT_LIST, &ManageCellPermitsDialog::OnLbnSelchangePermitList)
  ON_BN_CLICKED(IDC_REMOVE_PERMIT, &ManageCellPermitsDialog::OnBnClickedRemovePermit)
END_MESSAGE_MAP()

void ManageCellPermitsDialog::OnBnClickedPermitBrowse()
{
  CFileDialog FileDlg(true, _T("PERMIT.txt"), NULL, OFN_NOCHANGEDIR, _T("Cell Permits (PERMIT.txt)|PERMIT.txt|All Files (*.*)|*.*||"));
  FileDlg.m_pOFN->lpstrTitle = _T("Open PERMIT.TXT file");

  if( FileDlg.DoModal() != IDOK )
  {
    return;
  }
    
  m_cellPermitPath = FileDlg.GetPathName();

  UpdateData(FALSE);
}

void ManageCellPermitsDialog::OnBnClickedAddPermit()
{
  CString dataServerID;
  m_dataServerInput.GetWindowText(dataServerID);

  string permitPath = TSLUTF8Encoder(m_cellPermitPath);
  string dataServer = TSLUTF8Encoder(dataServerID);

  if ( !permitPath.empty() ) 
  {
    if ( !m_workFlow->installCellPermits(permitPath, dataServer) )
    {
      AfxMessageBox(_T("Unable to install cell permits. Please ensure the specified file is valid and that a data server ID has been set."), MB_ICONERROR ) ;
      return;
    }

    updateCellPermits();

    //AfxMessageBox(_T("Cell permits installed successfully."), MB_ICONINFORMATION ) ;

    m_cellPermitPath.Empty();

    UpdateData(FALSE);

  }
  else
  {
    AfxMessageBox(_T("Please select a cell permit file in the provided text box."), MB_ICONERROR ) ;
  }
}

void ManageCellPermitsDialog::updateCellPermits()
{
  vector< pair< const TSLS63CellPermit*, string> > cellPermits = m_workFlow->cellPermits();
  vector< pair< const TSLS63CellPermit*, string> >::const_iterator it;

  //clear the list
  m_cellPermitList.ResetContent();

  //for each cell permit
  for ( it = cellPermits.begin(); it != cellPermits.end() ; ++it )
  {
    string listItem = it->second;
    listItem.append(" - ");
    listItem.append(it->first->cellName());

    m_cellPermitList.AddString( (LPCTSTR)TSLUTF8Decoder(listItem) );
  }

}

void ManageCellPermitsDialog::listItemToString(const string &listItem, string &cellName, string &dataServerID) const
{
  const int delimiterLocation = listItem.find(" - ");
  cellName = listItem.substr(delimiterLocation + 3);
  dataServerID = listItem.substr(0, delimiterLocation);
}

void ManageCellPermitsDialog::OnLbnSelchangePermitList()
{
  string cellName("-");
  string dataServerID("-"); 
  long long expiryDate(0);

  int currentSelection;
  if ( (currentSelection = m_cellPermitList.GetCurSel()) != LB_ERR )
  {
    CString selectedItem;
    m_cellPermitList.GetText( currentSelection, selectedItem );
    string selectedItemUTF8 = TSLUTF8Encoder( selectedItem );
    listItemToString(selectedItemUTF8, cellName, dataServerID);

    m_workFlow->getCellExpiry(cellName, dataServerID, expiryDate);
  }

  if ( expiryDate )
  {
    CTime time(expiryDate) ;
    m_expiryLabel.SetWindowText(time.Format("%A, %B %d, %Y"));
  }
  else
  {
    m_expiryLabel.SetWindowText(_T("-"));
  }

  m_cellNameLabel.SetWindowText( (LPCTSTR)TSLUTF8Decoder(cellName) );
  m_dataServerLabel.SetWindowText( (LPCTSTR)TSLUTF8Decoder(dataServerID) );
}

void ManageCellPermitsDialog::OnBnClickedRemovePermit()
{
  int currentSelection;
  if ( (currentSelection = m_cellPermitList.GetCurSel()) != LB_ERR )
  {
    string cellName;
    string dataServerID;

    CString selectedItem;
    m_cellPermitList.GetText( currentSelection, selectedItem );
    string selectedItemUTF8 = TSLUTF8Encoder( selectedItem );
    listItemToString(selectedItemUTF8, cellName, dataServerID);

    m_workFlow->removeCellPermit(cellName, dataServerID);

    updateCellPermits();

    // Clear the information regarding the currently selected item.
    m_cellNameLabel.SetWindowText(_T("-"));
    m_dataServerLabel.SetWindowText(_T("-"));
    m_expiryLabel.SetWindowText(_T("-"));
  }
}
