// EditorSampleView.h : interface of the CEditorSampleView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class TSLViewMode;
class TSLViewModeEdit;
class TSLViewModeInterfaceNT;

class CEditorSampleDoc;

//The CEditorSampleView class provides the functionality for 
//view classes. A view is attached to a document and acts as 
//an intermediary between the document and the user: the view 
//renders an image of the document on the screen or printer 
//and interprets user input as operations upon the document.
//
//This encapsulates the TSLDrawingSurface used to render the 
//maps and titles, along with controlling the viewing 
//functionality.
class CEditorSampleView : public CView
{
protected: // create from serialization only
  //Constructor for the LandLink Sample View class.
  CEditorSampleView();
  DECLARE_DYNCREATE(CEditorSampleView)

  // Attributes
public:
  //Standard MFC query method.
  CEditorSampleDoc* GetDocument();

  //This method tells the View that editing is allowed.  
  //This status is passed to the TSLEditMode instance.
  void allowEdit( bool status ) ;

  const char * queryPrompt( ) ;

  TSLDrawingSurface * queryDrawingSurface( ){ return m_drawingSurface ; } 

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CEditorSampleView)
public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual void OnInitialUpdate();
protected:
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  // Implementation
public:
  //This reset callback sets the current mode back to the 
  //edit mode
  static void resetCB(void * self);

  //Destructor for the LandLink Sample view.
  //
  //Destroys the drawing surface and mode handlers.
  virtual ~CEditorSampleView();

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  //Menu handler for reset option.
  //
  //This changes the drawing surface view to display the 
  //entire map extent.
  afx_msg void OnToolsReset();

protected:

  // Generated message map functions
protected:
  //{{AFX_MSG(CEditorSampleView)
  afx_msg void OnToolsZoomin();
  afx_msg void OnToolsZoomout();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnUpdateToolsMode(CCmdUI* pCmdUI); 
  afx_msg void OnToolsPan();
  afx_msg void OnToolsZoom();
  afx_msg void OnToolsEdit();
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg void OnToolsMagnify();
  afx_msg void OnFilePrint();
  afx_msg void OnFilePrintPreview();
  //}}AFX_MSG
  // Class wizard doesn't know about the middle button !!
  afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
  DECLARE_MESSAGE_MAP()

private:

  TSLViewModeEdit* m_editMode;

  // Handle to drawing surface
  TSLNTSurface * m_drawingSurface ;

  // Current interaction mode
  TSLViewMode * m_currentMode ;

  // Instance of OS specific interaction mode interface
  TSLViewModeInterfaceNT * m_modeInterface ;

  //This attribute is set when a map view reset is 
  //required, usually on the first OnDraw after the 
  //OnInitialUpdate is called.  This is necessary since the 
  //toolbar position negotiation occurs between these two 
  //calls.  Hence a reset in the OnInitialUpdate will not 
  //be correct.
  bool m_resetRequired;

  //Temporary storage when printing
  bool   m_printing ;

  double m_uux1 ;
  double m_uuy1 ;
  double m_uux2 ;
  double m_uuy2 ;
  TSLDeviceUnits   m_scrx1 ;
  TSLDeviceUnits   m_scry1 ;
  TSLDeviceUnits   m_scrx2 ;
  TSLDeviceUnits   m_scry2 ;

  HCURSOR  m_zoomCursor ;
  HCURSOR  m_panCursor ;

};

#ifndef _DEBUG  // debug version in EditorSampleView.cpp
inline CEditorSampleDoc* CEditorSampleView::GetDocument()
{ return (CEditorSampleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCVIEW_H__CF7BD0CE_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
