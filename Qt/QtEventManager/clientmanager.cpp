/****************************************************************************
                Copyright (c) 2008-2018 by Envitia Group PLC.
****************************************************************************/

#include <math.h>       /* atan2 */
#include <QMessageBox>
#include <qtooltip.h>
#include <sstream>

#include "clientmanager.h"

//! maximum number of reverse times when searching for selected track's metadata.
#define maxReverse (10)

//! margin lat-long radius to search for the track given the clicked mouse's lat-long.
#define queryTrackMargin (0.005)//(0.001)

//! The name of our track manager
const char * ClientManager::m_trackManagerName = "TDM1MC1";

//! static counter used to set track number for track manager.
int ClientManager::trackNumbersCounter = 0;

//! convert types[int/double] into string
template<class T>
string toString(const T &val)
{
  stringstream stream;
  stream << val;
  return stream.str();
}

//! split string into small tokens
static void split(const string& s, char delim, std::vector<string>& tokens)
{
  // start with an empty result set
  tokens.clear();

  size_t chunkBegin = 0;

  for (;;)
  {
    size_t chunkEnd = s.find_first_of(delim, chunkBegin);

    //! we don't check for end of string until later, so that we include the final token
    //! extract the token between chunkBegin and chunkEnd (or chunkBegin and the end of the string)
    size_t chunkCount = (chunkEnd == string::npos) ? string::npos : (chunkEnd - chunkBegin);
    string token(s, chunkBegin, chunkCount);

    tokens.push_back(move(token));

    //! check for end of string
    if (chunkEnd == string::npos)
    {
      break;
    }

    chunkBegin = 1 + chunkEnd;
  }
}

#ifndef WINNT
ClientManager::ClientManager(QWidget *parent, TSLMotifSurface *drawingSurface)
#else
ClientManager::ClientManager(QWidget *parent, TSLNTSurface *drawingSurface)
#endif
  : m_parentWidget(parent),
  m_drawingSurface(drawingSurface),

  m_trackManager(NULL),
  m_currentTime(0)
{
}

ClientManager::~ClientManager()
{
}

//! create and initialize the track manager
void ClientManager::createTrackManager()
{
  // create track manager and attach the drawing surface to it.
  if (m_trackManager == NULL)
  {
    m_trackManager = TSLTrackDisplayManager::create();
    uint32_t trackId = m_trackManager->addDrawingSurface(m_drawingSurface, m_trackManagerName);
    m_surfaceId = m_drawingSurface ? -m_drawingSurface->id() : 0;
    if (trackId != m_surfaceId)
    {
      QMessageBox::critical(m_parentWidget, "Track Manager Error", "Failed to create track manager");
    }

    //! set the history points (number of history points, history points type).
    m_trackManager->numHistoryPoints(2);
    m_trackManager->historyPointType(TSLTrackDisplayManager::HistoryPointType::HistoryPointTypeSquare);

    //! create selection symbol (symbol around the track when it is selected).
    m_selectionSymbolTemplate = createSelectionSymbolTemplate();

    //! create template point symbol to be used if military symbol is not valid/supported
    m_pointSymbolTemplate = createPointSymbolTemplate(2);

    //! setup history symbol
    m_trackManager->historicDataExpiry(30);
    //m_trackManager->historyPointDistance(50000);
    m_trackManager->numHistoryPoints(200);

    //! create history track symbol template to be reused.
    m_historySymbolTemplate = createHistorySymbolTemplate(true);

    m_trackManager->historyPointSymbol(reinterpret_cast<TSLTrackHistorySymbol*>(m_historySymbolTemplate->clone()));
    m_trackManager->historyPointType(TSLTrackDisplayManager::HistoryPointTypeNone);
  }
}

//////////////////////////////////////// Symbols Templates////////////////////////////////////////

