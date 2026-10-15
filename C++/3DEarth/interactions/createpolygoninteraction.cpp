// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************
#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif

#include "MapLink3DIMode.h"
#include "createpolygoninteraction.h"

CreatePolygonInteraction::CreatePolygonInteraction(int modeID, std::string styleName)
  : Interaction(modeID)
  , m_styleName(styleName)
{}

CreatePolygonInteraction::~CreatePolygonInteraction() {}

void CreatePolygonInteraction::activate() {}
void CreatePolygonInteraction::deactivate() {
  if (m_polygon) {
    auto o = m_polygon->outer();
    if (o.size() > 0) {
      o.erase(o.size() - 1);
      m_polygon->outer(o);
    }

    // Update the style to use detailed tesselation/draping settings
    m_polygon->styleName(m_styleName.c_str());

    // The primitive is already present in the surface, and managed by the sample document
    // So all the interaction needs to do is drop the pointer - On the next left click
    // a new line will be started.
    m_polygon = nullptr;
  }
}

bool CreatePolygonInteraction::onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
  auto surface = m_drawingSurface;
  if (!surface) return false;

  double lat, lon;
  if (!surface->DUToLatLong(x, y, &lat, &lon)) return false;

  if (m_polygon == nullptr) {
    // Start a new primitive
    earth::GeodeticPointSet points;
    points.push_back({ lon, lat, m_height }); // The clicked point
    points.push_back({ lon, lat, m_height }); // The next point - will track the mouse as it moves
    // Create the primitive - Initially with a simplified style, to reduce the performance impact when moving the mouse
    m_polygon = new earth::geometry::Polygon((m_styleName + "-simple").c_str(), points);

    // Add the line to the surface - We'll be continuing to update the geometry while the user edits
    if (!surface->addGeometry(*m_polygon)) {
      delete m_polygon;
	  return false;
    }

	// Pass ownership to the document model
	if (m_requestHandler)
		m_requestHandler->geometryAdded(m_polygon);
  }
  else {
    // Add the current point to the end of the line
    if (m_polygon) {
      auto c = m_polygon->outer();
      c.push_back({ lon, lat, m_height });
      m_polygon->outer(c);
    }
  }

  return true;
}

bool CreatePolygonInteraction::onRButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
	auto surface = m_drawingSurface;
	if (!surface) return false;

	double lat, lon;
	if (!surface->DUToLatLong(x, y, &lat, &lon)) return false;
	if(m_polygon)
	{
		// Finish/commit the current primitive

		// Update the style to use detailed tesselation/draping settings
		m_polygon->styleName(m_styleName.c_str());

		// The primitive is already present in the surface, and managed by the sample document
		// So all the interaction needs to do is drop the pointer - On the next left click
		// a new line will be started.
		m_polygon = nullptr;
	}

	return true;
}

bool CreatePolygonInteraction::onMouseMove(TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
  if (!m_polygon || !m_drawingSurface) return false;

  auto surface = m_drawingSurface;

  // Move the last point of the line as the mouse moves
  double lat, lon;
  if (!surface->DUToLatLong(x, y, &lat, &lon)) return false;

  auto c = m_polygon->outer();
  if (c.size() > 0) c.erase(c.size() - 1);

  c.push_back({ lon, lat, m_height });
  m_polygon->outer(c);

  return true;
}
