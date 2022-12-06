/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "maplinkwidget.h"
#include "ui_simple3dinteraction.h"
#include <QMainWindow>

class MainWindow : public QMainWindow, private Ui::Simple3DInteraction
{
  Q_OBJECT

public:
  MainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0);
  virtual ~MainWindow();

  // These funcions are invoked in response to user interface events
private slots:
  void openMap();
  void openTerrain();
  void zoomIn();
  void zoomOut();
  void resetView();
  void activateEyePointInteractionMode();
  void activateWorldInteractionMode();
  void toggleWireframe();
  void toggleTerrainExaggeration();
  void toggleCameraAltitudeLimit();
};

#endif