//! create military track symbol template to be reused[if symbol template was previously created, re-use it].
TSLTrackSymbol* ClientManager::createMilitarySymbolTemplate(const string &symbolID, TSLTrackMilitarySymbol::Specification spec, TSLTrackMilitarySymbol::SpecificationTypeID specTypeID)
{
  //! if symbol template was created previously, reuse it.
  if (m_savedSymbolTemplates.count(symbolID) > 0)
  {
    return m_savedSymbolTemplates[symbolID];
  }

  //! create track symbol (military symbol)
  TSLTrackMilitarySymbol* symbol = TSLTrackMilitarySymbol::createSIDC(symbolID.c_str(), spec, specTypeID);

  symbol->size(30);
  symbol->sizeUnits(TSLTrackSymbol::SizeUnitsPoints);
  //symbol->hostility(TSLTrackMilitarySymbol::HostilityFriend);
  symbol->isFramed(true);

  //symbol->displayLatAndLon(true);
  //symbol->displaySpeed(true);
  //symbol->displaySpeedUnits("m/s");
  //symbol->altitudeOrDepth("-50k leagues");

  //! set the selection symbol to the track symbol.  
  TSLTrackSelectionSymbol* selectSymbol = (TSLTrackSelectionSymbol*)m_selectionSymbolTemplate->clone();
  symbol->selectionSymbol(selectSymbol);

  m_savedSymbolTemplates[symbolID] = symbol;
  return m_savedSymbolTemplates[symbolID];
}

//! create selection track symbol template to be reused.(symbol around the track when it is selected).
TSLTrackSelectionSymbol* ClientManager::createSelectionSymbolTemplate()
{
  TSLTrackSelectionSymbol* symbol = TSLTrackSelectionSymbol::create();
  symbol->colour(0x0000ffff);
  symbol->size(40);
  symbol->sizeUnits(TSLTrackSymbol::SizeUnits::SizeUnitsPixels);
  symbol->symbolID(3);

  return symbol;
}

//! create history track symbol template to be reused.
TSLTrackHistorySymbol* ClientManager::createHistorySymbolTemplate(bool showHistorySquare)
{
  TSLTrackHistorySymbol* symbol = (TSLTrackHistorySymbol::create());
  m_showHistorySquare = showHistorySquare;
  if (m_showHistorySquare)
  {

    symbol->colour(0xff00ffff);
    symbol->size(35);
    symbol->sizeUnits(TSLTrackSymbol::SizeUnitsPixels);
    symbol->symbolID(1);
  }
  else
  {
    symbol->symbolID(2);
    symbol->colour(0xff0000ff);
    symbol->size(40);
    symbol->sizeUnits(TSLTrackSymbol::SizeUnitsPixels);
  }

  return symbol;
}

//! Add entity to the point symbol
void ClientManager::addEntityToPointSymbol(TSLTrackPointSymbol * symbol, int symbolIDVal, TSLRGBA colour)
{
  TSLSymbol* sym = TSLSymbol::create(0, 0, 0);
  TSLRenderingAttributes attribs;
  attribs.m_symbolStyle = symbolIDVal;
  attribs.m_symbolColour = colour.composeRGB();
  attribs.m_symbolSizeFactor = 50;
  attribs.m_symbolSizeFactorUnits = TSLDimensionUnitsPixels;
  attribs.m_symbolOpacity = 32767;
  attribs.m_symbolScalable = TSLRasterSymbolScalableEnabled;
  attribs.m_symbolRotatable = TSLSymbolRotationDisabled;
  sym->setRendering(attribs);
  symbol->addSymbolEntity(sym);
  sym->destroy();
}

//! create point track symbol template to be reused.
TSLTrackPointSymbol* ClientManager::createPointSymbolTemplate(int symbolIDVal)
{
  string symbolID = toString(symbolIDVal);
  //! create track symbol (point symbol)
  TSLTrackPointSymbol * symbol = (TSLTrackPointSymbol::create());
  addEntityToPointSymbol(symbol, symbolIDVal, TSLRGBA(0x00, 0xff, 0x00, 0xff));

  //! set the selection symbol to the track symbol.  
  TSLTrackSelectionSymbol* selectSymbol = (TSLTrackSelectionSymbol*)m_selectionSymbolTemplate->clone();
  symbol->selectionSymbol(selectSymbol);

  return symbol;
}

