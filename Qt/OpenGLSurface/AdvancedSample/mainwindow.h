/****************************************************************************
                  Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_advancedsample.h"

class QMainWindow;
class QActionGroup;
class QLabel;
class QProgressDialog;
class QStatusBar;
class QTimer;
class QSpinBox;
class TreeModel;
class TreeItem;
class TrackHostilityDelegate;
class DirectImportWizard;
class CacheSizeDialog;
class URLDialog;

class MainWindow : public QMainWindow, private Ui_MainWindow
{
  Q_OBJECT
public:
  MainWindow( QWidget *parent = 0 );
  ~MainWindow();
  bool loadMap( const char *mapToLoad ) const;
  void openDirectImport( const std::string& fileName );

signals:
  void attributeDatalayer( const std::string& layerName, const std::string& layerType );

private slots:
  void exit();

  void loadLayerDialog();
  void loadURLDialog();
  void resetView() const;
  void zoomInOnce() const;
  void zoomOutOnce() const;
  void activatePanMode() const;
  void activateGrabMode() const;
  void activateZoomMode() const;
  void activateWaypointMode() const;
  void activateTrackSelectMode() const;
  void toggleLayerDock() const;
  void toggleAttributeDock() const;
  void toggleTrackDock() const;
  void setLayerMenuChecked() const;
  void setAttributeMenuChecked() const;
  void setTracksMenuChecked() const;
  void showAboutBox();
  void moveLayerToIndex( const char*, const char*, int ) const;
  void editTreeModel( const QModelIndex&, long ) const;
  void removeTreeLayer( const std::string& layerName, const std::string& treeAttribute ) const;
  void cacheSizeDialog( const std::string& name );
  void selectedAttributeData( const std::string& layerName, const std::string& layerType );

  void zoomScale( int );
  void nauticalRange( int );
#ifdef HAVE_DIRECT_IMPORT_SDK
  void moveDirectImportDataSetToIndex( const char* layerName, const char* scaleBandName, int rowFrom, int rowTo );
  void removeDirectImportDataSet( const char* layerName, const char* scaleBandName, int row );
  void editDirectImportDataSet( const char* layerName, const char* scaleBandName, int row );
  void finishedDirectImportWizard();
  void cancelDirectImportWizard();
#endif
  void cacheSizeDialogOKButton();
  void urlDialogOKButton();

  void toggleTrackMotion();
  void toggleFollowTrack( bool followTrack );
  void toggleViewOrientation( bool trackHeadingUp );
  void setNoTrackAnnotation();
  void setLowTrackAnnotation();
  void setMediumTrackAnnotation();
  void setHighTrackAnnotation();
  void pinSelectedTrack();
  void changeTrackNumbers();
  void setSymbolTypeAPP6A();
  void setSymbolType2525B();

  void setMainToolbar();
  void setProjectionToolbar();
  void setTrackToolbar();
  void setEntitiesToolbar();
  void setGeodeticToolbar();
  void setZoomRangeToolbar();
  void setSimulationToolbar();

  // Called by the track update thread when a track is selected/deselected. Used to update the status
  // of various UI controls and to trigger a display refresh if necessary.
  void trackSelectionStatusChanged( bool trackSelected );

  void lockProjectionOrigin() const;
  void setTimeAcceleration1x() const;
  void setTimeAcceleration10x() const;
  void setTimeAcceleration100x() const;
  void setTimeAcceleration1000x() const;
  void setTimeAcceleration10000x() const;

  void setProjectionStereographicWGS84() const;
  void setProjectionGnomicSphericalEarth() const;
  void setProjectionTransverseMercatorWGS84() const;
  void setProjectionMercator() const;

  void selectPolyline() const;
  void selectPolygon() const;
  void selectText() const;
  void selectVectorSymbol() const;
  void selectRasterSymbol() const;
  void selectArc() const;
  void selectEllipse() const;
  void selectGeoPolyline() const;
  void selectGeoPolygon() const;
  void selectGeoText() const;
  void selectGeoVectorSymbol() const;
  void selectGeoRasterSymbol() const;
  void selectGeoArc() const;
  void selectGeoEllipse() const;

  void saveToTMF();
  void loadFromTMF();

protected:
  virtual void dragEnterEvent( QDragEnterEvent* event );
  virtual void dragMoveEvent( QDragMoveEvent* event );
  virtual void dropEvent( QDropEvent* event );

private:
  void loadLayerFromPath( const QString& qFileName );

  //! @return true if a background map is loaded, false otherwise. A dialog will be displayed if a background map hasn't been loaded
  bool checkBackgroundMapIsLoaded();

  LayerManager* m_layerManager;

  // Status bar widgets
  QLabel* const m_mapUnitCursorPosition;
  QLabel* const m_latLonCursorPosition;
  QLabel* const m_tmcCursorPosition;

  // Dialog boxes / Wizards
#ifdef HAVE_DIRECT_IMPORT_SDK
  DirectImportWizard* m_directImportWizard;
#endif
  CacheSizeDialog* const m_cacheSizeDialog;
  URLDialog* const m_urlDialog;

  // Toolbar options
  QActionGroup* m_interactionModesGroup;
  QActionGroup* m_timeAccelerationGroup;  
  QActionGroup* m_symbolAnnotationGroup;
  QActionGroup* m_symbolTypeGroup;
  QActionGroup* m_projectionGroup;
  QActionGroup* m_entityGroup;

  QComboBox* const m_scaleBox;
  QComboBox* const m_rangeBox;

  QToolBar* m_projectionToolbar;
  QToolBar* m_entitiesToolbar;
  QToolBar* m_geodeticToolbar;
  QToolBar* m_trackToolbar;
  QToolBar* m_simulationToolbar;
  QToolBar* m_zoomRangeToolbar;
  QWidget *m_toolbarSpeedControl;

  // Qt delegate used for editing track hostilities in the UI
  TrackHostilityDelegate *m_hostilityDelegate;
};

#endif // MAINWINDOW_H
