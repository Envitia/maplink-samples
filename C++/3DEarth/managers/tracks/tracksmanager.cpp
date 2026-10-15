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

#include "tracksmanager.h"

#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>


TracksManager::TracksManager()
{
  m_trackMaxDegPerSecond = 1.0;
  m_trackMaxMetresPerSecond = 50.0;
}

TracksManager::~TracksManager()
{

}

std::vector<envitia::maplink::earth::Track>& TracksManager::getTracks() {
  return m_tracks;
}

envitia::maplink::earth::TrackSymbol* TracksManager::getTrackSymbolForTrackIdx(unsigned int trackIdx)
{
  if (m_trackSymbolsUsed.size() <= trackIdx)
    return nullptr;

  auto trackSymbolUsed = m_trackSymbolsUsed[trackIdx];
  return &m_trackSymbols[trackSymbolUsed];
}

bool TracksManager::selectTrack(envitia::maplink::earth::Track* pickedTrack)
{
	auto& docTracks = getTracks();

	// check if the track is already selected
	bool isAlreadySelected = false;
	if (m_lastPickedTrack != -1 && m_lastPickedTrack < docTracks.size())
	{
		if (&docTracks[m_lastPickedTrack] == pickedTrack)
			isAlreadySelected = true;
	}

	// Reset any previously selected tracks to unhighlighted
	if (auto* trackSym = getTrackSymbolForTrackIdx(m_lastPickedTrack)) {
		docTracks[m_lastPickedTrack].symbol(*trackSym);
	}
	m_lastPickedTrack = -1;

	// unselect it and return if already selected
	if (isAlreadySelected)
		return false;

	// Find where the picked track is in the document's tracks vector
	auto docTrackIt = std::find_if(docTracks.begin(), docTracks.end(),
		[&docTracks, &pickedTrack](const envitia::maplink::earth::Track& t) { return &t == pickedTrack; });
	if (docTrackIt != docTracks.end()) {
		int trackIdx = std::distance(docTracks.begin(), docTrackIt);

		m_lastPickedTrack = trackIdx;
		// Set the track to use the highlighted version of its symbol visualisation
		if (auto* trackSym = getHighlightTrackSymbolForTrackIdx(trackIdx)) {
			docTracks[trackIdx].symbol(*trackSym);
		}
	}

	return m_lastPickedTrack != -1;
}

envitia::maplink::earth::TrackSymbol* TracksManager::getHighlightTrackSymbolForTrackIdx(unsigned int trackIdx)
{
  if (m_trackSymbolsUsed.size() <= trackIdx)
    return nullptr;

  auto trackSymbolUsed = m_trackSymbolsUsed[trackIdx];
  return &m_highlightedTrackSymbols[trackSymbolUsed];
}

int TracksManager::getLastPickedTrack()
{
	return m_lastPickedTrack;
}

void TracksManager::initialiseTracks(unsigned int numToCreate) {
  std::random_device rd;
  std::mt19937 mt(rd());

  if (m_trackSymbols.empty())
  {
    // Create some track symbols
    // Each symbol is a possible visualisation of a track, such as an icon of an aircraft
    // These visualisations may contain any maplink entities, including raster, vector, or military symbology
    // These visualisations should be considered as mostly static, however may contain dynamic text entities
    // which can use the Track's dataset to display data such as the track's position or other per-track attributes

    // Red Triangle
    createTrackSymbol(5, 25, TSLRGB(0xff, 0x00, 0x00));
    // Green Aeroplane
    createTrackSymbol(6002, 30, TSLRGB(0x00, 0xdd, 0x00));
    // Purple Arrow
    createTrackSymbol(7, 25, TSLRGB(0xaa, 0x00, 0xff));
    // Yellow Star
    createTrackSymbol(14, 25, TSLRGB(0xff, 0xaa, 0x00));
    // Blue Circled Aeroplane
    createTrackSymbol(6003, 34, TSLRGB(0x00, 0x00, 0xff));
    // Light Blue Aeroplane (Raster symbol, has its own colour)
    createTrackSymbol(20800, 25, TSLRGB());
  }

  // Create some tracks
  // Each track is a representation of an object in the world, and will be rendered based on its assigned symbol
  std::uniform_int_distribution<unsigned int> randomSymbol(0, m_trackSymbols.size() - 1);
  std::uniform_real_distribution<double> randomLat(-90.0, 90.0);
  std::uniform_real_distribution<double> randomLon(-180.0, 180.0);
  std::uniform_real_distribution<double> randomAlt(50.0, 100000.0);

  envitia::maplink::earth::GeodeticDirection north = { 0, 1.0, 0 };

  std::uniform_real_distribution<double> randomMoveDegrees(-m_trackMaxDegPerSecond, m_trackMaxDegPerSecond);
  std::uniform_real_distribution<double> randomMoveMeters(-m_trackMaxMetresPerSecond, m_trackMaxMetresPerSecond);

  for (unsigned int i = 0; i < numToCreate; ++i) {
    // Create a Track, with a random visualisation from m_trackSymbols
    auto trackSymIndex = randomSymbol(mt);
    if (trackSymIndex >= m_trackSymbols.size()) continue;
    auto& trackSym = m_trackSymbols[trackSymIndex];
	envitia::maplink::earth::Track track(trackSym);

    // Position the track at a random location
    track.position(envitia::maplink::earth::GeodeticPoint(randomLon(mt), randomLat(mt), randomAlt(mt)));

    // Set a random velocity
    auto x = randomMoveDegrees(mt);
    auto y = randomMoveDegrees(mt);
    auto z = randomMoveMeters(mt);
	envitia::maplink::earth::GeodeticDirection vel(x, y, z);
    m_trackVelocities.emplace_back(vel);

    //calculate heading angle
    //Calculate the angle between direction vectors
    float dot = vel.x()*north.x() + vel.y()*north.y();      // dot product between[x1, y1] and [x2, y2]
    float det = vel.x()*north.y() - vel.y()*north.x();      // determinant
    double angle = atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)
    double rad2deg = 180.0 / 3.14159;
    track.rotation(angle*rad2deg);

    // Store the track
    m_tracks.emplace_back(track);

    // Store which visualisation we used for this track
    m_trackSymbolsUsed.emplace_back(trackSymIndex);
  }

  m_lastTrackUpdateTime = std::chrono::system_clock::now();
}