//////////////////////////////////////// Display tracks ////////////////////////////////////////
//! clone symbol template, create a display track, and add the track to the track manager.
bool ClientManager::createDisplayTrack(TSLTrackSymbol* symbolTemplate, const string& trackId, int trackNumber)
{
  //! clone symbol template.
  m_displayingTracks[trackId].m_symbol = symbolTemplate->clone();

  //! create track using the cloned symbol template.
  m_displayingTracks[trackId].m_track = TSLTrack::create(m_displayingTracks[trackId].m_symbol);

  //! add track to track manager.
  bool validtrack = m_trackManager->addTrack(trackNumber, m_displayingTracks[trackId].m_track);

  return validtrack;
}

//////////////////////////////////////// Selected tracks ////////////////////////////////////////

//! decode (TrackedItem) object into metadata key-value pairs.
bool ClientManager::decodeMetadata(const TrackedItem &updatedtrackedItem, std::vector<std::pair<string, string>> &metadatPairs)
{
  for (const auto& metaInfo : updatedtrackedItem.m_metaValue)
  {
    string metadata = metaInfo.second.m_metadata;
    string tempStr;

    //! remove first '{'
    size_t found = metadata.find('{');
    if (found != 0)
    {
      return false;
    }
    tempStr = metadata.substr(1, metadata.size() - 1);

    //! remove last '}'
    found = tempStr.find('}');
    if (found != tempStr.size() - 1)
    {
      return false;
    }
    metadata = tempStr.substr(0, found);

    //! remove extra '\"'
    std::string::iterator end_pos = std::remove(metadata.begin(), metadata.end(), '\"');
    metadata.erase(end_pos, metadata.end());

    //! decode the pairs
    std::vector<string> tokens;
    split(metadata, ',', tokens);
    for (const auto& token : tokens)
    {
      std::vector<string> keyVal;
      split(token, ':', keyVal);
      if (keyVal.size() == 2)
      {
        metadatPairs.push_back(make_pair(keyVal[0], keyVal[1]));
      }
    }
  }
  return true;
}

//! decode (TracksDelivery) object for the selected track into metadata key-value pairs.
void ClientManager::getSelectedTrackMetadata(const TracksDelivery &updatedtracksDelivery, const string &selectedTrackId, std::vector<std::pair<string, string>> &metadatPairs)
{
  metadatPairs.push_back(make_pair("Source", updatedtracksDelivery.m_sourceid));
  if (updatedtracksDelivery.m_compressedUpdates.count(selectedTrackId) > 0)
  {
    const CompressedUpdate& track = updatedtracksDelivery.m_compressedUpdates.at(selectedTrackId);
    metadatPairs.push_back(make_pair("ID", track.m_id));
    metadatPairs.push_back(make_pair("Hostility", track.m_aff));
    metadatPairs.push_back(make_pair("Symbol", track.m_sym));

    metadatPairs.push_back(make_pair("Latitude", toString(track.m_y)));
    metadatPairs.push_back(make_pair("Longitude", toString(track.m_x)));
    metadatPairs.push_back(make_pair("Velocity", toString(track.m_s)));
    metadatPairs.push_back(make_pair("Direction X", toString(track.m_dX)));
    metadatPairs.push_back(make_pair("Direction Y", toString(track.m_dY)));
  }
}

