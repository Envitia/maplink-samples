#ifndef _TEXTVIEW_H_
#define _TEXTVIEW_H_
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4786)

class CTextView : public CScrollView
{
protected:
  CTextView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CTextView)
public:
  void addLine( const std::string &line );
  void clear();
  bool logFile(const std::string &logFile);

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CHexView)
public:

  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:

  virtual void OnInitialUpdate(); // called first time after construct
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

public:
  virtual ~CTextView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  //{{AFX_MSG(CTextView)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
  CRect m_rectPrint;
  std::vector<std::string> m_stringList;
  FILE *m_logFile;
  bool m_scrollToEndPosition;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
