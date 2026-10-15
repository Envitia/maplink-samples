#pragma once

#ifndef DeclutterDlg_
#define DeclutterDlg_

// DeclutterDlg dialog. Simple dialog to allow the app6a symbols
// to be decluttered by the drawing surface.

class DeclutterDlg : public CDialog
{

  DECLARE_DYNAMIC(DeclutterDlg)

public:

  DeclutterDlg(CWnd* pParent = NULL);   // standard constructor

  virtual ~DeclutterDlg();

// Dialog Data
//{{AFX_DATA(DeclutterDlg)
  enum { IDD = IDD_DECLUTTERDLG };
  
  BOOL m_hideText;
  BOOL m_hideGraphics;
  BOOL m_hideFrame;

  //}}AFX_DATA

protected:

//{{AFX_VIRTUAL(DeclutterDlg)
  virtual void OnOK() ;
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//}}AFX_VIRTUAL

  //{{AFX_MSG(DeclutterDlg)
//}}AFX_MSG

  DECLARE_MESSAGE_MAP()

};

#endif