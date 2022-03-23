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