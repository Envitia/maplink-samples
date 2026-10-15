// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************
#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif

#include "MapLink3DIMode.h"

Interaction::~Interaction() {}

bool Interaction::onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
	return false;    // No redraw required
}

bool Interaction::onLButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
	return false;    // No redraw required
}

bool Interaction::onMButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
	return false;    // No redraw required
}

bool Interaction::onMButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
	return false;    // No redraw required
}

bool Interaction::onRButtonUp(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
	return false;    // No redraw required
}

bool Interaction::onRButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
	return false;    // No redraw required
}

bool Interaction::onMouseMove(TSLButtonType button, TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control)
{
	return false;    // No redraw required
}

bool Interaction::onMouseWheel(short delta, TSLDeviceUnits x, TSLDeviceUnits y)
{
	return false;
}

Interaction::Interaction() {}
Interaction::Interaction(int modeID)
  : m_id(modeID)
{}

int Interaction::id()
{
	return m_id;
}

void Interaction::setInteractionModeRequest(InteractionModeRequest* requestHandler)
{
	m_requestHandler = (requestHandler);
}

void Interaction::setDrawingSurface(envitia::maplink::earth::Surface3D* drawingSurface)
{
	m_drawingSurface = (drawingSurface);
}