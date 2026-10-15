#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif
#include "MapLink.h"

// Include the MapLink Earth SDK
// Note that the Earth SDK makes use of namespaces within the API
// in this case envitia::maplink::earth
#include <MapLinkEarth.h>

#include "geometrymanager.h"
#include <algorithm>

GeometryManager::GeometryManager()
{
}

GeometryManager::~GeometryManager()
{
}
std::vector<std::unique_ptr<envitia::maplink::earth::geometry::Geometry>>& GeometryManager::getGeometry() {
	return m_geometry;
}

void GeometryManager::addGeometry(envitia::maplink::earth::geometry::Geometry* geom) {
	m_geometry.emplace_back(geom);
}

void GeometryManager::removeGeometry(envitia::maplink::earth::geometry::Geometry& geom) {
	auto it = std::find_if(m_geometry.begin(), m_geometry.end(), [&geom](const std::unique_ptr<envitia::maplink::earth::geometry::Geometry>& p) {
		return p.get() == &geom;
	});
	if (it == m_geometry.end()) return;
	m_geometry.erase(it);
}

void GeometryManager::switchGeometryStyle(envitia::maplink::earth::geometry::Geometry* geom)
{
	if (!geom) return;

	// Toggle the geometry between normal and 'selected' style
	auto sName = geom->styleName();
	if (!sName) return;

	std::string styleName(sName);
	std::string suffix("-selected");

	if (styleName.size() < suffix.size() ||
		styleName.substr(styleName.size() - suffix.size(), std::string::npos).compare(suffix) != 0) {
		geom->styleName((styleName + suffix).c_str());
	}
	else {
		geom->styleName((styleName.substr(0, styleName.size() - suffix.size())).c_str());
	}
}

void GeometryManager::createStyles(envitia::maplink::earth::Surface3D* surface)
{
	createPolylineStyle(surface, "polyline", TSLRGBA(0xee, 0x22, 0x22, 0xee), 4.0, true, 0.0);
	createPolygonStyle(surface, "polygon", TSLRGBA(0xee, 0x22, 0x22, 0xee), 2.0, TSLRGBA(0x22, 0x22, 0xee, 0xcc), true, 0.0);
	createTextStyle(surface, "text", TSLRGBA(0x00, 0x00, 0x00, 0xff), 30, TSLRGBA(0xff, 0xff, 0xff, 0xff));
	createSymbolStyle(surface, "symbol", 5, 30, TSLRGBA(0x33, 0x33, 0x33, 0xff));
	createPolylineStyle(surface, "extruded-polyline", TSLRGBA(0x22, 0x22, 0xee, 0xcc), 2.0, false, 5000.0);
	createPolygonStyle(surface, "extruded-polygon", TSLRGBA(0xee, 0x22, 0x22, 0xcc), 2.0, TSLRGBA(0x22, 0x22, 0xee, 0xcc), false, 3000.0);
}

void GeometryManager::createPolygonStyle(envitia::maplink::earth::Surface3D* surface,
	std::string name, TSLRGBA edgeColour, float edgeWidth, TSLRGBA fillColour, bool draped, float extrusionHeight) {
	if (!surface) return;
	envitia::maplink::earth::geometry::Style s;

	s.edge.colour = edgeColour;
	s.edge.width = edgeWidth;
	s.fill.colour = fillColour;

	if (draped) {
		// Drape polygons using the 'projection' mode - This offers perfect conformance to the surface
		// and excellent performance at the cost of not being able to offset above the surface/some slight rendering differences
		s.draping.mode = envitia::maplink::earth::geometry::Style::DrapeModeClampToTerrain;
		s.draping.technique = envitia::maplink::earth::geometry::Style::DrapeTechniqueProjection;
	}
	else if (extrusionHeight > 0.0) {
		// Extrude the geometry
		s.extrusion.flatten = true;
		s.extrusion.height = extrusionHeight;
	}

	surface->setStyle(name.c_str(), s);

	// Add a 'simple' variant
	// This style is the same as normal, but with less aggressive tesselation/accuracy settings
	// This allows the polygon/polyline creation interactions to update geometry much faster when the mouse moves,
	// and ensure geometry creation is responsive.
	envitia::maplink::earth::geometry::Style sSimple(s);
	sSimple.edge.tessellationDistance = 500000.0;
	sSimple.fill.tessellationDistance = 500000.0;
	surface->setStyle((name + "-simple").c_str(), sSimple);

	// Add a 'selected' variant
	envitia::maplink::earth::geometry::Style sSelected(s);
	std::swap(sSelected.edge.colour, sSelected.fill.colour);
	surface->setStyle((name + "-selected").c_str(), sSelected);
}

