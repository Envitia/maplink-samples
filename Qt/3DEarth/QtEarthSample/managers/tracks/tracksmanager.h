#pragma once

#include <vector>
#include <memory>
#include <chrono>


class TracksManager
{
public:
	TracksManager();
	~TracksManager();

public:
  // Initialise the track simulation
  void initialiseTracks(unsigned int numToCreate);

  // Stop the simulation and remove all the tracks
  // Only to be called by the View once the tracks have been removed from the drawing surface.
  void removeTracks();

  // Create a TrackSymbol, and add it to m_trackSymbols
  void createTrackSymbol(TSLStyleID symbolID, unsigned int pxSize, TSLRGB symbolColour);
  // Called under createTrackSymbol to create a highlighted version of that symbol
  void createTrackHighlightSymbol(TSLRenderingAttributes symAttribs);

  // Update the tracks
  void updateTracks();

  // Query the list of tracks
  std::vector<envitia::maplink::earth::Track>& getTracks();

  // Query the visualisation used for the Track of this index
  // @param trackIdx - the index in the vector given by getTracks()
  envitia::maplink::earth::TrackSymbol* getTrackSymbolForTrackIdx(unsigned int trackIdx);

  bool selectTrack(envitia::maplink::earth::Track* pickedTrack);

  // Query the highlighted visualisation used for the Track of this index
  // @param trackIdx - the index in the vector given by getTracks()
  envitia::maplink::earth::TrackSymbol* getHighlightTrackSymbolForTrackIdx(unsigned int trackIdx);

  // get the last picked track
  int getLastPickedTrack();

  // reset last updated track time to now in order to resume the tracks if paused
  void resetLastTrackUpdateTime();

private:
  // The tracks displayed within the scene
  // These are owned/managed by the application
  // - A TrackSymbol specifies the rendering of a track
  // - One TrackSymbol may be used by many Tracks
  // - Each Track represents an object in the world such as a unit or vehicle
  std::vector<envitia::maplink::earth::TrackSymbol> m_trackSymbols;
  std::vector<envitia::maplink::earth::TrackSymbol> m_highlightedTrackSymbols;
  std::vector<envitia::maplink::earth::Track> m_tracks;
  std::vector<int> m_trackSymbolsUsed;
  std::vector<envitia::maplink::earth::GeodeticDirection> m_trackVelocities;
  float m_trackMaxDegPerSecond;
  float m_trackMaxMetresPerSecond;

  int m_lastPickedTrack = -1;

  std::chrono::time_point<std::chrono::system_clock> m_lastTrackUpdateTime;
};
