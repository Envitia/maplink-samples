// EditorSampleDoc.h : interface of the CEditorSampleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//This user-defined document class contains the application 
//specific data which the user is currently editing.
//
//The constructor reads the map paths and map name from the 
//initialisation file and uses them when constructing the map 
//layer.
//
//Its main function is to encapsulate the set of data layers 
//that need to be displayed by the CEditorSampleView class.  
//
// Map Layer  - This double buffered layer displays the map
// Grid Layer - This layer contains a snappable grid
// Edit Layer - This layer displays the current edit
//
//When the document is first displayed in a View, the layers 
//are added to the drawing surface within the view by the view 
//calling the attachToDS method.
class CEditorSampleDoc : public CDocument
{
  protected: // create from serialization only
	//Constructor for document.
	//
	//This reads the map paths and map name from the 
	//initialisation file and uses them when constructing the 
	//map layer.
    CEditorSampleDoc();
    DECLARE_DYNCREATE(CEditorSampleDoc)
    
      // Attributes
  public:
  
    // Operations
  public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEditorSampleDoc)
	public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL
  
    // Implementation
  public:
	//This method adds the set of data layers contained 
	//within the document to the specified drawing surface.
    bool addToSurface( TSLDrawingSurface * surface );

	//This method removes the current set of data layers from 
	//the specified drawing surface.
    bool detachFromSurface( TSLDrawingSurface * surface );
  
        // This method loads the map into the map data layer. Returns true on success.
    bool loadMap( const char * mapFile );

       //Destructor for  document
    virtual ~CEditorSampleDoc();

	//This method returns an envelope defined the area to 
	//resize the map to when the user resets the view
    TSLEnvelope getResetArea( ) ;

	//This method provides access the TSLMapDataLayer 
	//containing the map for the application.
    const TSLMapDataLayer * getMapLayer( ){ return m_mapLayer ; }

    const TSLStandardDataLayer * getEditLayer( ){ return m_editLayer ; }

	//This method displays the results of the Auto Title 
	//algorithm in the map display area.

  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif

    static const char * m_mapLayerName ;
    static const char * m_editLayerName ;
    static const char * m_gridLayerName ;
  
  protected:
  
    // Generated message map functions
  protected:
    //{{AFX_MSG(CEditorSampleDoc)
	afx_msg void OnFileLoadmapunderlay();
	afx_msg void OnViewGridlayer();
	afx_msg void OnUpdateViewGridlayer(CCmdUI* pCmdUI);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
  private:  
   
    //The data layer containing the map
    TSLMapDataLayer  * m_mapLayer ;

    //The data layer displaying edit.
    TSLStandardDataLayer * m_editLayer;
    
    //The data layer displaying the grid.
    TSLGridDataLayer * m_gridLayer ;

    //Current grid parameters - only displayed when a map is loaded
    double m_gridScale ;
    double m_gridXOrigin ;
    double m_gridYOrigin ;

    bool m_gridLayerVisible ;

    //This is the path list to search for map tiles.
    TSLPathList     * m_pathList ;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
