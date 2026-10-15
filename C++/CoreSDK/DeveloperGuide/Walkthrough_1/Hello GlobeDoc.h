// Hello GlobeDoc.h : interface of the CHelloGlobeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HELLOGLOBEDOC_H__EFDCE087_433F_4F0B_AC97_98A3BDB64F56__INCLUDED_)
#define AFX_HELLOGLOBEDOC_H__EFDCE087_433F_4F0B_AC97_98A3BDB64F56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHelloGlobeDoc : public CDocument
{
protected: // create from serialization only
	CHelloGlobeDoc();
	DECLARE_DYNCREATE(CHelloGlobeDoc)

// Attributes
public:

// Operations
public:
  bool addToSurface( TSLDrawingSurface * drawingSurface ) ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelloGlobeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHelloGlobeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHelloGlobeDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  TSLMapDataLayer * m_mapDataLayer ;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELLOGLOBEDOC_H__EFDCE087_433F_4F0B_AC97_98A3BDB64F56__INCLUDED_)
