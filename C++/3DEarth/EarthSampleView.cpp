
// EarthSampleView.cpp : implementation of the CEarthSampleView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "EarthSample.h"
#endif

#include "EarthSampleDoc.h" 
#include "EarthSampleView.h"
#include "MainFrm.h"

#include <sstream>
#include <algorithm>
#include <random>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEarthSampleView

IMPLEMENT_DYNCREATE(CEarthSampleView, CView)

BEGIN_MESSAGE_MAP(CEarthSampleView, CView)
  ON_WM_MOUSEMOVE()
  ON_WM_RBUTTONDOWN()
  ON_WM_RBUTTONUP()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MBUTTONDOWN()
  ON_WM_MBUTTONUP()
  ON_WM_SIZE()
  ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CEarthSampleView construction/destruction

CEarthSampleView::CEarthSampleView()
{
	m_modeManager = 0;
}

CEarthSampleView::~CEarthSampleView()
{
	if (m_modeManager)
	{
		delete m_modeManager;
		m_modeManager = 0;
	}

  while (m_surface->numTerrainDatabases() > 0) {
    m_surface->removeTerrainDatabase(0u);
  }
  while (m_surface->getNumDataLayers() > 0) {
    TSLDataLayer* layer = nullptr;
    const char* layerName = nullptr;
    m_surface->getDataLayerInfo(0, &layer, &layerName);
    m_surface->removeDataLayer(layerName);
  }
}

BOOL CEarthSampleView::PreCreateWindow(CREATESTRUCT& cs)
{
  return CView::PreCreateWindow(cs);
}

void CEarthSampleView::OnInitialUpdate()
{
  if (!m_surface) 
  {
    // Create the maplink surface, and attach to the window
    m_surface.reset(new earth::Surface3D((TSLWindowHandle)m_hWnd));

    // Set the size of the surface to match the window size
    m_surface->wndResize(0, 0, m_widgetWidth, m_widgetHeight, false);

	// Now create and initialse the mode manager and modes
	initializeModeManager();

	// Setup some basic styles for the created geometry
	GeometryManager::createStyles(m_surface.get());

	// Send this surface pointer to the Camera form, so that it can modify the camera.
	((CMainFrame*)AfxGetMainWnd())->attachCameraUIToSurface(m_surface.get());
  }

  // open initial map
  if (m_surface)
  {
	  CEarthSampleDoc* pDoc = GetDocument();
	  if (pDoc)
	  {
		  if (!pDoc->initialMapPath.empty())
		  {
			  std::string err_msg;
			  if (!loadLayer(pDoc->initialMapPath, err_msg))
			  {
				  AfxMessageBox(TSLUTF8Decoder(err_msg), MB_OK);
			  }
		  }
	  }
  }
}

void CALLBACK CEarthSampleView::OnRedrawTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
	::InvalidateRect(hwnd, NULL, false);
}

void CEarthSampleView::OnDraw(CDC* /*pDC*/)
{
  CEarthSampleDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc)
    return;

  // Update the track simulation
  // Note: Updating all tracks on every frame may affect performance,
  // instead tracks should be updated as and when needed, as part of the application's
  // event handling
  tracksManager().updateTracks();
  pDoc->notify_selectedTrackViewChanged();

  m_surface->redraw();

  // Render continuously
  KillTimer(m_redrawTimer);
  m_redrawTimer = SetTimer(0, EARTH_DRAW_DELAY, OnRedrawTimer);
}

void CEarthSampleView::OnSize(UINT nType, int cx, int cy) {
  CView::OnSize(nType, cx, cy);
  // Update the maplink surface with the new window size
  if (m_surface) {
    m_surface->wndResize(0, 0, cx, cy, false);
  }
  m_widgetWidth = cx;
  m_widgetHeight = cy;
}


