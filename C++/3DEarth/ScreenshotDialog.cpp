// ScreenshotDialog.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ScreenshotDialog.h"
#include "afxdialogex.h"


// ScreenshotDialog dialog

IMPLEMENT_DYNAMIC(ScreenshotDialog, CDialogEx)

ScreenshotDialog::ScreenshotDialog(CWnd* pParent /*=NULL*/)
  : CDialogEx(IDD_SCREENSHOT, pParent)
{

}

ScreenshotDialog::~ScreenshotDialog()
{
}

void ScreenshotDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ScreenshotDialog, CDialogEx)
END_MESSAGE_MAP()


// ScreenshotDialog message handlers
