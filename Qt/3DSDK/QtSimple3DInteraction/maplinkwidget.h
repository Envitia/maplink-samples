/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAPLINKWIDGET_H
#define MAPLINKWIDGET_H

#include <QEvent>
#include <QGLWidget>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPaintEngine>
#include <string>

class Application;

class MapLinkWidget : public QGLWidget
{
  Q_OBJECT

public:
  MapLinkWidget(QWidget *parent = 0);
  virtual ~MapLinkWidget();

  // These functions are invoked by the main window when items on the user interface are triggered
  bool loadMap( const char *filename );
  bool loadTerrain( const char *filename );
  void zoomIn();
  void zoomOut();
  void resetView();
  void activateEyePointInteractionMode();
  void activateWorldInteractionMode();
  void setWireframeMode( bool wireframe );
  void setTerrainExaggeration( bool exaggerate );
  void setCameraAltitudeLimit( bool limit );
  const char* statusBarText() const;

  // Called when the widget is being destroyed to clean up.
  virtual bool close();

  // Callback function to be invoked by the MapLink 3D drawing surface when new imagery
  // is available for drawing. This function should have a protoype that matches
  // the one from TSL3DRenderingCallback.
  static void repaintFunc( void *arg, int pending );

protected:
  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL( int width, int height );

  virtual void mouseMoveEvent( QMouseEvent *event );
  virtual void mousePressEvent( QMouseEvent *event );
  virtual void mouseReleaseEvent( QMouseEvent *event );
  virtual void wheelEvent( QWheelEvent *event );
 
private:
  // The application class contains all interactions with the MapLink API
  Application *m_application;
};


#endif // MAPLINKWIDGET