bool CEarthSampleView::loadLayer(std::string filePath, std::string& err_msg)
{
	if (!m_surface) return false;

	// Store the layer
	TSLSimpleString dir;
	TSLSimpleString base;
	TSLSimpleString ext;
	TSLFileHelper::decomposePathname(filePath.c_str(), dir, base, ext);

	std::string layerName = ext + "_" + base;
	DatalayersManager& dataManager = datalayersManager();
	if (ext == "map")
	{
		bool result = dataManager.loadMapLayer(filePath, layerName, err_msg);
		if (result)
		{
			auto layerIndex = dataManager.getLayersSize() - 1;
			auto layer = dataManager.getLayer(layerIndex);
			if (!layer) return FALSE;
			auto layerName = dataManager.getLayerName(layerIndex);

			// Add the layer to the active view
			if (!addDataLayer(layer, layerName))
			{
				err_msg = "Failed to add data layer to the surface";
				return FALSE;
			}

			// reactive trackball interaction mode
			if (m_modeManager->getCurrentMode() == ID_TOOLS_Trackball)
			{
				m_modeManager->setCurrentMode(ID_TOOLS_Trackball, true);
			}
		}
		resetView();
		return result;
	}
	else if (ext == "tdf")
	{
		bool result = dataManager.loadTerrainDatabase(filePath, layerName, err_msg);
		if (result)
		{
			auto index = dataManager.getTerrainDatabasesSize() - 1;
			auto database = dataManager.getTerrainDatabase(index);
			if (!database) return FALSE;
			auto layerName = dataManager.getTerrainDatabaseName(index);

			// Add the layer to the view
			if (!addTerrainDatabase(*database, layerName))
			{
				err_msg = "Failed to add terrain database to the surface";
				return FALSE;
			}

			// reactive trackball interaction mode
			if (m_modeManager->getCurrentMode() == ID_TOOLS_Trackball)
			{
				m_modeManager->setCurrentMode(ID_TOOLS_Trackball, true);
			}
		}
		return result;
	}

	// File type not recognised
	err_msg = "CEarthSampleDoc::OnOpenDocument: File type not recognised/handled by application";
	return FALSE;
}

/******** drawing surface functions ********/
bool CEarthSampleView::addDataLayer(TSLDataLayer* layer, std::string layerName) {
	if (!m_surface) return false;
	return m_surface->addDataLayer(layer, layerName.c_str());
}

bool CEarthSampleView::addTerrainDatabase(TSLTerrainDatabase& database, std::string databaseName) {
	if (!m_surface) return false;
	return m_surface->addTerrainDatabase(database);
}

bool CEarthSampleView::addTracks(std::vector<earth::Track>& tracks) {
	if (!m_surface) return false;
	if (tracks.empty()) return false;

	for (auto& t : tracks) {
		m_surface->addTrack(t);
	}
	return true;
}

void CEarthSampleView::removeTracks()
{
	// Clear the tracks from the surface
	m_surface->removeAllTracks();
	// Now we can delete them
	tracksManager().removeTracks();
}

bool CEarthSampleView::addGeometry(std::vector<std::unique_ptr<earth::geometry::Geometry>>& geom) {
	if (!m_surface) return false;
	if (geom.empty()) return false;

	for (auto& g : geom) {
		m_surface->addGeometry(*g);
	}
	return true;
}

bool CEarthSampleView::drawToHDC(TSLDeviceContext& hdc, TSLDeviceUnits width, TSLDeviceUnits height)
{
	return m_surface->drawToHDC(hdc, width, height);
}

void CEarthSampleView::resetView()
{
	m_surface->reset(false);

	// reset camera manager
	CEarthSampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc)
		pDoc->notify_resetCameraManager();

	// reactive trackball interaction mode
	if (m_modeManager->getCurrentMode() == ID_TOOLS_Trackball)
	{
		m_modeManager->setCurrentMode(ID_TOOLS_Trackball, true);
	}
}

