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
