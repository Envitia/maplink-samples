
// EarthSampleDoc.h : interface of the CEarthSampleDoc class
//


#pragma once
#include "managers/datalayers/datalayersmanager.h"
#include "managers/tracks/tracksmanager.h"
#include "managers/geometry/geometrymanager.h"

#include <vector>
#include <memory>
#include <chrono>


class CEarthSampleDoc : public CDocument
{
protected: // create from serialization only
	CEarthSampleDoc();
	DECLARE_DYNCREATE(CEarthSampleDoc)

public:
  // Update the position of the selected track
  void notify_selectedTrackViewChanged();

  // select track
  void notify_unselectTrack();
  void notify_selectTrack(earth::Track* pickedTrack, int lastPickedTrack);

  // reset camera manager
  void notify_resetCameraManager();

  // initial map to load
  std::string initialMapPath;

// Overrides
public:
	virtual BOOL OnNewDocument();
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CEarthSampleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
