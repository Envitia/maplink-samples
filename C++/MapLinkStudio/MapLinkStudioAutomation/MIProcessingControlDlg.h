#if !defined(AFX_MIPROCESSINGCONTROLDLG_H__C16DC320_98DC_4384_8FFB_F68FBC508777__INCLUDED_)
#define AFX_MIPROCESSINGCONTROLDLG_H__C16DC320_98DC_4384_8FFB_F68FBC508777__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MIProcessingControlDlg.h : header file
//

class CMapLinkStudioClientView;
class IMapLinkStudio;
/////////////////////////////////////////////////////////////////////////////
// CMIProcessingControlDlg dialog

class CMIProcessingControlDlg : public CDialog
{
  // Construction
public:
  CMIProcessingControlDlg(CMapLinkStudioClientView* view);   // standard constructor

  void StartMaplink();

  void SetCoordSystemName(string coordSystemName);

  // Dialog Data
  //{{AFX_DATA(CMIProcessingControlDlg)
  enum { IDD = IDD_MAP_PROC_DIALOG };
  CStatic	m_status;
  CButton	m_bAdvanced;
  CButton	m_cornersLatLong;
  CButton m_cornersCurrent;
  CButton	m_bRemove;
  CButton	m_bEdit;
  CButton	m_outputBrowse;
  CButton	m_bGenerate;
  CStatic	m_template;
  CStatic	m_source;
  CEdit	m_ap_output_dir;
  CEdit	m_map_tr_lon;
  CEdit	m_map_tr_lat;
  CEdit	m_map_bl_lon;
  CEdit	m_map_bl_lat;
  CListBox	m_mi_data_list;
  CString	m_coordSystem;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMIProcessingControlDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CMIProcessingControlDlg)
  afx_msg void OnGenerateMap();
  afx_msg void OnMiAdvanced();
  afx_msg void OnMiAddData();
  afx_msg void OnMiEditData();
  afx_msg void OnMiRemoveData();
  afx_msg void OnMiDirBrowser();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnSelchangeMiDataList();
  afx_msg void OnMiDataCornersCurrx();
  afx_msg void OnMiDataCornersLatlon();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  string m_dataset;

  string getLayerName(void);

  vector<string> libraryNames(void);

  vector<string> resolutionNames(void);

  string m_layerName;

  datasetInfo m_datasetInfo;

  vector <datafileInfo> m_datafileInfo;

  CMapLinkStudioClientView* m_view;

  IMapLinkStudio * m_maplinkStudio ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIPROCESSINGCONTROLDLG_H__C16DC320_98DC_4384_8FFB_F68FBC508777__INCLUDED_)
