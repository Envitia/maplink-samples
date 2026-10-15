
// EarthSampleDoc.cpp : implementation of the CEarthSampleDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "EarthSample.h"
#endif

#include "EarthSampleDoc.h"
#include "CameraControlFormView.h"
#include "MainFrm.h"
#include "EarthSampleView.h"

#include <propkey.h>

#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEarthSampleDoc

IMPLEMENT_DYNCREATE(CEarthSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CEarthSampleDoc, CDocument)
END_MESSAGE_MAP()


// CEarthSampleDoc construction/destruction

CEarthSampleDoc::CEarthSampleDoc()
{
}

CEarthSampleDoc::~CEarthSampleDoc()
{

}

BOOL CEarthSampleDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  // TODO: add reinitialization code here
  // (SDI documents will reuse this document)

  return TRUE;
}

BOOL CEarthSampleDoc::OnOpenDocument(LPCTSTR lpszPathName) {
  if (!CDocument::OnOpenDocument(lpszPathName)) return FALSE;

  initialMapPath = TSLUTF8Encoder(lpszPathName);
  
  // open initial map
  auto mainFrm = static_cast<CMainFrame*>(AfxGetMainWnd());
  auto earthView = static_cast<CEarthSampleView*>(mainFrm->getEarthView());
  std::string err_msg;
  bool result = earthView->loadLayer(initialMapPath, err_msg);
  if (!result)
  {
	  AfxMessageBox(TSLUTF8Decoder(err_msg), MB_OK);
  }
  return result;
}

void CEarthSampleDoc::notify_selectedTrackViewChanged() {
  auto camForm = (CCameraControlFormView*)static_cast<CMainFrame*>(AfxGetMainWnd())->getCameraControlFormView();
  camForm->OnSelectedTrackViewChanged(NULL, NULL);
}

void CEarthSampleDoc::notify_unselectTrack()
{
	auto camForm = (CCameraControlFormView*)static_cast<CMainFrame*>(AfxGetMainWnd())->getCameraControlFormView();
	camForm->selectedTrack(nullptr, 0);
}

void CEarthSampleDoc::notify_selectTrack(earth::Track* pickedTrack, int lastPickedTrack)
{
	auto camForm = (CCameraControlFormView*)static_cast<CMainFrame*>(AfxGetMainWnd())->getCameraControlFormView();
	// For the first track result, set this as the active track in the Camera controls view
	camForm->selectedTrack(pickedTrack, lastPickedTrack);
}

void CEarthSampleDoc::notify_resetCameraManager()
{
	auto camForm = (CCameraControlFormView*)static_cast<CMainFrame*>(AfxGetMainWnd())->getCameraControlFormView();
	camForm->resetCameraManager();
}

// CEarthSampleDoc serialization
void CEarthSampleDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {

  }
  else
  {

  }
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CEarthSampleDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
  // Modify this code to draw the document's data
  dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

  CString strText = _T("TODO: implement thumbnail drawing here");
  LOGFONT lf;

  CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
  pDefaultGUIFont->GetLogFont(&lf);
  lf.lfHeight = 36;

  CFont fontDraw;
  fontDraw.CreateFontIndirect(&lf);

  CFont* pOldFont = dc.SelectObject(&fontDraw);
  dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
  dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CEarthSampleDoc::InitializeSearchContent()
{
  CString strSearchContent;
  // Set search contents from document's data. 
  // The content parts should be separated by ";"

  // For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
  SetSearchContent(strSearchContent);
}

void CEarthSampleDoc::SetSearchContent(const CString& value)
{
  if (value.IsEmpty())
  {
    RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
  }
  else
  {
    CMFCFilterChunkValueImpl *pChunk = NULL;
    ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
    if (pChunk != NULL)
    {
      pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
      SetChunkValue(pChunk);
    }
  }
}

#endif // SHARED_HANDLERS

// CEarthSampleDoc diagnostics

#ifdef _DEBUG
void CEarthSampleDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CEarthSampleDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG


// CEarthSampleDoc commands
