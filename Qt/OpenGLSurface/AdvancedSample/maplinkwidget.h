/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAPLINKWIDGET_H
#define MAPLINKWIDGET_H

#include <QGLWidget>
#include <QLabel>
//#include <QProgressDialog>
#include <QTimer>
#include <QTime>

#include <string>
#include "ui/datalayerdialog.h"
#include "ui/tmflayerdialog.h"
#include "ui/attributetreewidget/attributetreewidget.h"
#include "ui/layertreeview/treemodel.h"

#ifdef HAVE_DIRECT_IMPORT_SDK
# include "MapLinkDirectImport.h"
class DirectImportDataSetTreeItem;
class DirectImportScaleBandTreeItem;
#endif

#ifdef _MSC_VER
class TSLWGLSurface;
#else
class TSLGLXSurface;
#endif

// Determines who will perform the buffer swap after a draw:
// true - Qt performs the OpenGL buffer swap
// false - MapLink Pro performs the OpenGL buffer swap.
#define ML_QT_BUFFER_SWAP true

class Application;
class AddWaypointInteractionMode;
class LayerManager;

////////////////////////////////////////////////////////////////
// A very simple MapLink Pro 'Qt Widget'
////////////////////////////////////////////////////////////////
class MapLinkWidget : public QGLWidget
# ifdef HAVE_DIRECT_IMPORT_SDK
  , public TSLDirectImportDataLayerCallbacks
