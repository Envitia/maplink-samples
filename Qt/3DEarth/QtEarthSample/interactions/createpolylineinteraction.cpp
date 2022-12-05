// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************
#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif


#include "MapLink3DIMode.h"
#include "createpolylineinteraction.h"

CreatePolylineInteraction::CreatePolylineInteraction(int modeID, std::string styleName)
  : Interaction(modeID)
  , m_styleName(styleName)
{}

CreatePolylineInteraction::~CreatePolylineInteraction() {}

void CreatePolylineInteraction::activate() {}
void CreatePolylineInteraction::deactivate() {
  if (m_polyline) {
    auto c = m_polyline->coords();
    if (c.size() > 0) {
      c.erase(c.size() - 1);
      m_polyline->coords(c);
    }

    // Finish/commit the current polyline
    // Update the style to use detailed tesselation/draping settings
    m_polyline->styleName(m_styleName.c_str());

    // The line is already present in the surface, and managed by the sample document
    // So all the interaction needs to do is drop the pointer - On the next left click
    // a new line will be started.
    m_polyline = nullptr;
  }
}

bool CreatePolylineInteraction::onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
  auto surface = m_drawingSurface;
  if (!surface) return false;

  double lat, lon;
  if (!surface->DUToLatLong(x, y, &lat, &lon)) return false;

  if (m_polyline == nullptr) {
    // Start a new line
    earth::GeodeticPointSet points;
    points.push_back({ lon, lat, m_lineHeight }); // The clicked point
    points.push_back({ lon, lat, m_lineHeight }); // The next point - will track the mouse as it moves
    // Create the primitive - Initially with a simplified style, to reduce the performance impact when moving the mouse
    m_polyline = new earth::geometry::Polyline((m_styleName + "-simple").c_str(), points);

    // Add the line to the surface - We'll be continuing to update the geometry while the user edits
    if (!surface->addGeometry(*m_polyline)) {
      delete m_polyline;
	  return false;
    }
    // Pass ownership to the document model
	if (m_requestHandler)
		m_requestHandler->geometryAdded(m_polyline);
  }
  else {
    // Add the current point to the end of the line
    if (m_polyline) {
      auto c = m_polyline->coords();
      c.push_back({ lon, lat, m_lineHeight });
      m_polyline->coords(c);
    }
  }

  return true;
}

bool CreatePolylineInteraction::onRButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
	auto surface = m_drawingSurface;
	if (!surface) return false;

	double lat, lon;
	if (!surface->DUToLatLong(x, y, &lat, &lon)) return false;
	if(m_polyline)
	{
		// Finish/commit the current polyline
		// Update the style to use detailed tesselation/draping settings
		m_polyline->styleName(m_styleName.c_str());

		// The line is already present in the surface, and managed by the sample document
		// So all the interaction needs to do is drop the pointer - On the next left click
		// a new line will be started.
		m_polyline = nullptr;
	}

	return true;
}

bool CreatePolylineInteraction::onMouseMove(TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
  if (!m_polyline || !m_drawingSurface) return false;

  auto surface = m_drawingSurface;

  // Move the last point of the line as the mouse moves
  double lat, lon;
  if (!surface->DUToLatLong(x, y, &lat, &lon)) return false;

  auto c = m_polyline->coords();
  if (c.size() > 0) c.erase(c.size() - 1);

  c.push_back({ lon, lat, m_lineHeight });
  m_polyline->coords(c);

  return true;
}
