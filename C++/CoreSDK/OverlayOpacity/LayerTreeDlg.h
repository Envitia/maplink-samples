#pragma once

class COverlayOpacityDoc;

// CLayerTreeDlg dialog

class CLayerTreeDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CLayerTreeDlg)

public:
	CLayerTreeDlg();   // standard constructor
	virtual ~CLayerTreeDlg();

// Dialog Data
	enum { IDD = IDD_LAYER_LIST };

  BOOL Create( CWnd* pParentWnd, LPCTSTR lpszTemplateName,
                     UINT nStyle, UINT nID, BOOL = TRUE);

  BOOL Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle,
                     UINT nID, BOOL = TRUE);

  void UpdateTree ( COverlayOpacityDoc * doc, TSLDrawingSurface * surface );
  afx_msg void OnTvnSelchangedMapTree(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

        COverlayOpacityDoc * m_doc;

private:

  string m_mapName;
  
};
