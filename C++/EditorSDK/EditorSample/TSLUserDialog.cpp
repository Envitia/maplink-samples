// UserDialog.cpp : implementation file
//

#include "stdafx.h"
#include "EditorSample.h"
#include "TSLUserDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TSLUserDialog dialog


TSLUserDialog::TSLUserDialog(CWnd* pParent /*=NULL*/)
  : CDialog(TSLUserDialog::IDD, pParent)
{
  //{{AFX_DATA_INIT(TSLUserDialog)
    m_prompt = _T("");
  //}}AFX_DATA_INIT
}


void TSLUserDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(TSLUserDialog)
    DDX_Control(pDX, IDC_DIALOG_BUTTON4, m_button4);
    DDX_Control(pDX, IDC_DIALOG_BUTTON3, m_button3);
    DDX_Control(pDX, IDC_DIALOG_BUTTON2, m_button2);
    DDX_Control(pDX, IDC_DIALOG_BUTTON1, m_button1);
    DDX_Text(pDX, IDC_DIALOG_PROMPT, m_prompt);
    DDV_MaxChars(pDX, m_prompt, 9999999);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TSLUserDialog, CDialog)
  //{{AFX_MSG_MAP(TSLUserDialog)
  ON_BN_CLICKED(IDC_DIALOG_BUTTON1, OnDialogButton1)
  ON_BN_CLICKED(IDC_DIALOG_BUTTON2, OnDialogButton2)
  ON_BN_CLICKED(IDC_DIALOG_BUTTON3, OnDialogButton3)
  ON_BN_CLICKED(IDC_DIALOG_BUTTON4, OnDialogButton4)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TSLUserDialog message handlers

int TSLUserDialog::askForData(const std::string & prompt, const std::string & b1,
			    const std::string & b2, const std::string & b3, 
			    const std::string & b4 )
{
  m_prompt = prompt.c_str() ;

  lb1 = b1 ;
  lb2 = b2 ;
  lb3 = b3 ;
  lb4 = b4 ;

  INT_PTR pressed = DoModal() ;

  return pressed ;
}

BOOL TSLUserDialog::OnInitDialog() 
{
  
  CDialog::OnInitDialog();

  if ( lb1.size() )
    m_button1.SetWindowText( (LPCTSTR)TSLUTF8Decoder(lb1) ) ;
  else
    m_button1.ShowWindow( SW_HIDE ) ;

  if ( lb2.size() )
    m_button2.SetWindowText( (LPCTSTR)TSLUTF8Decoder(lb2) ) ;
  else
    m_button2.ShowWindow( SW_HIDE ) ;

  if ( lb3.size() )
    m_button3.SetWindowText( (LPCTSTR)TSLUTF8Decoder(lb3) ) ;
  else
    m_button3.ShowWindow( SW_HIDE ) ;

  if ( lb4.size() )
    m_button4.SetWindowText( (LPCTSTR)TSLUTF8Decoder(lb4) ) ;
  else
    m_button4.ShowWindow( SW_HIDE ) ;


  return TRUE;  // return TRUE unless you set the focus to a control
	        // EXCEPTION: OCX Property Pages should return FALSE
}

void TSLUserDialog::OnDialogButton1() 
{
  EndDialog( 101 ) ;	
}

void TSLUserDialog::OnDialogButton2() 
{
  EndDialog( 102 ) ;	
}

void TSLUserDialog::OnDialogButton3() 
{
  EndDialog( 103 ) ;	
}

void TSLUserDialog::OnDialogButton4() 
{
  EndDialog( 104 ) ;	
}
