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

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ui_qttrackmanager.h"

class MainWindow : public QMainWindow, private Ui_QtTrackManagerClass
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void loadMap(const char *filename);

  //! Add menu items for the symbol sets.
  void updateSymbolSetsMenu(const ConfigurationSettings& configSettings);
  QVector<QAction*> symbolSetActions;

  //! parse configuration file to read the hostility and type mapping.
  bool parseConfigurationFile(const QString &configFilePath, QString &msgError);

  //! set the call back to update the GUI for reseting interaction modes.
  void resetInteractionModes();

  private slots:
  void loadMap();
  void resetView();
  void zoomInOnce();
  void zoomOutOnce();
  void activatePanMode();
  void activateGrabMode();
  void activateZoomMode();
  void showAboutBox();
  void exit();

  //! Tracks
  //! handles start thread button click
  void activateStartTracks();

  //! handles stop thread button click
  void activateStopTracks();

  //! handles changing the tracks symbols [config xml/ default]
  void activateUse_symbolSetsChanged(QAction *);
private:
  QActionGroup *m_interactionModesGroup;
};

#endif
