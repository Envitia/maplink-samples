#if !defined(AFX_ATTRIBUTEPANEL_H__20FF785F_FFD1_11D3_9284_00C04FA1187D__INCLUDED_)
#define AFX_ATTRIBUTEPANEL_H__20FF785F_FFD1_11D3_9284_00C04FA1187D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// AttributePanel.h : header file
//

//Dockable Attribute Panel Dialog Bar.
//
//This allows the user to select colours and styles.

class CAttributePanel : public CDialogBar
{
private:
  //This attribute contains the list of colours read 
  //from the configuration file. It is used when drawing 
  //and selecting the dynamic combo boxes.
  vector< pair< long, long   > > m_colours;
  
  typedef map<long, HBITMAP> StylesMap;
  typedef map<long, HBITMAP>::iterator StylesMapIterator;
  StylesMap m_lineStyles[2];
  StylesMap m_fillStyles[2];
  StylesMap m_fontStyles[2];

  typedef map<long, int> StyleToIndexMap;
  StyleToIndexMap m_lineStylesToIndex;
  StyleToIndexMap m_fillStylesToIndex;
  StyleToIndexMap m_fontStylesToIndex;

  int lookupColour( long val ) ;
  HBITMAP blankBitmap( int width, int height, COLORREF backgroundColour );

  
  // Construction
public:
  //Utility function used by Frame Window to create the 
  //Attribute Panel.  This encapsulates the calls necessary 
  //to get the DialogBar functionality working.
  bool initialiseAndCreate( CFrameWnd * frame ) ;
  //Constructor for Attribute Panel
  CAttributePanel();   // standard constructor
  
  //Destructor for the Attribute Panel
  ~CAttributePanel();
  
  //This method is called once all MapLink configuration 
  //files have been loaded. 
  //
  //It loads the required colours from the configuration 
  //file and populates the combo boxes.
  void initialiseColours();
  
  // Dialog Data
  //{{AFX_DATA(CAttributePanel)
	enum { IDD = IDD_ATTRIBUTE_PANEL };
	CComboBox	m_textBackgroundStyle;
	CComboBox	m_textBackgroundMode;
	CComboBox	m_textBackgroundEdgeColour;
	CComboBox	m_textBackgroundColour;
        CComboBox	m_valign;
        CComboBox	m_halign;
        CComboBox	m_textFont;
        CComboBox	m_textColour;
        CComboBox	m_fillStyle;
        CComboBox	m_edgeStyle;
        CComboBox	m_edgeColour;
        CComboBox	m_fillColour;
        int		m_edgeThickness;
	CString	        m_textHeight;
	//}}AFX_DATA
  
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAttributePanel)
protected:
  //Dialog data exchange.
  //
  //Automatically generate by DevStudio Class Wizard.
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL
  //Command Update handler.
  //
  //This is required to stop MFC from greying out all 
  //controls in the panel since there are no handlers in 
  //the parent class.
  virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler );
  
  // Implementation
protected:
  
  // Generated message map functions
        //{{AFX_MSG(CAttributePanel)
        afx_msg void OnSelchangeEdgeColour();
        afx_msg void OnSelchangeEdgeStyle();
        afx_msg void OnSelchangeFillColour();
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnSelchangeFillStyle();
        afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
        afx_msg void OnChangeEdgeThickness();
	afx_msg void OnSelchangeTextColour();
	afx_msg void OnChangeTextHeight();
	afx_msg void OnSelchangeTextFont();
	afx_msg void OnSelchangeTextHalign();
	afx_msg void OnSelchangeTextValign();
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnSelchangeTextBackgroundEdgeColour();
	afx_msg void OnSelchangeTextBackgroundMode();
	afx_msg void OnSelchangeTextBackgroundStyle();
	afx_msg void OnSelchangeTextBackgroundColour();
	//}}AFX_MSG
  //Dialog initialisation for Attribute Panel.
  //
  //Reads configuration file and initialises controls.
  afx_msg LRESULT OnInitDialog( WPARAM wParam, LPARAM lParam ) ;
  DECLARE_MESSAGE_MAP()
    
  private:
    TSLNTSurface *m_surface;

    COLORREF m_windowColour ;
    COLORREF m_highlightColour ;

    int m_windowColourIndex ;
    int m_windowTextColourIndex;
    int m_highlightColourIndex ;
    int m_highlightTextColourIndex;

    int m_editControlHeight, m_listBoxItemHeight;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATTRIBUTEPANEL_H__20FF785F_FFD1_11D3_9284_00C04FA1187D__INCLUDED_)
