// GeometryCreationDoc.h : interface of the CGeometryCreationDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEOMETRYCREATIONDOC_H__412F36F8_49E5_4DDA_81AB_051951201134__INCLUDED_)
#define AFX_GEOMETRYCREATIONDOC_H__412F36F8_49E5_4DDA_81AB_051951201134__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGeometryCreationDoc : public CDocument
{
  protected: // create from serialization only
    CGeometryCreationDoc();
    DECLARE_DYNCREATE(CGeometryCreationDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
  
    //This method adds the set of data layers contained 
    //within the document to the specified drawing surface.
    bool addToSurface( TSLDrawingSurface * surface );
  
    //This method removes the current set of data layers from 
    //the specified drawing surface.
    bool detachFromSurface( TSLDrawingSurface * surface );

    void updatedPrimitive( TSLEntity * entity ) ;

    TSLEntitySet * entities( )
    {
      return m_dataLayer ? m_dataLayer->entitySet() : 0 ;
    }
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGeometryCreationDoc)
	public:
    virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~CGeometryCreationDoc();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif
  
    // The data layer containing the geometry
    TSLStandardDataLayer * m_dataLayer ;
  
    // Name of my layer
    static const char * m_layerName ;
    
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(CGeometryCreationDoc)
	afx_msg void OnPrimitivesRectangle();
	afx_msg void OnPrimitivesPolygon();
	afx_msg void OnPrimitivesPolyline();
	afx_msg void OnPrimitivesSymbol();
	afx_msg void OnPrimitivesText();
	afx_msg void OnPrimitivesArc();
	afx_msg void OnPrimitivesEllipse();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GEOMETRYCREATIONDOC_H__412F36F8_49E5_4DDA_81AB_051951201134__INCLUDED_)
