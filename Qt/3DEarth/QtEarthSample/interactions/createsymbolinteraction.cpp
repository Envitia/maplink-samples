// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************
#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif


#include "MapLink3DIMode.h"
#include "createsymbolinteraction.h"

CreateSymbolInteraction::CreateSymbolInteraction(int modeID)
  : Interaction(modeID)
{}

CreateSymbolInteraction::~CreateSymbolInteraction() {}

void CreateSymbolInteraction::activate() {}
void CreateSymbolInteraction::deactivate() {}

bool CreateSymbolInteraction::onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
  auto surface = m_drawingSurface;
  if (!surface) return false;

  double lat, lon;
  if (!surface->DUToLatLong(x, y, &lat, &lon)) return false;

  double alt = 100;
  surface->getTerrainHeight(lat, lon, alt);

  auto sym = new earth::geometry::Symbol("symbol", { lon, lat, alt });
  if (!surface->addGeometry(*sym)) {
    delete sym;
	return false;
  }

  // Pass ownership to the document model
  if (m_requestHandler)
	  m_requestHandler->geometryAdded(sym);

  return true;
}
