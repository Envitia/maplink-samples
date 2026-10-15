// C2DemoDoc.h : interface of the CC2DemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class TSLMapDataLayer ;
class TSLStandardDataLayer ;
class TSLObjectDataLayer ;
class TSLPathList ;

class CAnimator;
class CRangeRing;
class COwnShipTrack;

class CC2DemoDoc : public CDocument
{
  protected: // create from serialization only
    CC2DemoDoc();
    DECLARE_DYNCREATE(CC2DemoDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    //
    // Query the map data layer that has been loaded into the document.
    TSLMapDataLayer * getMapDataLayer()
    { 
      return m_mapDataLayer ; 
    }

    // Query the map data layer that has been loaded into the document
    TSLStandardDataLayer * getStandardDataLayer()
    {
      return m_standardDataLayer ;
    }

    // Query the object data layer that has been loaded into the document
    TSLObjectDataLayer * getObjectDataLayer()
    {
      return m_objectDataLayer ;
    }

    // Retrieve a pointer to the own ship track
    COwnShipTrack * getOwnShipTrack()
    {
      return m_ownShipTrack ;
    }

    // Retrieve a pointer to the animator
    CAnimator * getAnimator()
    {
      return m_animator ;
    }
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CC2DemoDoc)
	public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~CC2DemoDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:

    ////////////////////////////////////////////
    // Own Ship Dynamic Data Object
    COwnShipTrack	* m_ownShipTrack ;

    ////////////////////////////////////////////
    // Range Ring Dynamic Data Object
    CRangeRing		* m_rangeRing ;
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(CC2DemoDoc)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:

    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;

    // The standard data layer containing geometry and symbols
    TSLStandardDataLayer * m_standardDataLayer ;

    // The object layer containing Dynamic Data Objects ( Tracks )
    TSLObjectDataLayer * m_objectDataLayer ;

    // Animator to update the graphics
    CAnimator * m_animator ;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
