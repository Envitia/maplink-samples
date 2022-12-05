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

#ifndef TRACKINFORMATION_H
#define TRACKINFORMATION_H

#include <qstring.h>

//! Struct which contains the track information to be used for each track creation.
struct TrackInformation
{
  //! track id
  uint32_t id;

  //! track latitude
  double lat;

  //! track latitude moving offset
  double lat_move_offset;

  //! track longitude
  double lon;

  //! track longitude moving offset
  double lon_move_offset;

  //! track type
  QString type;

  //! track hostility
  QString hostility;

  //! track symbol id
  uint32_t symbolId;

  //! track colour
  uint32_t colour;
  
  //! track size
  uint32_t size;

  //! check if the basic track information has changed.
  bool operator!=(const TrackInformation& other)
  {
    return (
      id != other.id ||
      symbolId != other.symbolId ||
      colour != other.colour ||
      size != other.size
      );
  }
};

#endif // TRACKINFORMATION_H
