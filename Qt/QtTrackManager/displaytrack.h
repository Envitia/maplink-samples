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

#ifndef DISPLAYTRACKS_H
#define DISPLAYTRACKS_H

#include "MapLink.h"
#include "tsltrackdisplaymanager.h"


struct TrackInformation;
//!
//! Display track class contains the track object and symbol and track creation methods.
//!
class DisplayTrack
{
public:
  DisplayTrack(TSLTrackDisplayManager*  trackManager);

  //! clone symbol template, create a display track or update existing track, and add the track to the track manager.
  bool updateDisplayTrack(const TrackInformation& trackInfo);

  //! move the track
  void moveTrack(double latitude, double longitude);

  //! check if track information has changed
  bool isInformationChanged(const TrackInformation& trackInfo);

  //! remove the track from the track manager
  void removeDisplayTrack();

private:
  //! create selection symbol (symbol around the track when it is selected).
  void createDefaultSelectionSymbol();

  //! create a track based on its symbol id and colour.
  TSLTrackSymbol* createCustomTrackSymbol(uint32_t symbolId, uint32_t color, uint32_t size);

  //! Add entity to the point symbol
  static void addEntityToPointSymbol(TSLTrackPointSymbol * symbol, int symbolIDVal, int colour, uint32_t size);

  //! display track
  TSLTrack* m_track;

  //! track information
  TrackInformation m_trackInfo;

  //! reference to the track manager
  TSLTrackDisplayManager*  m_trackManager;

  //! default selection symbol template to be re-used for all symbols.
  static TSLTrackPointSymbol* m_templateSelectionSymbol;
};

#endif // DISPLAYTRACKS_H