/****************************************************************************
Copyright (c) 2008-2022 by Envitia Group PLC.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

****************************************************************************/

#include "trackinformation.h"
#include "displaytrack.h"
#include "tslsymbol.h"
#include "tslrenderingattributes.h"

TSLTrackPointSymbol* DisplayTrack::m_templateSelectionSymbol = nullptr;
TSLAPP6AHelper* DisplayTrack::m_militarySymbolHelper = nullptr;

TSLAPP6AHelper* DisplayTrack::getMilitarySymbolHelper()
{
  if (m_militarySymbolHelper == nullptr)
  {
    // Load app6a config. Alternative configs available for app6 (unfilled) and mil-std-2525
    std::string config(TSLUtilityFunctions::getMapLinkHome());
    config.append("/config/app6aConfig.csv"); // the default config file
    m_militarySymbolHelper = new TSLAPP6AHelper(config.c_str());
    if (!m_militarySymbolHelper->valid())
    {
      throw std::exception("invalid app6 symbol config");
    }
  }

  return m_militarySymbolHelper;
}

DisplayTrack::DisplayTrack(TSLTrackDisplayManager*  trackManager)
  : m_track(nullptr)
{
  m_trackManager = trackManager;

  //! create selection symbol (symbol around the track when it is selected).
  createDefaultSelectionSymbol();
}

void DisplayTrack::removeDisplayTrack()
{
  //! remove track from track manager
  if (m_trackManager)
  {
    m_trackManager->removeTrack(m_trackInfo.id);
  }
}

void DisplayTrack::addEntityToPointSymbol(TSLTrackPointSymbol * symbol, int symbolIDVal, int colour, uint32_t size)
{
  // Hard coded app6a symbol. (i.e. completely ignore the application's config for now!)
  // See dev guide section 12.7 for explanation.

  const char fighterId[] = "1.x.2.1.1.2";
  TSLAPP6ASymbol theSymbol;
  if (!getMilitarySymbolHelper()->getSymbolFromID(fighterId, theSymbol))
  {
    return;
  }
    
  theSymbol.hostility(TSLAPP6ASymbol::HostilityHostile);
  theSymbol.designation("ABC123");
  theSymbol.heightType(TSLDimensionUnitsPixels);
  theSymbol.height(100);
  theSymbol.x(0); // TMCs
  theSymbol.y(0);

  TSLEntitySet* es = getMilitarySymbolHelper()->getSymbolAsEntitySet(&theSymbol);
  symbol->addSymbolEntity(es, true);
}

void DisplayTrack::createDefaultSelectionSymbol()
{
  if (!m_templateSelectionSymbol)
  {
    m_templateSelectionSymbol = (TSLTrackPointSymbol::create());
    
    TSLSymbol* symbol = TSLSymbol::create(0, 0, 0);
    symbol->setRendering(TSLRenderingAttributeSymbolStyle, 3);
    symbol->setRendering(TSLRenderingAttributeSymbolColour, 0x0000ffff);
    symbol->setRendering(TSLRenderingAttributeSymbolSizeFactor, 70);
    symbol->setRendering(TSLRenderingAttributeSymbolSizeFactorUnits, TSLDimensionUnitsPixels);
    m_templateSelectionSymbol->addSymbolEntity(symbol, false);
    symbol->destroy();
  }
}

TSLTrackSymbol* DisplayTrack::createCustomTrackSymbol(uint32_t symbolId, uint32_t color, uint32_t size)
{
  //! create track symbol (point symbol)
  TSLTrackPointSymbol* pointSymbol(TSLTrackPointSymbol::create());
  addEntityToPointSymbol(pointSymbol, symbolId, color, size);

  //! set the selection symbol to the track symbol.
  pointSymbol->selectionSymbol(m_templateSelectionSymbol, TSLTrackSymbol::SelectionBehaviourAdditional);

  return pointSymbol;
}

//! clone symbol template, create a display track, and add the track to the track manager.
bool DisplayTrack::updateDisplayTrack(const TrackInformation& trackInfo)
{
  m_trackInfo = trackInfo;

  TSLTrackSymbol* symbolTemplate = nullptr;
  symbolTemplate = createCustomTrackSymbol(m_trackInfo.symbolId, m_trackInfo.colour, m_trackInfo.size);

  //!
  //! if the track is not created, create it.
  //! If the track is already created, update the symbol.
  //!
  bool result = false;
  if (!m_track)
  {
    //! create track using the cloned symbol template.
    m_track = TSLTrack::create(symbolTemplate);

    //! add track to track manager.
    result = (m_trackManager) ? m_trackManager->addTrack(m_trackInfo.id, m_track) : false;
  }
  else
  {
    //! update the symbol of the track.
    result = m_track->updateSymbol(0, symbolTemplate);
  }

  return result;
}

//! move the track
void DisplayTrack::moveTrack(double latitude, double longitude)
{
  if (m_track)
  {
    m_track->move(latitude, longitude);
  }
}

//! check if track information has changed
bool DisplayTrack::isInformationChanged(const TrackInformation& trackInfo)
{
  return (m_trackInfo != trackInfo);
}