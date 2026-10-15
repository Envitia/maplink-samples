// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************
#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif


#include "MapLink3DIMode.h"
#include "deletegeometryinteraction.h"

DeleteGeometryInteraction::DeleteGeometryInteraction(int modeID)
  : Interaction(modeID)
{}

DeleteGeometryInteraction::~DeleteGeometryInteraction() {}

void DeleteGeometryInteraction::activate() {}
void DeleteGeometryInteraction::deactivate() {}

bool DeleteGeometryInteraction::onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
  auto surface = m_drawingSurface;
  if (!surface) return false;

  // Perform a geometry pick operation at x,y
  auto geom = surface->pickGeometry(x, y, 10);
  if (!geom) return false;

  // Remove the geometry from the surface
  surface->removeGeometry(*geom);

  // And delete the geometry from the document model (Delete it)
  if (m_requestHandler)
	  m_requestHandler->geometryRemoved(geom);

  return true;
}