void GeometryManager::createPolylineStyle(envitia::maplink::earth::Surface3D* surface,
	std::string name, TSLRGBA edgeColour, float edgeWidth, bool draped, float extrusionHeight) {
	if (!surface) return;
	envitia::maplink::earth::geometry::Style s;

	s.edge.colour = edgeColour;
	s.edge.width = edgeWidth;

	// Extruded polylines also have a fill style, which in this case will be a translucent version of the edge style
	s.fill.colour = s.edge.colour;
	s.fill.colour.m_a = 0x22;

	if (draped) {
		// Drape polygons using the 'projection' mode - This offers perfect conformance to the surface
		// and excellent performance at the cost of not being able to offset above the surface/some slight rendering differences
		s.draping.mode = envitia::maplink::earth::geometry::Style::DrapeModeClampToTerrain;
		s.draping.technique = envitia::maplink::earth::geometry::Style::DrapeTechniqueProjection;
	}
	else if (extrusionHeight > 0.0) {
		// Extrude the geometry
		s.extrusion.flatten = true;
		s.extrusion.height = extrusionHeight;
	}

	surface->setStyle(name.c_str(), s);

	// Add a 'simple' variant
	// This style is the same as normal, but with less aggressive tesselation/accuracy settings
	// This allows the polygon/polyline creation interactions to update geometry much faster when the mouse moves,
	// and ensure geometry creation is responsive.
	envitia::maplink::earth::geometry::Style sSimple(s);
	sSimple.edge.tessellationDistance = 500000.0;
	sSimple.fill.tessellationDistance = 500000.0;
	surface->setStyle((name + "-simple").c_str(), sSimple);

	// Add a 'selected' variant
	envitia::maplink::earth::geometry::Style sSelected(s);
	if (extrusionHeight > 0.0) {
		sSelected.fill.colour.m_a = 0xff;
	}
	sSelected.edge.colour = TSLRGBA(0xff - s.edge.colour.m_r, 0xff - s.edge.colour.m_g, 0xff - s.edge.colour.m_b, 0xff);
	surface->setStyle((name + "-selected").c_str(), sSelected);
}

void GeometryManager::createTextStyle(envitia::maplink::earth::Surface3D* surface,
	std::string name, TSLRGBA textColour, unsigned int textSize, TSLRGBA backgroundColour) {
	if (!surface) return;
	envitia::maplink::earth::geometry::Style s;

	s.text.colour = textColour;
	s.text.size = textSize;
	if (backgroundColour.m_a != 0x00) {
		s.text.backgroundHalo = true;
		s.text.backgroundThickness = 2;
		s.text.backgroundColour = backgroundColour;
	}

	surface->setStyle(name.c_str(), s);

	// Add a 'selected' variant
	envitia::maplink::earth::geometry::Style sSelected(s);
	sSelected.text.colour.m_a = 0x22;
	surface->setStyle((name + "-selected").c_str(), sSelected);
}

void GeometryManager::createSymbolStyle(envitia::maplink::earth::Surface3D* surface,
	std::string name, TSLStyleID symID, unsigned int symSize, TSLRGBA symColour) {
	if (!surface) return;
	envitia::maplink::earth::geometry::Style s;

	s.symbol.id = symID;
	s.symbol.size = symSize;
	s.symbol.colour = symColour;

	surface->setStyle(name.c_str(), s);

	// Add a 'selected' variant
	envitia::maplink::earth::geometry::Style sSelected(s);
	sSelected.symbol.colour = TSLRGBA(0xcc, 0xcc, 0xcc, 0xff);
	surface->setStyle((name + "-selected").c_str(), sSelected);
}