void TracksManager::removeTracks()
{
  m_tracks.clear();
  m_trackSymbolsUsed.clear();
  m_trackVelocities.clear();
}

void TracksManager::createTrackSymbol(TSLStyleID symbolID, unsigned int pxSize, TSLRGB symbolColour) {
	envitia::maplink::earth::TrackSymbol trackSym;

  // Setup the visualisation to use a 2D maplink symbol (raster or vector)
  TSLSymbol* sym = TSLSymbol::create(1, 0, 0);
  TSLRenderingAttributes attribs;
  attribs.m_symbolColour = symbolColour.composeRGB();
  attribs.m_symbolSizeFactor = static_cast<double>(pxSize);
  attribs.m_symbolSizeFactorUnits = TSLDimensionUnitsPixels;
  attribs.m_symbolStyle = symbolID;
  attribs.m_symbolScalable = TSLRasterSymbolScalableEnabled;
  attribs.m_symbolRotatable = TSLSymbolRotation::TSLSymbolRotationEnabled;
  sym->setRendering(attribs);

  // Add the 2D symbol to the track symbol
  // Note that the track symbol will copy the provided entity
  trackSym.addEntity(sym);
  sym->destroy();

  // Store the track symbol for later use
  // When passed to a track the track symbol will be copied,
  // so the TrackSymbols may be deleted independently of the Tracks themselves.
  m_trackSymbols.emplace_back(trackSym);

  //Create a highlighted version of this symbol for use as a selection marker
  createTrackHighlightSymbol(attribs);
}

void TracksManager::createTrackHighlightSymbol(TSLRenderingAttributes symAttribs)
{
	envitia::maplink::earth::TrackSymbol highlightTrackSym;

  // Create annotation definitions in order to display the track's position and speed
	envitia::maplink::earth::TrackSymbol::AnnotationAttributes posAnnotation;
  posAnnotation.offsetY = -30;
  posAnnotation.size = 15;
  posAnnotation.fillColour = TSLRGBA(0xcc, 0xcc, 0xcc, 0xff);
  posAnnotation.backgroundHalo = true;
  posAnnotation.backgroundColour = TSLRGBA(0x22, 0x22, 0x22, 0xff);
  posAnnotation.horizontalAlignment = TSLHorizontalAlignmentCentre;
  posAnnotation.verticalAlignment = TSLVerticalAlignmentMiddle;
  highlightTrackSym.addAnnotation("position", posAnnotation);

  auto speedAnnotation = posAnnotation;
  speedAnnotation.offsetY = 30;
  highlightTrackSym.addAnnotation("speed", speedAnnotation);

  // Setup a TrackSymbol like the first, but also containing a highlight rectangle

  // Polygon coordinates for the highlight box
  std::unique_ptr<TSLCoordSet, TSLDestroyPointer<TSLCoordSet>> boxCoords(new TSLCoordSet());
  boxCoords->add(-500, -500);
  boxCoords->add(500, -500);
  boxCoords->add(500, 500);
  boxCoords->add(-500, 500);

  // Create the box in the entity set and setup its rendering attributes
  std::unique_ptr<TSLPolygon, TSLDestroyPointer<TSLPolygon>> box(TSLPolygon::create(1, *boxCoords.get()));
  TSLRenderingAttributes boxAttribs;
  boxAttribs.m_exteriorEdgeColour = TSLRGB(0xff, 0xff, 0x00).composeRGB();
  boxAttribs.m_exteriorEdgeStyle = 1;
  boxAttribs.m_exteriorEdgeThickness = 3;
  boxAttribs.m_exteriorEdgeThicknessUnits = TSLDimensionUnitsPixels;
  box->setRendering(boxAttribs);

  // Create a symbol in the entity set and use the attribs from the main symbol
  std::unique_ptr<TSLSymbol, TSLDestroyPointer<TSLSymbol>> symbol(TSLSymbol::create(1, 0, 0, 100, 0));
  symbol->setRendering(symAttribs);

  // Add the entities into the visualisation
  highlightTrackSym.addEntity(box.get());
  highlightTrackSym.addEntity(symbol.get());

  // Store the highlighted track symbol for later use
  m_highlightedTrackSymbols.emplace_back(highlightTrackSym);
}

