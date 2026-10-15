#if !defined(AFX_AREADIALOG_H__AE4125C7_2D3C_46CA_A272_152248ECA9F8__INCLUDED_)
#define AFX_AREADIALOG_H__AE4125C7_2D3C_46CA_A272_152248ECA9F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AreaDialog.h : header file
//

class CGLScatterGraph;

/////////////////////////////////////////////////////////////////////////////
// AreaDialog dialog

class AreaDialog : public CDialog
{
  CGLScatterGraph*    m_graph;
  float*	      m_data;
  COLORREF*	      m_colours;
  int		      m_numData;
  int		      m_numLat;
  int		      m_numLon;
  // Construction
public:
  AreaDialog(CWnd* pParent = NULL);   // standard constructor
  ~AreaDialog();

  void setData( double minLat, double minLon, double maxLat, double maxLon, int numLat, int numLon, double* data );

  // Dialog Data
  //{{AFX_DATA(AreaDialog)
  enum { IDD = IDD_AREADIALOG };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(AreaDialog)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(AreaDialog)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AREADIALOG_H__AE4125C7_2D3C_46CA_A272_152248ECA9F8__INCLUDED_)
