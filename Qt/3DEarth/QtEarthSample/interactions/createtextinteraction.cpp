// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************
#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif


#include "MapLink3DIMode.h"
#include "createtextinteraction.h"

CreateTextInteraction::CreateTextInteraction(int modeID)
  : Interaction(modeID)
{}

CreateTextInteraction::~CreateTextInteraction() {}

void CreateTextInteraction::activate() {}
void CreateTextInteraction::deactivate() {}

bool CreateTextInteraction::onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
  auto surface = m_drawingSurface;
  if (!surface) return false;

  double lat, lon;
  if (!surface->DUToLatLong(x, y, &lat, &lon)) return false;

  // Create some text
  auto text = new earth::geometry::Text("text", { lon, lat, 0.f }, "Test Text");
  if (!surface->addGeometry(*text)) {
    delete text;
    return false;
  }

  // Pass ownership to the document model
  if (m_requestHandler)
	  m_requestHandler->geometryAdded(text);

  return true;
}
