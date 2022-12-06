/****************************************************************************
Copyright (c) 2008-2022 by Envitia Group PLC.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free 
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more 
details.

You should have received a copy of the GNU Lesser General Public License 
along with this program. If not, see <https://www.gnu.org/licenses/>.

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