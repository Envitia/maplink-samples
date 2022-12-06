/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QMessageBox>
#include "tracklayer.h"
#include "tracks/trackmanager.h"
#include "tracks/track.h"
#include "MapLinkDrawing.h"
#include "MapLinkOpenGLSurface.h"
#include "MapLinkOpenGLTrackHelper.h"
#include <cmath>
#include <cassert>
#include <iostream>

using namespace std;


#define TRACK_NOT_SELECTED 65534


TrackLayer::TrackLayer()
  : m_bufferTrackLimit(0)
  // Feature IDs used for decluttering of tracks by hostility type
  , m_friendFeatureID(1)
  , m_hostileFeatureID(2)
  , m_neutralFeatureID(3)
  , m_unknownFeatureID(4)
  , m_suspectFeatureID(5)
  , m_assumedFriendFeatureID(6)
  , m_pendingFeatureID(7)
  , m_jokerFeatureID(8)
  , m_fakerFeatureID(9)
  , m_headingIndicatorFeatureID(10)
  , m_historyPointsFeatureID(11)
  , m_lastAnnotationLevel(AnnotationNone)
  , m_trackHelper(0)
  , m_lastSelectedTrack(TRACK_NOT_SELECTED)
{
}

TrackLayer::~TrackLayer()
{
  if (m_trackHelper)
  {
    delete m_trackHelper;
    m_trackHelper = NULL;
  }
}

bool TrackLayer::initialise(TSLOpenGLSurface *surface)
{
  if (!m_trackHelper)
  {
    m_trackHelper = new TSLOpenGLTrackHelper();

    // Need to initialise the Track helper class
    if (m_trackHelper)
    {
      // It is legitimate to remove the constness from the drawing surface for the purposes of accessing the state tracker.
      TSLOpenGLSurface *nonConstGLSurface = const_cast<TSLOpenGLSurface*>(surface);
      if (!nonConstGLSurface)
      {
        QMessageBox::critical(NULL, "Track Helper Initialisation Failed", "The TSLOpenGLSurface is null");
        return false;
      }

      // Initialise with positionsInTMCs set to false as we will all our Track Positions will be in Lat/Long
      // The Track Helper will then convert the positions to TMCs
      if (!m_trackHelper->initialise(nonConstGLSurface, false))
      {
        QMessageBox::critical(NULL, "Track Helper Initialisation Failed", "Check the Error Stack for further information.");
        return false;
      }

      const unsigned int maxHistoryPoints = (unsigned int)Track::maxHistoryPoints();
      if (!m_trackHelper->maximumNumberOfTrackHistoryPoints(maxHistoryPoints))
      {
        QMessageBox::information(NULL, "Track Helper failed to set Maximum Number Of Track History points", "Check the Error Stack for further inforamtion.");
        return false;
      }
    }
    else
    {
      QMessageBox::critical(NULL, "Track Helper creation Failed", "Check the Error Stack for further inforamtion.");
      return false;
    }
  }

  // Create some features that we can use to declutter tracks by hostility type.
  TSLDataLayer *customLayer = dataLayer();
  customLayer->addFeatureRendering("Friend", m_friendFeatureID);
  customLayer->addFeatureRendering("Hostile", m_hostileFeatureID);
  customLayer->addFeatureRendering("Neutral", m_neutralFeatureID);
  customLayer->addFeatureRendering("Unknown", m_unknownFeatureID);
  customLayer->addFeatureRendering("Suspect", m_suspectFeatureID);
  customLayer->addFeatureRendering("Assumed Friend", m_assumedFriendFeatureID);
  customLayer->addFeatureRendering("Pending", m_pendingFeatureID);
  customLayer->addFeatureRendering("Joker", m_jokerFeatureID);
  customLayer->addFeatureRendering("Faker", m_fakerFeatureID);
  customLayer->addFeatureRendering("Heading Indicators", m_headingIndicatorFeatureID);
  customLayer->addFeatureRendering("History Points", m_historyPointsFeatureID);

  return true;
}