/******** Interaction modes ********/
void CEarthSampleView::initializeModeManager()
{
	if (m_modeManager == NULL)
	{
		m_modeManager = new InteractionModeManager(this, m_surface.get(), 30, true);
		m_modeManager->addMode(new SelectInteraction(ID_TOOLS_Select), false);
		m_modeManager->addMode(new CreatePolygonInteraction(ID_TOOLS_CreatePolygon, "polygon"), false);
		m_modeManager->addMode(new CreatePolylineInteraction(ID_TOOLS_CreatePolyline, "polyline"), false);
		m_modeManager->addMode(new CreateTextInteraction(ID_TOOLS_CreateText), false);
		m_modeManager->addMode(new CreateSymbolInteraction(ID_TOOLS_CreateSymbol), false);
		m_modeManager->addMode(new CreatePolygonInteraction(ID_TOOLS_CreateExtrudedPolygon, "extruded-polygon"), false);
		m_modeManager->addMode(new CreatePolylineInteraction(ID_TOOLS_CreateExtrudedPolyline, "extruded-polyline"), false);
		m_modeManager->addMode(new DeleteGeometryInteraction(ID_TOOLS_DeleteGeometry), false);
		m_modeManager->addMode(new TrackballViewInteraction(ID_TOOLS_Trackball), true);
		m_modeManager->setCurrentMode(ID_TOOLS_Trackball, true);
	}
}

int CEarthSampleView::currentInteractionMode()
{
	if (m_modeManager)
		return m_modeManager->getCurrentMode();

	return ID_TOOLS_NoInteraction;
}

void CEarthSampleView::interaction(int i, bool reactivate) {
	if (m_modeManager)
		m_modeManager->setCurrentMode(i, reactivate);
}

void CEarthSampleView::OnMouseMove(UINT nFlags, CPoint point) {
  double lat, lon;


  auto wnd = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
  if (wnd) {
    std::stringstream str;
    if (!m_surface->DUToLatLong(point.x, point.y, &lat, &lon)) {
      str << "Mouse: " << point.x << "," << point.y;
    }
    else {
      str << "Mouse: " << point.x << "," << point.y << " : ENU:" << lon << "," << lat;
      double x, y, z;
      if (m_surface->geodeticToGeocentric(lat, lon, 0, x, y, z)) {
        str << ": ECEF: " << x << "," << y << "," << z;
      }
    }

    wnd->setStatusText(str.str());
  }

  // If the user is in the middle of an interaction, pass the event onto the handler
  if (m_modeManager)
  {
	  TSLButtonType button = TSLButtonNone;

	  if (nFlags & MK_LBUTTON)
		  button = TSLButtonLeft;
	  else if (nFlags & MK_MBUTTON)
		  button = TSLButtonCentre;
	  else if (nFlags & MK_RBUTTON)
		  button = TSLButtonRight;

	  if (m_modeManager->onMouseMove(button, point.x, point.y,
		  nFlags & MK_SHIFT, nFlags & MK_CONTROL))
	  {
		  // Request a redraw if the interaction hander requires it
		  //InvalidateRect(0, false);
	  }
  }
}

void CEarthSampleView::OnRButtonDown(UINT nFlags, CPoint point) {
	// If an interaction is activated pass the call through
	if (m_modeManager)
	{
		if (m_modeManager->onRButtonDown(point.x, point.y,
			nFlags & MK_SHIFT, nFlags & MK_CONTROL))
		{
			// Request a redraw if the interaction hander requires it
			//InvalidateRect(0, false);
		}
	}
}

void CEarthSampleView::OnRButtonUp(UINT nFlags, CPoint point) {
  // If an interaction is activated pass the call through
	if (m_modeManager)
	{
		if (m_modeManager->onRButtonUp(point.x, point.y,
			nFlags & MK_SHIFT, nFlags & MK_CONTROL))
		{
			// Request a redraw if the interaction hander requires it
			//InvalidateRect(0, false);
		}
	}
}

void CEarthSampleView::OnLButtonUp(UINT nFlags, CPoint point) {
	// If an interaction is activated pass the call through
	if (m_modeManager)
	{
		if (m_modeManager->onLButtonUp(point.x, point.y,
			nFlags & MK_SHIFT, nFlags & MK_CONTROL))
		{
			// Request a redraw if the interaction hander requires it
			//InvalidateRect(0, false);
		}
	}
}

