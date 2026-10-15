// importexportDoc.h : interface of the CImportExportDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
#define AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>
using namespace std;

class CImportExportDoc : public CDocument
{
  protected: // create from serialization only
    CImportExportDoc();
    DECLARE_DYNCREATE(CImportExportDoc)

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


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CImportExportDoc)
	public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual void DeleteContents();
	//}}AFX_VIRTUAL

    // Implementation
  public:
    virtual ~CImportExportDoc();
  #ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
  #endif

  protected:

    // Generated message map functions
  protected:
    //{{AFX_MSG(CImportExportDoc)
	afx_msg void OnFileAppend();
	afx_msg void OnUpdateFileAppend(CCmdUI* pCmdUI);
	afx_msg void OnFileExportAscii();
	afx_msg void OnUpdateFileExportAscii(CCmdUI* pCmdUI);
	afx_msg void OnFileExportShapefile();
	afx_msg void OnUpdateFileExportShapefile(CCmdUI* pCmdUI);
	afx_msg void OnFileExportTmf();
	afx_msg void OnUpdateFileExportTmf(CCmdUI* pCmdUI);
	afx_msg void OnFileExportMif();
	afx_msg void OnUpdateFileExportMif(CCmdUI* pCmdUI);
	afx_msg void OnFileExportOSMasterMap();
	afx_msg void OnUpdateFileExportOSMasterMap(CCmdUI* pCmdUI);
	//}}AFX_MSG
        afx_msg void OnSetTestConfig(UINT nID);
        afx_msg void OnUpdateSetTestConfig(CCmdUI* pCmdUI);
    DECLARE_MESSAGE_MAP()

  private:

    bool importFile(const char *filepath, bool append);

    void saveFile(const char* defExt);

    void addFeatureCodeMapping(const char *nativeFeatureCode);

    void setupFeatureRendering(TSLDataLayer *layer);

    void addOSMFeatureMapping();

    void addS57FeatureMapping();

    void setupOSMFeatureRendering(TSLDataLayer *layer);

    void initialiseFeatureRendering();

    void setupInteropConfig(const char *basefilename);

    // The data layer into which we will import the data
    TSLStandardDataLayer * m_stdDataLayer ;

    // Name of my import layer
    static const char * m_stdLayerName ;

    // Interoperability configuration settings.
    TSLInteropConfig *m_interopConfig;

    // Interoperability test configuration selection.
    UINT m_testConfigSelection;

    // This contains the feature code to use for the next file to import
    int m_nextImportFeatureCode;
    string m_nextNativeFeatureCode;

    // And the next edge and fill colours etc. to use
    int m_nextEdgeColour;
    int m_nextFillColour;
    int m_nextSymbolColour;
    int m_nextTextColour;

    string m_mifFilterConfig ;

    vector<TSLFeatureCodeMapping> m_featureCodesLoaded;

public:
  afx_msg void OnFileOpen();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDOC_H__CF7BD0CC_4693_11D3_8F16_0020AFC8854E__INCLUDED_)