bool TrackLayer::drawLayer(TSLRenderingInterface *renderingInterface, const TSLEnvelope* extent, TSLCustomDataLayerHandler& layerHandler)
{
  const TrackManager::DisplayInfo *displayInfo = TrackManager::instance().displayInformation();
  if (!displayInfo || displayInfo->m_tracks.empty())
  {
    // No tracks to display
    return false;
  }

  if (!m_trackHelper)
  {
    // Invalid Track Helper
    QMessageBox::critical(NULL, "Track Helper object invalid", "Check the Error Stack for further information.");
    return false;
  }

  // Number of Tracks has changed ?
  if ((displayInfo->m_tracks.size() != m_bufferTrackLimit) || (m_lastAnnotationLevel != displayInfo->m_annotationLevel))
  {
    // As we put some of the APP6A/2525B annotations into the texture atlas along with the symbol, changing
    // the amount of annotations displayed requires us to regenerate the contents of the texture atlas.

    // Reset last selected track
    m_lastSelectedTrack = TRACK_NOT_SELECTED;

    // Remove all existing symbols in the TrackHelper
    if (!m_trackHelper->removeAllTracks())
    {
      // Error removing all symbols from Track Helper
      QMessageBox::critical(NULL, "Error removing all symbols from Track Helper", "Check the Error Stack for further information.");
      return false;
    }
    m_instanceIDs.clear();

    m_bufferTrackLimit = displayInfo->m_tracks.size();
    m_instanceIDs.resize(m_bufferTrackLimit);
    for (size_t i = 0; i < m_bufferTrackLimit; ++i)
    {
      const Track::DisplayInfo& currentTrack = displayInfo->m_tracks[i];

      // In order to rasterise the symbol, we first need to get it in a form that can be drawn using a drawing surface.
      // In our case, this is the TSLEntitySet representation. We can draw this using a TSLStandardDataLayer in conjunction
      // with the MapLink drawing surface and a framebuffer object to render directly to our texture atlas.
      TSLAPP6ASymbol symbol;
      TrackManager::instance().symbolHelper()->getSymbol(currentTrack.m_symbolKey, symbol);
      symbol.hostility(currentTrack.m_hostility);
      symbol.height(currentTrack.m_size);
      symbol.heightType(TSLDimensionUnitsPixels);

      // Add any required fixed annotations to the symbol. We will bake these into the texture atlas as they
      // don't change over time.
      // For static labels we don't have specific strings to use, so just fill in a basic identifier to show where
      // the label is in relation to the symbol.
      switch (displayInfo->m_annotationLevel)
      {
      case AnnotationHigh:
        symbol.additionalInformation("AdditionalInfo");
        symbol.C2HQName("C2HQName");
        symbol.designation("Designation");
        // Fall through

      case AnnotationMedium:
        symbol.staffComments("Comments");
        symbol.higherFormation("Formation");
        symbol.quantity("Quality"); // Do as static text element
        symbol.combatEffectiveness("Effectiveness");
        // Fall through

      case AnnotationLow:
        // Position and speed annotations are enabled, but as they vary on a per-track basis they aren't
        // stored in the texture atlas and are instead rendered seperately.
        symbol.unitSize(TSLAPP6ASymbol::UnitSizeArmy);
        break;

      case AnnotationNone:
      default:
        break;
      }
      symbol.textColour(TSLComposeRGB(255, 255, 255));

      // Need to get as Entity Set and pass to Track Helper to rasterise/add to Texture Atlas 
      TSLEntitySet* es = TrackManager::instance().symbolHelper()->getSymbolAsEntitySet(&symbol);
      if (!es)
      {
        // Error Invalid Entity Set
        QMessageBox::critical(NULL, "Error retrieving Track Symbol", "Check the Error Stack for further information.");
        return false;
      }

      // Apply a black halo to the annotation text so it is more visible.
      applyHaloTextStyle(es, TSLComposeRGB(0, 0, 0));

      unsigned int trackID = 0;
      if (!m_trackHelper->addTrackReference(*es, trackID))
      {
        // Error adding track reference
        QMessageBox::critical(NULL, "Error adding Track reference to Track Helper", "Check the Error Stack for further information.");
        return false;
      }

      // Now need to add Track instance
      unsigned int instanceID = 0;
      if (!m_trackHelper->addTrackInstance(trackID, currentTrack.m_lat, currentTrack.m_lon, instanceID))
      {
        // Error adding Track Instance
        QMessageBox::critical(NULL, "Error adding Track instance to Track Helper", "Check the Error Stack for further information.");
        return false;
      }

      unsigned int hostilityColour;
      switch (currentTrack.m_hostility)
      {
      case TSLAPP6ASymbol::HostilityPending:
      case TSLAPP6ASymbol::HostilityUnknown:
        hostilityColour = 0xFF00FFFF;
        break;

      case TSLAPP6ASymbol::HostilityAssumedFriend:
      case TSLAPP6ASymbol::HostilityFriend:
        hostilityColour = 0xFFFFFF00;
        break;

      case TSLAPP6ASymbol::HostilityNeutral:
        hostilityColour = 0xFF00FF00;
        break;

      case TSLAPP6ASymbol::HostilitySuspect:
      case TSLAPP6ASymbol::HostilityHostile:
      case TSLAPP6ASymbol::HostilityJoker:
      case TSLAPP6ASymbol::HostilityFaker:
        hostilityColour = 0xFF0000FF;
        break;

      case TSLAPP6ASymbol::HostilityNone:
      default:
        hostilityColour = 0xFF000000;
        break;
      }

      // Add Track instance Heading
      unsigned int hdgLineLengthPixels = 50;
      if (!m_trackHelper->addTrackInstanceHeading(instanceID, hostilityColour, hdgLineLengthPixels, currentTrack.m_displayHeading))
      {
        // Error adding Track Heading
        QMessageBox::critical(NULL, "Error adding Track Heading to Track Helper", "Check the Error Stack for further information.");
        return false;
      }

      // History Points
      unsigned int histPointDistanceMetres = 20000;
      if (!m_trackHelper->addTrackInstanceHistoryPoints(instanceID, hostilityColour, histPointDistanceMetres))
      {
        // Error adding Track HistoryPoints
        QMessageBox::critical(NULL, "Error adding Track History Points to Track Helper", "Check the Error Stack for further information.");
        return false;
      }

      // Update Track Instance ID
      m_instanceIDs[i] = instanceID;

      // Destroy EntitySet object
      es->destroy();
    }
  }

  // Update each Track
  for (size_t i = 0; i < m_bufferTrackLimit; ++i)
  {
    const Track::DisplayInfo& currentTrack = displayInfo->m_tracks[i];

    // See if this track is decluttered
    TSLFeatureID declutterID = 0;
    switch (currentTrack.m_hostility)
    {
    case TSLAPP6ASymbol::HostilityUnknown:
      declutterID = m_unknownFeatureID;
      break;

    case TSLAPP6ASymbol::HostilityAssumedFriend:
      declutterID = m_assumedFriendFeatureID;
      break;

    case TSLAPP6ASymbol::HostilityFriend:
      declutterID = m_friendFeatureID;
      break;

    case TSLAPP6ASymbol::HostilityNeutral:
      declutterID = m_neutralFeatureID;
      break;

    case TSLAPP6ASymbol::HostilitySuspect:
      declutterID = m_suspectFeatureID;
      break;

    case TSLAPP6ASymbol::HostilityHostile:
      declutterID = m_hostileFeatureID;
      break;

    case TSLAPP6ASymbol::HostilityPending:
      declutterID = m_pendingFeatureID;
      break;

    case TSLAPP6ASymbol::HostilityJoker:
      declutterID = m_jokerFeatureID;
      break;

    case TSLAPP6ASymbol::HostilityFaker:
      declutterID = m_fakerFeatureID;
      break;

    default:
      assert(false);
      break;
    }

    if (renderingInterface->isDecluttered(NULL, declutterID))
    {
      // Tracks of this hostility are decluttered, don't add it to the list to draw
    }

    unsigned int instanceID = m_instanceIDs[i];
    if (!m_trackHelper->updateTrackInstance(instanceID, currentTrack.m_lat, currentTrack.m_lon))
    {
      // Error updating Symbol Instance
      QMessageBox::critical(NULL, "Error updating Track in Track Helper", "Check the Error Stack for further information.");
      return false;
    }

    if (!m_trackHelper->updateTrackInstanceHeading(instanceID, currentTrack.m_displayHeading))
    {
      // Error updating Track Heading
      QMessageBox::critical(NULL, "Error updating Track Heading in Track Helper", "Check the Error Stack for further information.");
      return false;
    }

    // Speed Label
    if (currentTrack.m_speedLabel)
    {
      if (!m_trackHelper->addTrackInstanceSingleDrawLabel(instanceID, currentTrack.m_speedLabel))
      {
        // Error adding Track Speed Label
        QMessageBox::critical(NULL, "Error adding single draw Track Speed Label to Track Helper", "Check the Error Stack for further information.");
        return false;
      }
    }

    // Position Label
    if (currentTrack.m_positionLabel)
    {
      if (!m_trackHelper->addTrackInstanceSingleDrawLabel(instanceID, currentTrack.m_positionLabel))
      {
        // Error adding Track Position Label
        QMessageBox::critical(NULL, "Error adding single draw Track Position Label to Track Helper", "Check the Error Stack for further information.");
        return false;
      }
    }

    // Is Track Selected ?
    if (displayInfo->m_selectedTrack == i && displayInfo->m_selectedTrack != m_lastSelectedTrack)
    {
      // First remove any previously selected Track
      if (m_lastSelectedTrack != TRACK_NOT_SELECTED)
      {
        if (!m_trackHelper->deselectTrackInstance((unsigned int)m_lastSelectedTrack))
        {
          // Error deselecting selected track
          QMessageBox::critical(NULL, "Error deselecting currently selected Track in Track Helper", "Check the Error Stack for further information.");
          return false;
        }
      }

      unsigned int col = 0xFF0F0F0F;
      if (!m_trackHelper->selectTrackInstance(instanceID, col))
      {
        // Error selecting Track Instance
        QMessageBox::critical(NULL, "Error selecting Track in Track Helper", "Check the Error Stack for further information.");
        return false;
      }
      m_lastSelectedTrack = instanceID;
    }
  }

  // Update Annotation level
  m_lastAnnotationLevel = displayInfo->m_annotationLevel;
  return m_trackHelper->draw(renderingInterface, extent);
}

void TrackLayer::applyHaloTextStyle(TSLEntitySet *set, TSLStyleID colour)
{
  if (!set)
  {
    return;
  }

  int numEntities = set->size();
  for (int i = 0; i < numEntities; ++i)
  {
    TSLEntity *entity = (*set)[i];
    if (entity->type() == TSLGeometryTypeEntitySet)
    {
      applyHaloTextStyle(reinterpret_cast<TSLEntitySet*>(entity), colour);
    }
    else if (entity->type() == TSLGeometryTypeText)
    {
      entity->setRendering(TSLRenderingAttributeTextBackgroundMode, TSLTextBackgroundModeHalo);
      entity->setRendering(TSLRenderingAttributeTextBackgroundColour, colour);
    }
  }
}

