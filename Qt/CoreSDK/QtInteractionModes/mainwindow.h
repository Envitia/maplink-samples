/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ui_qtinteractionmodes.h"

class MainWindow : public QMainWindow, private Ui_QtInteractionModesClass
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void loadMap(const char *filename);

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

private:
  QActionGroup *m_interactionModesGroup;
};

#endif
