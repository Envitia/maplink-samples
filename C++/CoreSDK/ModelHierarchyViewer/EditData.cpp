// EditData.cpp : implementation file
//

#include "stdafx.h"
#include "ModelHierarchyViewer.h"
#include "EditData.h"
#include <afxdialogex.h>


// EditData dialog

IMPLEMENT_DYNAMIC(EditData, CDialog)

EditData::EditData(CWnd* pParent /*=NULL*/)
	: CDialog(EditData::IDD, pParent)
{

}

EditData::~EditData()
{
}  

void EditData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_STATIC1, static_value);
  DDX_Control(pDX, IDC_STATIC2, static_rotation);
  DDX_Control(pDX, IDC_STATIC3, static_height);

  DDX_Control(pDX, IDC_EDIT1, edit_value);
  DDX_Control(pDX, IDC_EDIT2, edit_rotation);
  DDX_Control(pDX, IDC_EDIT3, edit_height);
}


BEGIN_MESSAGE_MAP(EditData, CDialog)
  ON_BN_CLICKED(IDOK, &EditData::OnBnClickedOk)
END_MESSAGE_MAP()


// EditData message handlers


BOOL EditData::OnInitDialog()
{
  CDialog::OnInitDialog();

  editControls( );

  return TRUE; 
}

void EditData::OnBnClickedOk()
{
  CString value, rotation, height;
  edit_value.GetWindowText(value);
  edit_rotation.GetWindowText(rotation);
  edit_height.GetWindowText(height);
  m_textEntity->value(value);
  m_textEntity->rotation(_wtof(rotation));
  m_textEntity->height(_wtol(height));
  CDialog::OnOK();
}

template<class T>
static CString convertToString( const T &value)
{
  std::stringstream strs;
  strs << value;
  return strs.str().c_str();
}

void EditData::editControls( )
{
   const char*   value    = m_textEntity->value();
   double   rotation      = m_textEntity->rotation();
   TSLTMC   height        = m_textEntity->height();
   static_value.SetWindowText(_T("Value:"));
   edit_value.SetWindowText((CString)value);
   static_rotation.SetWindowText(_T("Rotation:"));
   edit_rotation.SetWindowText( convertToString(rotation) );
   static_height.SetWindowText(_T("Height:"));
   edit_height.SetWindowText( convertToString(height) );
}