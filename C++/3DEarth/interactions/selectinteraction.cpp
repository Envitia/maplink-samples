// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************
#ifdef WINNT
# define WIN32_LEAN_AND_MEAN
# define VC_EXTRALEAN
# include "windows.h"
#endif


#include "MapLink3DIMode.h"
#include "selectinteraction.h"

#include <algorithm>
#include <memory>

SelectInteraction::SelectInteraction(int modeID)
  : Interaction(modeID)
{}

SelectInteraction::~SelectInteraction() {}

void SelectInteraction::activate() {}
void SelectInteraction::deactivate() {}

bool SelectInteraction::onLButtonDown(TSLDeviceUnits x, TSLDeviceUnits y, bool shift, bool control) {
  auto surface = m_drawingSurface;
  if (!surface) return false;

  // First check for selection of tracks
  if (selectTrack(x, y)) return false;

  // Then of geometry
  selectGeometry(x, y);
  return true;
}

bool SelectInteraction::selectTrack(TSLDeviceUnits x, TSLDeviceUnits y) {
	auto surface = m_drawingSurface;
	if (!surface) return false;

	// When the view is clicked perform a 'pick' operation on the tracks
	// The Earth SDK provides 2 methods for picking tracks
	// - Surface3D::pickTracks: Similar to entity/datalayer picking, returns a TSLPickResultSet of tracks, ordered by the distance from the pick location
	// - Surface3D::pickTrack: Directly returns a Track pointer. Will return the closest track to the pick location
	// This method demonstrates the TSLPickResultSet based method, but either would work for this case.
	std::unique_ptr<TSLPickResultSet, TSLDestroyPointer<TSLPickResultSet>> tracks(surface->pickTracks(x, y, 10));
	if (!tracks || tracks->numResults() == 0) return false;

	earth::Track* pickedTrack = nullptr;
	// Go through the results and set their visualisation to be highlighted
	for (auto i = 0; i < tracks->numResults(); ++i) {
		auto pickResult = tracks->getResult(i);
		if (pickResult->queryType() != TSLPickEarthTrack) continue;
		auto track = reinterpret_cast<const earth::PickResultTrack*>(tracks->getResult(i));
		pickedTrack = track->track();

		// We only want the closest track to the pick, so we can break here
		// Alternatively the application could call Surface3D::pickTrack, which would obtain the same result.
		break;
	}

	if (pickedTrack && m_requestHandler)
	{
		m_requestHandler->trackSelected(pickedTrack);
		return true;
	}
	
	return false;
}
void SelectInteraction::selectGeometry(TSLDeviceUnits x, TSLDeviceUnits y) {
  auto surface = m_drawingSurface;
  if (!surface) return;

  // Perform a geometry pick operation at x,y
  auto geom = surface->pickGeometry(x, y, 10);

  if (m_requestHandler)
	  m_requestHandler->geometrySelected(geom);
}
