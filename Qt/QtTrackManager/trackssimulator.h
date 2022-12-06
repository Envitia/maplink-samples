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

#ifndef TRACKSSIMULATORTHREAD_H
#define TRACKSSIMULATORTHREAD_H

#include <QThread>
#include <qmutex.h>
#include "trackinformation.h"
#include "configurationsettings.h"

//!
//! Tracks simulator which parses the xml configuration file to read the tracks information, then
//! create and move the tracks using a timer.
//!
class TracksSimulator : public QThread
{
  Q_OBJECT
public:
  //! constructor to initialize the tracks positions.
  explicit TracksSimulator(QObject *parent = 0);

  //! run the thread.
  void run();

public:// Tracks positions

  //! Update Tracks Positions.
  void updateTracksPositions();

  //! parse configuration file to read the hostility and type mapping.
  bool parseConfigurationFile(const QString &configFilePath, QString &msgError);

  //! get configuration settings.
  const ConfigurationSettings& getConfigurationSettings();

  //! change Tracks symbols
  bool changeTracksSymbol(QString newSymbolSet, QString &msgError);

signals:
  //! Signal to be sent by the thread when tracks are updated.
  void tracksUpdated();

public:
  //! map of tracks information
  std::map<int, TrackInformation> m_tracksInformation;

  //! mutex to protect the map of tracks.
  QMutex m_mutexTracks;

  //! flag to be sent to quit the thread.
  bool m_abort;

  //! mutex to protect the abort flag.
  QMutex m_mutexAbort;


private:
  //! configurations file object.
  ConfigurationSettings m_config;
};

#endif // TRACKSSIMULATORTHREAD_H
