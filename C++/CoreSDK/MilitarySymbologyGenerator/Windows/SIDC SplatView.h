
// Hello GlobeView.h : interface of the CSidcSplatView class
//

#pragma once
#include <MapLink.h>
#include <memory>


class CSidcSplatView : public CView
{
protected: // create from serialization only
	CSidcSplatView() noexcept;
	DECLARE_DYNCREATE(CSidcSplatView)

public:
	CSidcSplatDoc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual ~CSidcSplatView();

protected:

protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

private:
	std::unique_ptr<TSLNTSurface> m_drawingSurface;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

inline CSidcSplatDoc* CSidcSplatView::GetDocument() const {
  return reinterpret_cast<CSidcSplatDoc*>(m_pDocument);
}
