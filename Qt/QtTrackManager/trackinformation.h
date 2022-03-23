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
