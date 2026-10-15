// TextEntry.cpp : implementation file
//

#include "stdafx.h"
#include "EditorSample.h"
#include "TSLTextEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TSLTextEntry dialog


TSLTextEntry::TSLTextEntry(CWnd* pParent /*=NULL*/)
	: CDialog(TSLTextEntry::IDD, pParent)
{
  //{{AFX_DATA_INIT(TSLTextEntry)
    m_prompt = _T("");
    m_value = _T("");
  //}}AFX_DATA_INIT
}


void TSLTextEntry::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(TSLTextEntry)
    DDX_Text(pDX, IDC_TEXT_ENTRY_PROMPT, m_prompt);
    DDV_MaxChars(pDX, m_prompt, 999999);
    DDX_Text(pDX, IDC_TEXT_ENTRY_VALUE, m_value);
    DDV_MaxChars(pDX, m_value, 999999);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TSLTextEntry, CDialog)
  //{{AFX_MSG_MAP(TSLTextEntry)
  // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TSLTextEntry message handlers

int TSLTextEntry::askForData(const std::string & prompt, std::string & value)
{
  m_value = (LPCTSTR)TSLUTF8Decoder(value) ;
  m_prompt = (LPCTSTR)TSLUTF8Decoder(prompt) ;

  INT_PTR pressed = DoModal() ;

  value = TSLUTF8Encoder(m_value) ;
  return ( pressed == IDCANCEL || pressed == IDABORT ) ;
}
