/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// This class is the main window of the application.

#include <QMainWindow>
#include <QActionGroup>
#include <QLabel>
#include "ui_mainwindow.h"
#include "decluttermodel.h"

class QSpinBox;
class TrackHostilityDelegate;

class MainWindow : public QMainWindow, private Ui_MainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void loadMap( const char *mapToLoad );

private slots:
  void loadMap();
  void toggleTrackMotion();
  void toggleFollowTrack( bool followTrack );
  void toggleViewOrientation( bool trackHeadingUp );
  void setNoTrackAnnotation();
  void setLowTrackAnnotation();
  void setMediumTrackAnnotation();
  void setHighTrackAnnotation();
  void pinSelectedTrack();
  void changeTrackNumbers();
  void setViewScale500();
  void setViewScale1000();
  void setViewScale10000();
  void setViewScale50000();
  void setViewScale250000();
  void toggleTiledBuffering( bool enable );
  void setSymbolTypeAPP6A();
  void setSymbolType2525B();
  void showAboutBox();

  // Called by the track update thread when a track is selected/deselected. Used to update the status
  // of various UI controls and to trigger a display refresh if necessary.
  void trackSelectionStatusChanged( bool trackSelected );

private:
  // Enables/disables UI controls that allow modification of the tracks in the simulation.
  // These are disabled when playback is enabled
  void toggleTrackControls( bool enable );

  // Enables/disables UI controls that only make sense when a map is loaded
  void toggleMapLoadedControls( bool enable );

  // Changes the current view scale to be the specified scale
  void setViewScale( double scaleFactor );

  QActionGroup *m_interactionModesGroup;
  QActionGroup *m_symbolAnnotationGroup;
  QActionGroup *m_symbolTypeGroup;

  // Used to cause the application to update the display continuously without user interaction
  QTimer *m_appRefresh;

  QWidget *m_toolbarSpeedControl;

  // Qt delegate used for editing track hostilities in the UI
  TrackHostilityDelegate *m_hostilityDelegate;
};

#endif // MAINWINDOW_H
