/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ui_qteventmanager.h"
#include <qlabel.h>

class MainWindow : public QMainWindow, private Ui_QtEventManagerClass
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
  void activateTracksMode();
  void showAboutBox();
  void exit();

private:
  QActionGroup *m_interactionModesGroup;

  //! Tracks
private:
  //! Client Connection thread
  ClientConnectionThread *m_clientConnectionThread;

  public slots:
  //! handles start client thread button click
  void activateStartClient();

  //! handles stop client thread button click
  void activateStopClient();

public:
  //! handles exit client thread
  bool activateExitClient();

  //! show metadata table widget
  void showMetadataTableWidget(std::vector<std::pair<string, string>> &metadatPairs);

  public slots:
  //! handles tracks updated slot sent by the thread
  void onTracksUpdated();

  //! handles tracks updated slot sent by the thread
  void onTrackedItemUpdated();

  //! handles errors updated slot sent by the thread
  void onErrorsUpdated();

  //! tracks history
public:
  //! flag to record tracks history
  bool m_recordTracksHistory;
  //! maximum tracks history time to record.
  int m_recordMaximum;

  //! cuurent recorded maximum time when recording history.
  int m_current_RecordMaximum;

  //! tracks history slider
  QSlider *m_tracksHistorySlider;
  QLabel  *m_tracksHistoryTitle;
  QLabel  *m_tracksHistoryProgress;

  QAction* m_tracksHistoryTitleAction;
  QAction* m_tracksHistorySliderAction;
  QAction* m_tracksHistoryProgressAction;

  //! Show/hide tracks history slider and labels.
  void showHistorySlider(bool visible);

  public slots:
  //! handles changing the tracks history slider.
  void historySliderValueChanged(int value);
};

#endif