# endif
{
  Q_OBJECT
public:

  MapLinkWidget( QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
  virtual ~MapLinkWidget();

  bool loadMap( const char *mapToLoad, bool useSharedCache );
  void displayZoomScaleAndRange();
  void calculateZoomScaleAndRange( int&, double&, double& );
  void setViewScale( double scaleFactor );
  void setViewRange( double range );


  // Event handlers invoked by the main window
  void resetView();
  void zoomInOnce();
  void zoomOutOnce();
  void activatePanMode() const;
  void activateGrabMode() const;
  void activateZoomMode() const;
  void activateWaypointMode() const;
  void activateTrackSelectMode() const;

  void moveLayerToIndex( const char*, const char*, int );
  void editTreeModel( const QModelIndex&, long );
  void removeTreeLayer( std::string name, std::string treeAttribute );

  void handleDataDialog( const std::string& filename );
  void handleTMFDialog( const std::string& filename );
#ifdef HAVE_DIRECT_IMPORT_SDK
  void moveDirectImportDataSetToIndex( const char* layerName, const char* scaleBandName, int rowFrom, int rowTo );
  void removeDirectImportDataSet( const char* layerName, const char* scaleBandName, int row );
  //! Called before the direct import wizard is displayed
  void beginDirectImportLoad();
  //! Called after the direct import wizard is displayed
  void endDirectImportLoad( const std::string& layerName );
#endif
  //void handleProgressDialog( std::string );
  //void closeProgressDialog();

  void pauseUpdate( bool );

  void selectEntity( std::string );
  void setAttributeTree( AttributeTreeWidget* ) const;
  TreeModel* getTreeModel();
  LayerManager* layerManager();

# ifdef _MSC_VER
  TSLWGLSurface* drawingSurface() const;
# else
  TSLGLXSurface* drawingSurface() const;
# endif

signals:
  //! Emitted when the declutter list needs to be updated
  void updateDeclutterList( const QString& layerName );

public slots:
  void zoomToLayerExtent( std::string );
#ifdef HAVE_DIRECT_IMPORT_SDK
  void zoomToLayerDataSetExtent( DirectImportDataSetTreeItem* );
  void zoomToLayerScaleBandExtent( DirectImportScaleBandTreeItem* );
#endif
  void selectedAttributeData( std::string, std::string ) const;
  void dataDialogOKButton();
  void tmfDialogOKButton();
  void tmfBrowseButton();
  void idle();

  void saveToTMF( const char* filename );
  void loadFromTMF( const char* filename );

protected:
  // Qt OpenGL drawing overrides
  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL( int width, int height );

  // Keyboard and Mouse events.
  virtual void mouseMoveEvent( QMouseEvent *event );
  virtual void mousePressEvent( QMouseEvent *event );
  virtual void mouseReleaseEvent( QMouseEvent *event );
  virtual void wheelEvent( QWheelEvent *event );
  virtual void keyPressEvent( QKeyEvent *event );

  // Event filter used when we have a parent to ensure we
  // get mouse, keyboard and resize events.
  bool eventFilter( QObject *obj, QEvent *event );

private:
  bool m_initialise;

  // Application instance - this contains all the MapLink related code.
  Application* const m_application;

  DataLayerDialog* const m_dataLayerDialog;
  TMFLayerDialog* const m_tmfLayerDialog;

  //QProgressDialog* const m_progressDialog;

  QTimer m_viewUpdater;
  bool m_pauseUpdate;

  // Used for debounce in wheelEvent()
  QTime m_lastWheelEventTime;

  std::string m_selectedEntity;
  std::string m_selectedFile;

  LayerManager* m_layerManager;

  // Labels for displaying the current cursor position
  QLabel* m_statusBarTMCPosition;
  QLabel* m_statusBarMUPosition;
  QLabel* m_statusBarLatLonPosition;
  QComboBox* m_zoomScaleComboBox;
  QComboBox* m_viewRangeComboBox;
public:

# ifdef HAVE_DIRECT_IMPORT_SDK
  // TSLDirectImportDataLayerCallbacks
  virtual void onDeviceCapabilitiesRequired( TSLDeviceCapabilities& capabilities );
  virtual unsigned int onChoiceOfDrivers( const char* data, const TSLvector<const char*>* drivers );
  virtual const TSLCoordinateSystem* onNoCoordinateSystem( const TSLDirectImportDataSet* dataSet, TSLDirectImportDriver* driver );
  virtual TSLMUExtent onNoExtent( const TSLDirectImportDataSet* dataSet, TSLDirectImportDriver* driver );

  virtual void onDataSetLoadScheduled( const TSLDirectImportDataSet* dataSet, unsigned int numProcessingTotal );
  virtual void onDataSetLoadCancelled( const TSLDirectImportDataSet* dataSet, unsigned int numProcessingTotal );
  virtual void onDataSetLoadComplete( const TSLDirectImportDataSet* dataSet, unsigned int numProcessingTotal );
  virtual void onTileLoadScheduled( const TSLDirectImportDataSet* dataSet, unsigned int numScheduled, unsigned int numProcessing, unsigned int numProcessingTotal );
  virtual void onTileLoadCancelled( const TSLDirectImportDataSet* dataSet, unsigned int numProcessing, unsigned int numProcessingTotal );
  virtual void onTileLoadFailed( const TSLDirectImportDataSet* dataSet, unsigned int numProcessing, unsigned int numProcessingTotal );
  virtual void onTileLoadComplete( const TSLDirectImportDataSet* dataSet, unsigned int numProcessing, unsigned int numProcessingTotal );

  virtual void requestRedraw();
# endif

  void setStatusBarTMCWidget( QLabel* );
  void setStatusBarMUWidget( QLabel* );
  void setStatusBarlatLonWidget( QLabel* );
  void setZoomScaleComboBox( QComboBox* widget );
  void setViewRangeComboBox( QComboBox* widget );
};

inline void MapLinkWidget::setStatusBarTMCWidget( QLabel *widget )
{
  m_statusBarTMCPosition = widget;

  // Initialise status bar text
  m_statusBarTMCPosition->setText( "X = <invalid>  Y = <invalid> (TMC)" );
}

inline void MapLinkWidget::setStatusBarMUWidget( QLabel *widget )
{
  m_statusBarMUPosition = widget;

  // Initialise status bar text
  m_statusBarMUPosition->setText( "X = <invalid>  Y = <invalid> (Map Units)" );
}

inline void MapLinkWidget::setStatusBarlatLonWidget( QLabel *widget )
{
  m_statusBarLatLonPosition = widget;

  // Initialise status bar text
  m_statusBarLatLonPosition->setText( "Latitude = <invalid>  Longitude = <invalid>" );
}

inline void MapLinkWidget::setZoomScaleComboBox( QComboBox* widget )
{
  m_zoomScaleComboBox = widget;
}

inline void MapLinkWidget::setViewRangeComboBox( QComboBox* widget )
{
  m_viewRangeComboBox = widget;
}
#endif // MAPLINKWIDGET_H