//! decode (TrackedItem) object for the selected track into metadata key-value pairs.
void ClientManager::getSelectedTrackMetadata(const TrackedItem &updatedtrackedItem, std::vector<std::pair<string, string>> &metadatPairs)
{
  metadatPairs.push_back(make_pair("Source", updatedtrackedItem.m_sourceid));
  if (!updatedtrackedItem.m_app6Value.empty())
  {
    const App6AEventType& updateEvent = updatedtrackedItem.m_app6Value.rbegin()->second;
    metadatPairs.push_back(make_pair("ID", updateEvent.m_id));
    metadatPairs.push_back(make_pair("Hostility", updateEvent.m_affiliation));
    metadatPairs.push_back(make_pair("Symbol", updateEvent.m_app6ACode));
  }

  if (!updatedtrackedItem.m_positionValue.empty())
  {
    int reverseCounter = 0;
    bool positionRead = false, directionRead = false;
    for (auto it = updatedtrackedItem.m_positionValue.rbegin(); it != updatedtrackedItem.m_positionValue.rend(); ++it)
    {
      const PositionEventType& positionEvent = it->second;

      //! read position
      if (!positionRead)
      {
        if (positionEvent.m_pos.m_x != 0 || positionEvent.m_pos.m_y != 0)
        {
          metadatPairs.push_back(make_pair("Latitude", toString(positionEvent.m_pos.m_y)));
          metadatPairs.push_back(make_pair("Longitude", toString(positionEvent.m_pos.m_x)));
          positionRead = true;
        }
      }

      //! read direction
      if (!directionRead)
      {
        if (positionEvent.m_vel.m_speed != 0 || positionEvent.m_vel.m_direction.m_x != 0 || positionEvent.m_vel.m_direction.m_y != 0)
        {
          metadatPairs.push_back(make_pair("Velocity", toString(positionEvent.m_vel.m_speed)));
          metadatPairs.push_back(make_pair("Direction X", toString(positionEvent.m_vel.m_direction.m_x)));
          metadatPairs.push_back(make_pair("Direction Y", toString(positionEvent.m_vel.m_direction.m_y)));
          directionRead = true;
        }
      }

      //! check if maximum reverse
      if (++reverseCounter > maxReverse)
      {
        break;
      }

      //! check if position and direction are done.
      if (positionRead && directionRead)
      {
        break;
      }
    }
  }
}

//////////////////////////////////////// Record history tracks ////////////////////////////////////////

//! set the record tracks history and set The maximum period to store historic Track data for. 
//!
//! Data older than (currentTime - historicDataExpiry) will be deleted.
void ClientManager::setRecordTracks(bool recordTracksHistory, int historicDataExpiryTime)
{
  m_recordTracksHistory = recordTracksHistory;
  m_trackManager->historicDataExpiry(historicDataExpiryTime);
}

//! get current time when recording history.
int ClientManager::getCurrentTime()
{
  return m_currentTime;
}

//! Set the display time for the manager.
void ClientManager::displayTime(int time)
{
  m_trackManager->displayTime(time);
}

//! Clear all history points if not in tracks mode.
void ClientManager::clearHistoryPoints()
{
  //! if we are recording history points, don't clear the history.
  if (!m_recordTracksHistory)
  {
    m_trackManager->clearAllHistoryPoints();
  }

  m_trackManager->clearAllTrackSelections(m_surfaceId);
  m_trackManager->historyPointType(TSLTrackDisplayManager::HistoryPointTypeNone);

  if (m_displayingTracks.count(m_selectedTrackId) > 0 && m_trackManager != NULL)
  {
    m_displayingTracks[m_selectedTrackId].m_track->historyPointsVisible(m_surfaceId, false);
    redrawSurface();
  }
  m_selectedTrackId = "";
}

//////////////////////////////////////// Client connection Thread ////////////////////////////////////////

//! set client connection Thread
void ClientManager::setClientConnectionThread(ClientConnectionThread *_clientConnectionThread)
{
  m_clientConnectionThread = _clientConnectionThread;
}

////////////////// Tracks Updated //////////////////

