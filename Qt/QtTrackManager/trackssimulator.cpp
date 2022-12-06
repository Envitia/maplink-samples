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

#include "trackssimulator.h"

//! constructor to initialize the tracks positions.
TracksSimulator::TracksSimulator(QObject *parent)
  : QThread(parent)
  , m_abort(false)
{
}

//! run the thread.
void TracksSimulator::run()
{
  m_abort = false;

  while (true)
  {
    //! Check if thread stop is triggered
    m_mutexAbort.lock();
    if (this->m_abort)
    {
      m_mutexAbort.unlock();
      return;
    }
    m_mutexAbort.unlock();

    //! update tracks
    m_mutexTracks.lock();
    updateTracksPositions();
    m_mutexTracks.unlock();

    //! send tracks updated signal
    emit tracksUpdated();

    //! sleep
    this->msleep(50);
  }
}


bool TracksSimulator::parseConfigurationFile(const QString &configFilePath, QString &msgError)
{
  //! parse the configuration file
  return m_config.parseConfigFile(configFilePath, m_tracksInformation, msgError);
}

//! get configuration settings.
const ConfigurationSettings& TracksSimulator::getConfigurationSettings()
{
  return m_config;
}

//! Update Tracks Positions.
void TracksSimulator::updateTracksPositions()
{
  //! move each track by its defined lat/lon offset.
  for (auto it = m_tracksInformation.begin(); it != m_tracksInformation.end(); ++it)
  {
    int i = it->first;
    double lon_shift = m_tracksInformation[i].lon_move_offset;// getRandom(10);
    double lat_shift = m_tracksInformation[i].lat_move_offset;// getRandom(10);

    m_tracksInformation[i].lon += lon_shift;
    m_tracksInformation[i].lat += lat_shift;

    // Check to see if the track has fallen off the bottom of the Earth
    if (m_tracksInformation[i].lat < -90.0)
    {
      // Direct the track to start moving back up north
      m_tracksInformation[i].lat = -90.0;
      m_tracksInformation[i].lat_move_offset = -m_tracksInformation[i].lat_move_offset;
    }
    // Check to see if the track has fallen off the top of the Earth
    else if (m_tracksInformation[i].lat > 90.0)
    {
      // Direct the track to start moving back down south
      m_tracksInformation[i].lat = 90.0;
      m_tracksInformation[i].lat_move_offset = -m_tracksInformation[i].lat_move_offset;
    }

    // Longitude bounds - this is OK to wrap round the Earth
    if (m_tracksInformation[i].lon < -180.0)
      m_tracksInformation[i].lon = 180.0;
    else if (m_tracksInformation[i].lon > 180.0)
      m_tracksInformation[i].lon = -180.0;
  }
}

//! change Tracks symbols
bool TracksSimulator::changeTracksSymbol(QString newSymbolSet, QString &msgError)
{
  return m_config.updateTracks(m_tracksInformation, newSymbolSet, msgError);
}