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
