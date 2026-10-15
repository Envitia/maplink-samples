
// EarthSampleView.h : interface of the CEarthSampleView class
//

#pragma once

// Include the MapLink Core SDK
#include <MapLink.h>

// Include the MapLink Earth SDK
// Note that the Earth SDK makes use of namespaces within the API
// in this case envitia::maplink::earth
#include <MapLinkEarth.h>
using namespace envitia::maplink;

#include <memory>
#include <vector>
#include <map>

#include "CameraControlFormView.h"

#include "managers/datalayers/datalayersmanager.h"
#include "managers/tracks/tracksmanager.h"
#include "managers/geometry/geometrymanager.h"

//! Interaction mode IDs - these can be any numbers
#define ID_TOOLS_NoInteraction            0
#define ID_TOOLS_Select                   1
#define ID_TOOLS_CreatePolygon            2
#define ID_TOOLS_CreatePolyline           3
#define ID_TOOLS_CreateText               4
#define ID_TOOLS_CreateSymbol             5
#define ID_TOOLS_CreateExtrudedPolygon    6
#define ID_TOOLS_CreateExtrudedPolyline   7
#define ID_TOOLS_DeleteGeometry           8
#define ID_TOOLS_Trackball                9

class CEarthSampleDoc;

class CEarthSampleView : public CView, public InteractionModeRequest
{
protected: // create from serialization only
	CEarthSampleView();
	DECLARE_DYNCREATE(CEarthSampleView)

	// Handler methods for core application/window events
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual void OnSize(UINT nType, int cx, int cy);
	static void CALLBACK OnRedrawTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

public:
	virtual ~CEarthSampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:    /******** drawing surface functions ********/
	// load map layer/ terrain database
	bool loadLayer(std::string filePath, std::string& err_msg);

	// Add a datalayer to the view
	bool addDataLayer(TSLDataLayer* layer, std::string layerName);

	// Add a terrain database to the view
	bool addTerrainDatabase(TSLTerrainDatabase& database, std::string databaseName);

	// Add tracks to the view
	bool addTracks(std::vector<earth::Track>& tracks);

	// Remove tracks from the view
	void removeTracks();

	// Add geometry to the view
	bool addGeometry(std::vector<std::unique_ptr<earth::geometry::Geometry>>& geom);

	// Render the view to an HDC
	bool drawToHDC(TSLDeviceContext& hdc, TSLDeviceUnits width, TSLDeviceUnits height);

	void resetView();

public:        /******** Interaction modes ********/
	// initialize interaction mode manager
	void initializeModeManager();
	
	// Set the interaction of the view
	void interaction(int i, bool reactivate = false);
	int currentInteractionMode();

	// mouse/buttons
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT, short, CPoint);

	// interaction modes callbacks
	virtual void resetMode(Interaction * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU) override final;
	virtual void viewChanged(envitia::maplink::earth::Surface3D* drawingSurface) override final;
	virtual void geometryAdded(earth::geometry::Geometry* geom) override final;
	virtual void geometryRemoved(earth::geometry::Geometry* geom) override final;
	virtual void geometrySelected(earth::geometry::Geometry* geom) override final;
	virtual void trackSelected(earth::Track* track) override final;
	virtual TSLEnvelope GetWindowEnvelope() override final;

public:	/******** properties control ********/
	CEarthSampleDoc* GetDocument() const;
	static CEarthSampleView* GetView();

	earth::Surface3D* GetSurface();

	// object to manage the map and terrain data layers
	DatalayersManager& datalayersManager();

	// object to manage the tracks
	TracksManager& tracksManager();

	// geometry manager
	GeometryManager& geometryManager();

private:
  // view height and width
  int m_widgetWidth = 1, m_widgetHeight = 1;

  // A timer used to regularly redraw the surface
  // This sample is configured to continuously redraw,
  // any changes to the surface/view will take effect
  // automatically
  UINT_PTR m_redrawTimer;

  // The MapLink Surface
  std::unique_ptr<earth::Surface3D> m_surface;

  // Interaction manager
  InteractionModeManager * m_modeManager;

  /** data managers */
  // object to manage the map and terrain data layers
  DatalayersManager m_datalayersManager;

  // object to manage the tracks
  TracksManager m_tracksManager;

  // geometry manager
  GeometryManager m_geometryManager;
};

#ifndef _DEBUG  // debug version in EarthSampleView.cpp
inline CEarthSampleDoc* CEarthSampleView::GetDocument() const
   { return reinterpret_cast<CEarthSampleDoc*>(m_pDocument); }
#endif