//! handles tracks updated slot sent by the thread
void ClientManager::onTracksUpdated(std::vector<std::pair<string, string>> &metadatPairs)
{
  m_clientConnectionThread->m_mutexTracks.lock();

  if (m_recordTracksHistory)
  {
    m_trackManager->currentTime(++m_currentTime);
  }

  //! Process and update the track manager with the updated tracks.
  processUpdatedTracks(m_clientConnectionThread->m_tracksDelivery, metadatPairs);

  //! redraw the drawing surface.
  redrawSurface();

  m_clientConnectionThread->m_mutexTracks.unlock();
}

//! decode hostility string into TSLTrackMilitarySymbol::Hostility enum value
TSLTrackMilitarySymbol::Hostility ClientManager::decodeHostility(const string & affCode)
{
  TSLTrackMilitarySymbol::Hostility hostility;
  if (affCode == "unknown")
  {
    hostility = (TSLTrackMilitarySymbol::HostilityUnknown);
  }
  else if (affCode == "friend")
  {
    hostility = (TSLTrackMilitarySymbol::HostilityFriend);
  }
  else if (affCode == "hostile")
  {
    hostility = (TSLTrackMilitarySymbol::HostilityHostile);
  }
  else if (affCode == "neutral")
  {
    hostility = (TSLTrackMilitarySymbol::HostilityNeutral);
  }
  else if (affCode == "none")
  {
    hostility = (TSLTrackMilitarySymbol::HostilityNone);
  }
  else if (affCode == "pending")
  {
    hostility = (TSLTrackMilitarySymbol::HostilityPending);
  }
  else if (affCode == "assumedfriend")
  {
    hostility = (TSLTrackMilitarySymbol::HostilityAssumedFriend);
  }
  else if (affCode == "suspect")
  {
    hostility = (TSLTrackMilitarySymbol::HostilitySuspect);
  }
  else if (affCode == "joker")
  {
    hostility = (TSLTrackMilitarySymbol::HostilityJoker);
  }
  else if (affCode == "faker")
  {
    hostility = (TSLTrackMilitarySymbol::HostilityFaker);
  }
  return hostility;
}

