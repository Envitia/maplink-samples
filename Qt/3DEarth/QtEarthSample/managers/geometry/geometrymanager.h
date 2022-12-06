#pragma once

#include <vector>
#include <memory>


class GeometryManager
{
public:
	GeometryManager();
	~GeometryManager();

public:

	// Query the geometry
	std::vector<std::unique_ptr<envitia::maplink::earth::geometry::Geometry>>& getGeometry();

	// Add a geometry instance to the model
	void addGeometry(envitia::maplink::earth::geometry::Geometry* geom);

	// Delete a geometry instance from the model
	void removeGeometry(envitia::maplink::earth::geometry::Geometry& geom);
public:
	static void createStyles(envitia::maplink::earth::Surface3D* surface);
	static void switchGeometryStyle(envitia::maplink::earth::geometry::Geometry* geom);

private:
	// Setup a style on the surface
	// The style will be created with appropriate settings for the geometry type
	// In addition to the base style a '-selected' variant will be created with different rendering
	// and a '-simple' variant will be created for tesselated geometry.
	static void createPolygonStyle(envitia::maplink::earth::Surface3D* surface,
		std::string name, TSLRGBA edgeColour, float edgeWidth, TSLRGBA fillColour, bool draped, float extrusionHeight);

	static void createPolylineStyle(envitia::maplink::earth::Surface3D* surface,
		std::string name, TSLRGBA edgeColour, float edgeWidth, bool draped, float extrusionHeight);

	static void createTextStyle(envitia::maplink::earth::Surface3D* surface,
		std::string name, TSLRGBA textColour, unsigned int textSize, TSLRGBA backgroundColour);

	static void createSymbolStyle(envitia::maplink::earth::Surface3D* surface,
		std::string name, TSLStyleID symID, unsigned int symSize, TSLRGBA symColour);

private:
	// The geometry within the scene
	// Like Tracks these are owned/managed by the application
	std::vector<std::unique_ptr<envitia::maplink::earth::geometry::Geometry>> m_geometry;
};
