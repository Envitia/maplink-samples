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

#ifndef MAPLINKWIDGET_H
#define MAPLINKWIDGET_H

#include <QMainWindow>
#include <QEvent>
#include <QWidget>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPaintEngine>

#include "application.h"
#include "trackssimulator.h"

//! Qt and X11 use Bool.
#ifdef Bool
#undef Bool
#endif

class MapLinkWidget : public QWidget
{
  Q_OBJECT
public:
  MapLinkWidget(QWidget *parent = 0);//, Qt::WFlags flags = 0);
  virtual ~MapLinkWidget();

  //! Loads a map/file
  void loadMap(const char *filename);

  //! parse configuration file to read the hostility and type mapping.
  bool parseConfigurationFile(const QString &configFilePath, QString &msgError);

  //! get configuration settings.
  const ConfigurationSettings& getConfigurationSettings();

  virtual bool close();

  //! Event handlers invoked by the main window
  void resetView();
  void zoomInOnce();
  void zoomOutOnce();
  void activatePanMode();
  void activateGrabMode();
  void activateZoomMode();

  //! set the call back to update the GUI for reseting interaction modes.
  void ResetInteractionModesCallBack(resetInteractionModesCallBack func);

  //! handles start thread button click
  void activateStartTracks();
  //! handles stop thread button click
  void activateStopTracks();

  //! handles changing the tracks symbols [config xml/ default]
  bool activateUse_symbolSetsChanged(const QString& symbolSet, QString& msgError);

public slots:
  //! handles tracks updated slot sent by the thread
  void onTracksUpdated();

signals:
  void mapDrawn();

protected:
  //! resize - informs the drawing surface of any change in size of the window
  //! Relys on the ResizeAction to maintain the view of the map sensibly
  virtual void resizeEvent(QResizeEvent *);

  //! paint the window
  virtual void paintEvent(QPaintEvent *);

  //! Keyboard and Mouse events.
  virtual void mouseMoveEvent(QMouseEvent *event);
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void wheelEvent(QWheelEvent *event);
  virtual void keyPressEvent(QKeyEvent *event);
  virtual void keyReleaseEvent(QKeyEvent *event);

  //! Important for Qt4 to stop Qt drawing to this widget.
  virtual QPaintEngine *paintEngine() const
  {
    return 0;
  }

private:
  //! Creates the MapLink drawing surface and associated map data layer
  void create();

  //! Application instance - this contains all the MapLink related code.
  Application *m_application;
  
  //! flag to be set when the map is initialized
  bool m_initialized;

  //! Tracks simulator thread
  TracksSimulator *tracksSimulatorThread;
  
};


#endif //! MAPLINKSAMPLE2_H
