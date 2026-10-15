#if !defined(AFX_TERRAINVIEW_H__58AFD8DE_4B0D_11D5_93AE_00C04FA10B1D__INCLUDED_)
#define AFX_TERRAINVIEW_H__58AFD8DE_4B0D_11D5_93AE_00C04FA10B1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TerrainView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTerrainView view

class CTerrainView : public CView
{
protected:
  CTerrainView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CTerrainView)

private:
  int    m_num_values ;
  double m_values[ 100]  ;
  double m_range ;

public:

  void SetData(int num_values, double * values, double range = 0.0) ;

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTerrainView)
protected:
  virtual void OnDraw(CDC* pDC);      // overridden to draw this view
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CTerrainView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CTerrainView)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERRAINVIEW_H__58AFD8DE_4B0D_11D5_93AE_00C04FA10B1D__INCLUDED_)