//! Process and update the track manager with the updated tracks.
void ClientManager::processUpdatedTracks(const TracksDelivery &updatedtracksDelivery, std::vector<std::pair<string, string>> &metadatPairs)
{
  //! Check if some tracks are removed from the existing tracks.
  std::vector<string> removedTracks;
  for (auto it = m_displayingTracks.begin(); it != m_displayingTracks.end(); ++it)
  {
    const string& trackId = it->first;
    if (updatedtracksDelivery.m_compressedUpdates.count(trackId) == 0)
    {
      //! add the track id to removed tracks vector.
      removedTracks.push_back(trackId);
    }
  }

  //! erase the removed tracks[If any] from the track manager and the displaying tracks.
  for (auto it = removedTracks.begin(); it != removedTracks.end(); ++it)
  {
    const string& trackId = *it;
    // check if the track id is being displayed
    if (m_displayingTracks.count(trackId) > 0)
    {
      //! remove track from track manager
      m_trackManager->removeTrack(m_displayingTracks[trackId].m_trackNumber);
      //! erase it from displaying tracks.
      m_displayingTracks.erase(trackId);
    }
  }

  //! If some update tracks are new, create them, then modify the existing displaying tracks.
  for (auto it = updatedtracksDelivery.m_compressedUpdates.begin(); it != updatedtracksDelivery.m_compressedUpdates.end(); ++it)
  {
    const string& trackId = it->first;
    const CompressedUpdate& updatedTrackIndo = it->second;

    //! If this track does not exist, create it.
    if (m_displayingTracks.count(trackId) == 0)
    {
      //! create new track number
      int trackNumber = ++trackNumbersCounter;
      m_displayingTracks[trackId].m_trackNumber = trackNumber;

      //! create track symbol (military symbol) 
      string symbolID = updatedTrackIndo.m_sym;
      m_displayingTracks[trackId].m_sym = symbolID;
      TSLTrackSymbol* symbol = createMilitarySymbolTemplate(symbolID, TSLTrackMilitarySymbol::SpecificationAPP6A, TSLTrackMilitarySymbol::SIDCSpecificationType);

      //! clone symbol template, create a display track, and add the track to the track manager.
      bool validtrack = createDisplayTrack(symbol, trackId, trackNumber);
      //! If invalid track symbol, display point track instead
      if (!validtrack)
      {
        //! remove track from track manager
        m_trackManager->removeTrack(trackNumber);

        //! if invalid/unsupported track symbol, use the point symbol.
        m_savedSymbolTemplates[symbolID] = m_pointSymbolTemplate;

        //! clone symbol template, create a display track, and add the track to the track manager.
        validtrack = createDisplayTrack(m_savedSymbolTemplates[symbolID], trackId, trackNumber);
      }
    }

    //! update symbol
    TSLTrackMilitarySymbol* symbol = reinterpret_cast<TSLTrackMilitarySymbol*>(m_displayingTracks[trackId].m_symbol);
    if (symbol != NULL && symbol->type() == TSLTrackSymbol::MilitarySymbol)
    {
      bool isSymbolChanged = false;

      //! if track's symbol has changed, update the symbol.
      if (m_displayingTracks[trackId].m_sym != updatedTrackIndo.m_sym)
      {
        m_displayingTracks[trackId].m_sym = updatedTrackIndo.m_sym;
        TSLTrackSymbol* tempSymbol = createMilitarySymbolTemplate(m_displayingTracks[trackId].m_sym, TSLTrackMilitarySymbol::SpecificationAPP6A, TSLTrackMilitarySymbol::SIDCSpecificationType);

        //!< may cause memory leak if the old symbol is not deleted.[remove the track->add it again]
        //m_displayingTracks[trackId].m_symbol = tempSymbol->clone();

        //! remove track from track manager
        m_trackManager->removeTrack(m_displayingTracks[trackId].m_trackNumber);
        //! clone symbol template, create a display track, and add the track to the track manager.
        bool validtrack = createDisplayTrack(tempSymbol, trackId, m_displayingTracks[trackId].m_trackNumber);

        symbol = reinterpret_cast<TSLTrackMilitarySymbol*>(m_displayingTracks[trackId].m_symbol);
        isSymbolChanged = true;
      }

      //! if track's affliation has changed, update the symbol.
      if (m_displayingTracks[trackId].m_affl != updatedTrackIndo.m_aff)
      {
        m_displayingTracks[trackId].m_affl = updatedTrackIndo.m_aff;
        symbol->hostility(decodeHostility(updatedTrackIndo.m_aff));
        isSymbolChanged = true;
      }

      //! update the track's symbol.
      if (isSymbolChanged)
      {
        m_displayingTracks[trackId].m_track->updateSymbol(0, symbol);
      }
    }

    //! update the track with the (updatedtracksDelivery) information.
    m_displayingTracks[trackId].m_track->trackName(trackId.c_str());
    m_displayingTracks[trackId].m_track->altitude(updatedTrackIndo.m_z);
    m_displayingTracks[trackId].m_track->velocity(updatedTrackIndo.m_s);

    //! update heading value
    double heading = atan2(updatedTrackIndo.m_dY, updatedTrackIndo.m_dX) * 180 / M_PI;
    m_displayingTracks[trackId].m_track->heading(heading);
    m_displayingTracks[trackId].m_track->headingIndicatorVisible(m_surfaceId, true);
    m_displayingTracks[trackId].m_track->headingIndicatorLength(25);

    //! move the track to its current lat/lon position
    m_displayingTracks[trackId].m_track->move(updatedTrackIndo.m_y, updatedTrackIndo.m_x);
  }

  //! update the selected metadata table if any is selected
  if (m_selectedTrackId != "")
  {
    //! get the track's information
    getSelectedTrackMetadata(updatedtracksDelivery, m_selectedTrackId, metadatPairs);

    //! add selected track's metadata
    metadatPairs.insert(metadatPairs.end(), m_selectedMetadatPairs.begin(), m_selectedMetadatPairs.end());
  }
}

