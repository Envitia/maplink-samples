// C2PerformanceDemoDoc.h : interface of the CC2PerformanceDemoDoc class
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
class COwnShipTrack;
class CAnimator;
class CRangeRing;
class CTrack;

class CC2PerformanceDemoDoc : public CDocument
{ 
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
    // Query the lat/lon data layer that has been loaded into the document
    TSLLatLongGridDataLayer * getGridLayer()
    {
      return m_latLonGridDataLayer ;
    }

    // Retrieve a pointer to the own ship track
    COwnShipTrack * getOwnShipTrack()
    {
      return (COwnShipTrack*)m_ownShipTrack ;
    }

    // Retrieve a pointer to the animator
    CAnimator * getAnimator()
    {
      return m_animator ;
    }

    bool isDisplayingAPP6A ()
    {
      return m_isAPP6A;
    }

    bool isDisplaying2525B ()
    {
      return m_is2525B;
    }

    bool changeToAPP6aSymbols ();
    bool changeTo2525BSymbols ();
    bool changeToStandardSymbols ();
    int getCurrentNoOfSymbolTracks ();
    void setCurrentNoOfSymbolTracks ( int newNumber );
    void refreshAPP6AAnnotationLevel ();
    
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CC2PerformanceDemoDoc)
	public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL
  
    // Implementation
  public:
    virtual ~CC2PerformanceDemoDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif
  
  protected:

    ////////////////////////////////////////////
    // Own Ship Dynamic Data Object
    CTrack	* m_ownShipTrack ;

    ////////////////////////////////////////////
    // Range Ring Dynamic Data Object
    CRangeRing		* m_rangeRing ;
  
    //{{AFX_MSG(CC2PerformanceDemoDoc)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    CC2PerformanceDemoDoc();
    DECLARE_DYNCREATE(CC2PerformanceDemoDoc)
    
  private:

    // The data layer containing the map
    TSLMapDataLayer * m_mapDataLayer ;

    // The standard data layer containing geometry and symbols
    TSLStandardDataLayer * m_standardDataLayer ;

    // The object layer containing Dynamic Data Objects ( Tracks )
    TSLObjectDataLayer * m_objectDataLayer ;

    // Grid data-layer
    TSLLatLongGridDataLayer * m_latLonGridDataLayer;

    TSLAPP6AHelper* m_symbolHelper;

    // Animator to update the graphics
    CAnimator * m_animator ;

    bool m_isAPP6A;
    bool m_is2525B;

    int m_NoOfSymbolTracks;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