void CEarthSampleView::OnLButtonDown(UINT nFlags, CPoint point) {
	// If an interaction is activated pass the call through
	if (m_modeManager)
	{
		if (m_modeManager->onLButtonDown(point.x, point.y,
			nFlags & MK_SHIFT, nFlags & MK_CONTROL))
		{
			// Request a redraw if the interaction hander requires it
			//InvalidateRect(0, false);
		}
	}
}

void CEarthSampleView::OnMButtonUp(UINT nFlags, CPoint point) {
	// If an interaction is activated pass the call through
	if (m_modeManager)
	{
		if (m_modeManager->onMButtonUp(point.x, point.y,
			nFlags & MK_SHIFT, nFlags & MK_CONTROL))
		{
			// Request a redraw if the interaction hander requires it
			//InvalidateRect(0, false);
		}
	}
}

void CEarthSampleView::OnMButtonDown(UINT nFlags, CPoint point) {
	// If an interaction is activated pass the call through
	if (m_modeManager)
	{
		if (m_modeManager->onMButtonDown(point.x, point.y,
			nFlags & MK_SHIFT, nFlags & MK_CONTROL))
		{
			// Request a redraw if the interaction hander requires it
			//InvalidateRect(0, false);
		}
	}
}

BOOL CEarthSampleView::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
  if (m_modeManager) m_modeManager->onMouseWheel(zDelta, point.x, point.y);
  return true;
}

void CEarthSampleView::resetMode(Interaction * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
	// Ignore the override
}

void CEarthSampleView::viewChanged(envitia::maplink::earth::Surface3D* drawingSurface)
{
	if (auto wnd = dynamic_cast<CMainFrame*>(AfxGetMainWnd())) {
		wnd->OnTrackballMove();
	}
}

void CEarthSampleView::geometryAdded(earth::geometry::Geometry* geom)
{
	m_geometryManager.addGeometry(geom);
}

void CEarthSampleView::geometryRemoved(earth::geometry::Geometry* geom)
{
	m_geometryManager.removeGeometry(*geom);
}

void CEarthSampleView::geometrySelected(earth::geometry::Geometry* geom)
{
	m_geometryManager.switchGeometryStyle(geom);
}

void CEarthSampleView::trackSelected(earth::Track* track)
{
	if (!track) return;
	CEarthSampleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc)
		pDoc->notify_unselectTrack();
	bool isSelected = tracksManager().selectTrack(track);
	if(isSelected&& pDoc)
		pDoc->notify_selectTrack(track, tracksManager().getLastPickedTrack());
}

TSLEnvelope CEarthSampleView::GetWindowEnvelope()
{
	return TSLEnvelope(0, 0, m_widgetWidth, m_widgetHeight);
}

/******** properties control ********/
CEarthSampleView* CEarthSampleView::GetView()
{
  CMainFrame * pFrame = ((CMainFrame*)AfxGetMainWnd());

  if (!pFrame)
  {
    return NULL;
  }

  CView * pView = pFrame->getEarthView();

  if (!pView)
    return NULL;

  // Fail if view is of wrong kind
  // (this could occur with splitter windows, or additional
  // views on a single document
  if (!pView->IsKindOf(RUNTIME_CLASS(CEarthSampleView)))
    return NULL;

  return (CEarthSampleView *)pView;
}

earth::Surface3D * CEarthSampleView::GetSurface()
{
  return m_surface.get();
}

// object to manage the map and terrain data layers
DatalayersManager& CEarthSampleView::datalayersManager()
{
	return m_datalayersManager;
}

// object to manage the tracks
TracksManager& CEarthSampleView::tracksManager()
{
	return m_tracksManager;
}

// geometry manager
GeometryManager& CEarthSampleView::geometryManager()
{
	return m_geometryManager;
}

// CEarthSampleView diagnostics

#ifdef _DEBUG
void CEarthSampleView::AssertValid() const
{
  CView::AssertValid();
}

void CEarthSampleView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CEarthSampleDoc* CEarthSampleView::GetDocument() const // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEarthSampleDoc)));
  return (CEarthSampleDoc*)m_pDocument;
}
#endif //_DEBUG

// CEarthSampleView message handlers