//! redraw the drawing surface.
bool ClientManager::redrawSurface()
{
  return m_drawingSurface->redraw();
}

////////////////// Tracked Item Updated //////////////////

//! handles tracks updated slot sent by the thread
bool ClientManager::onTrackedItemUpdated(std::vector<std::pair<string, string>> &metadatPairs)
{
  m_clientConnectionThread->m_mutexTrackedItem.lock();
  m_clientConnectionThread->m_mutexTracks.lock();

  //! Process and update the track manager with the updated tracks.
  bool retProc = processUpdatedTrackedItem(m_clientConnectionThread->m_trackedItem, metadatPairs);

  //! redraw the drawing surface.
  bool retDraw = redrawSurface();

  m_clientConnectionThread->m_mutexTrackedItem.unlock();
  m_clientConnectionThread->m_mutexTracks.unlock();

  return retProc & retDraw;
}

//! Process and update the track manager with the updated tracks.
bool ClientManager::processUpdatedTrackedItem(const TrackedItem &updatedtrackedItem, std::vector<std::pair<string, string>> &metadatPairs)
{
  const string& trackId = updatedtrackedItem.m_trackid;
  //! If this track does not exist, return.
  if (m_displayingTracks.count(trackId) == 0)
  {
    return false;
  }

  m_selectedTrackId = trackId;

  //! get the track's information
  getSelectedTrackMetadata(updatedtrackedItem, metadatPairs);
  //! get the track's metadata and append it to the metadatPairs
  m_selectedMetadatPairs.clear();
  if (decodeMetadata(updatedtrackedItem, m_selectedMetadatPairs))
  {
    metadatPairs.insert(metadatPairs.end(), m_selectedMetadatPairs.begin(), m_selectedMetadatPairs.end());
  }

  //! show history points
  if (m_showHistorySquare)
  {
    m_trackManager->historyPointType(TSLTrackDisplayManager::HistoryPointTypeSquare);
  }
  else
  {
    m_trackManager->historyPointType(TSLTrackDisplayManager::HistoryPointTypeSymbol);
  }
  m_displayingTracks[trackId].m_track->historyPointsVisible(m_surfaceId, true);

  //! select clicked track.
  m_trackManager->selectTrack(m_surfaceId, m_displayingTracks[trackId].m_trackNumber, true);

  //! display the history of the track.
  if (!m_recordTracksHistory)
  {
    m_trackManager->clearAllHistoryPoints();
  }
  double currentLat = m_displayingTracks[trackId].m_track->latitude();
  double currentLon = m_displayingTracks[trackId].m_track->longitude();
  for (const auto& posInfo : updatedtrackedItem.m_positionValue)
  {
    if (posInfo.second.m_pos.m_x != 0 || posInfo.second.m_pos.m_y != 0)
    {
      m_displayingTracks[trackId].m_track->move(posInfo.second.m_pos.m_y, posInfo.second.m_pos.m_x);
    }
  }
  m_displayingTracks[trackId].m_track->move(currentLat, currentLon);

  //! clear previously selected track
  if (prev_selectedTrackId != m_selectedTrackId && prev_selectedTrackId != "")
  {
    if (m_displayingTracks.count(prev_selectedTrackId) > 0)
    {
      m_trackManager->selectTrack(m_surfaceId, m_displayingTracks[prev_selectedTrackId].m_trackNumber, false);
      m_displayingTracks[prev_selectedTrackId].m_track->historyPointsVisible(m_surfaceId, false);
    }
  }
  prev_selectedTrackId = m_selectedTrackId;

  return true;
}