void TracksManager::resetLastTrackUpdateTime() {
	m_lastTrackUpdateTime = std::chrono::system_clock::now();
}

void TracksManager::updateTracks() {

	if (m_tracks.empty()) return;
  auto now = std::chrono::system_clock::now();
  auto updateDelta = now - m_lastTrackUpdateTime;
  m_lastTrackUpdateTime = now;
  auto deltaSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(updateDelta).count() / 1000.0;
  // Perform an update operation on each of the tracks
  // Note: Performing this update for every track, every frame
  // will have a performance impact for a large number of tracks.
  std::random_device rd;
  std::mt19937 mt(rd());

  //Select a random track to change direction this frame
  std::uniform_int_distribution<unsigned int> randomTrackIdx(0, m_tracks.size()+40);
  unsigned int trackToChange = randomTrackIdx(mt);

  std::uniform_real_distribution<double> randomMoveDegrees(-m_trackMaxDegPerSecond, m_trackMaxDegPerSecond);
  std::uniform_real_distribution<double> randomMoveMeters(-m_trackMaxMetresPerSecond, m_trackMaxMetresPerSecond);
  for (int trackIdx = 0; trackIdx < m_tracks.size(); trackIdx++) {
    auto& t = m_tracks[trackIdx];
    auto& vel = m_trackVelocities[trackIdx];

    auto pos = t.position();

    // If this is the randomly selected track, change direction
    if (trackIdx == trackToChange)
    {
      // Set a random velocity
      auto x = randomMoveDegrees(mt);
      auto y = randomMoveDegrees(mt);
      auto z = randomMoveMeters(mt);
      vel = { x, y, z };

      //calculate heading angle
	  envitia::maplink::earth::GeodeticDirection north = { 0, 1.0, 0 };

      //Calculate the angle between direction vectors
      float dot = vel.x()*north.x() + vel.y()*north.y();      // dot product between[x1, y1] and [x2, y2]
      float det = vel.x()*north.y() - vel.y()*north.x();      // determinant
      double angle = atan2(det, dot);  // atan2(y, x) or atan2(sin, cos)
      double rad2deg = 180.0 / 3.14159;
      t.rotation(angle*rad2deg);
    }

    // Move the track by its velocity
     pos += vel*deltaSeconds;
    if (bool high = (pos.x() > 180.0) || pos.x() < -180.0)
    {
      if (high) pos.x(180.0);
      else pos.x(-180.0);
      vel.x(vel.x()*-1);
    }
    if (bool high = (pos.y() > 90.0) || pos.y() < -90.0)
    {
      if (high) pos.y(90.0);
      else pos.y(-90.0);
      vel.y(vel.y()*-1);
    }
    // Stop it going into space or underground
    if (bool high = (pos.z() > 5000.0) || pos.z() <= 0.0) {
      if (high) pos.z(5000.0);
      else pos.z(0.0);
      vel.z(0);
    }
    t.position(pos);

    // Update the tracks position and speed annotation
    std::stringstream str;
    str << std::fixed << std::setw(2) << std::setprecision(4) << pos.y() << "," << pos.x();
    t.setAttribute("position", str.str().c_str());
    str.str("");
    auto velCopy = vel;
    // Remove the height coord to just get horizontal velocity in degrees per second
    velCopy.z(0);
    str << std::setw(2) << std::setprecision(2) << velCopy.length() << " deg/s";
    t.setAttribute("speed", str.str().c_str());
  }
}