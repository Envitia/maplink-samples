#pragma once


// CLayerProperties dialog

#include <string>
using namespace std;

class CLayerProperties : public CDialogBar
{
	DECLARE_DYNAMIC(CLayerProperties)

public:
	CLayerProperties();   // standard constructor
	virtual ~CLayerProperties();

// Dialog Data
	enum { IDD = IDD_LAYER_PROPERTIES };

  BOOL Create( CWnd* pParentWnd, LPCTSTR lpszTemplateName,
                     UINT nStyle, UINT nID, BOOL = TRUE);

  BOOL Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle,
                     UINT nID, BOOL = TRUE);

  void Update(string * layerName, TSLDrawingSurface* surface);


  virtual LRESULT OnInitDialog(WPARAM wParam, LPARAM lParam);
  afx_msg void OnBnClickedLayerBuffered();
  afx_msg void OnBnClickedLayerVisible();


protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  DECLARE_MESSAGE_MAP()
  
private:

  CButton m_layerBuffered;
  CButton m_layerVisible;
  CSliderCtrl m_opacitySlider;

  TSLDrawingSurface * m_surface;
  string * m_selectedLayer;
  
public:
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