//! handles clicking left click in the tracks mode.
bool ClientManager::handleTracksModeLeftClick(TSLDeviceUnits x, TSLDeviceUnits y)
{
  double lat = 0.0;
  double lon = 0.0;

  // Get the position pressed in lat long
  m_drawingSurface->DUToLatLong(x, y, &lat, &lon);

  m_clientConnectionThread->m_mutexTrackedItem.lock();
  //! query the track display manager to get the clicked track.
  double margin = queryTrackMargin;
  TSLTrack *clickedTrack = NULL;
  while (clickedTrack == NULL && margin < 1.0)
  {
    clickedTrack = m_trackManager->queryTrackFromLatLon(lat, lon, margin);
    margin *= 5;
  }
    
  if (clickedTrack == NULL)
  {
    m_clientConnectionThread->m_mutexTrackedItem.unlock();

    //! show tool tip with the error message [TODO]
    QToolTip::showText(QPoint(x, y + 32), "Failed to query Track From Lat Lon.\nZoom in and click in the middle of the track.");
    return false;
  }

  //! get subscription ID
  string   trackName = clickedTrack->trackName();

  //! Send getTrackedItemByID STOMP command to the server
  string errorMsg;
  if (!m_clientConnectionThread->send_getTrackedItemByID(
    m_clientConnectionThread->m_settings.m_publicationIdentifier,
    trackName,
    m_clientConnectionThread->m_settings.m_serverSubscriptionId,
    errorMsg)
    )
  {
    //! show tool tip with the error message [TODO]
    QToolTip::showText(QPoint(x, y + 32), errorMsg.c_str());

    m_clientConnectionThread->m_mutexTrackedItem.unlock();
    return false;
  }

  m_clientConnectionThread->m_mutexTrackedItem.unlock();
  return true;
}

////////////////// Errors Updated //////////////////

//! handles errors updated slot sent by the thread
void ClientManager::onErrorsUpdated()
{
  m_clientConnectionThread->m_mutexErrorsMsg.lock();
  string msg = m_clientConnectionThread->m_errorsMsg;
  m_clientConnectionThread->m_mutexErrorsMsg.unlock();

  //! show Message box
  QMessageBox::critical(m_parentWidget, "Errors Subscribed Channel", QString::fromUtf8(msg.c_str()));
}

//! update the server with the current view extent.
void ClientManager::updateViewExtent(TSLDrawingSurface* drawingSurface)
{
  //! get current extent
  TSLDeviceUnits leftX;
  TSLDeviceUnits topY;
  TSLDeviceUnits rightX;
  TSLDeviceUnits bottomY;
  drawingSurface->getDUExtent(&leftX, &topY, &rightX, &bottomY);

  //! Get the positions in lat long
  double lowerCornerX = 0, lowerCornerY = 0, upperCornerX = 0, upperCornerY = 0;
  bool ret1 = m_drawingSurface->DUToLatLong(leftX, bottomY, &lowerCornerY, &lowerCornerX, true);
  bool ret2 = m_drawingSurface->DUToLatLong(rightX, topY, &upperCornerY, &upperCornerX, true);
  if (!ret1 || !ret2)
  {
    //! clamp the lat long extents.
    if (lowerCornerY < -90)
      lowerCornerY = -90;
    if (upperCornerY > 90)
      upperCornerY = 90;
    if (lowerCornerX < -180)
      lowerCornerX = -180;
    if (upperCornerX > 180)
      upperCornerX = 180;
  }

#if 1
  //! Send updateViewExtent STOMP command to the server
  string errorMsg;
  if (m_clientConnectionThread)
  {
    if (!m_clientConnectionThread->send_UpdateViewExtent("EPSG:4326", lowerCornerX, lowerCornerY, upperCornerX, upperCornerY, errorMsg))
    {
      //! show tool tip with the error message [TODO]

      return;
    }
  }
#endif
}