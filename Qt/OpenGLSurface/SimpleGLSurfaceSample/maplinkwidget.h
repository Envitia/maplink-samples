/****************************************************************************
                Copyright (c) 2013-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAPLINKWIDGET_H
#define MAPLINKWIDGET_H

#include <QGLWidget>
#include <QLabel>

#include <string>

#include "MapLink.h"

// Qt and X11 use Bool.
#ifdef Bool
#undef Bool
#endif

// Determines who will perform the buffer swap after a draw:
// true - Qt performs the OpenGL buffer swap
// false - MapLink Pro performs the OpenGL buffer swap.
#define ML_QT_BUFFER_SWAP true

class Application;

////////////////////////////////////////////////////////////////
// A very simple MapLink Pro 'Qt Widget'
////////////////////////////////////////////////////////////////
class MapLinkWidget : public QGLWidget, public TSLDrawingSurfaceDrawCallback
{
  Q_OBJECT
public:
  MapLinkWidget( QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
  virtual ~MapLinkWidget();

  // Loads a map/file
  void loadFile( const char *filename);

  // Event handlers invoked by the main window
  void resetView();
  void zoomInOnce();
  void zoomOutOnce();
  void activatePanMode();
  void activateGrabMode();
  void activateZoomMode();
  void enableBufferedLayerTiling( bool enable );

protected:
  // Qt OpenGL drawing overrides
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

  // Keyboard and Mouse events.
  virtual void mouseMoveEvent( QMouseEvent *event );
  virtual void mousePressEvent( QMouseEvent *event );
  virtual void mouseReleaseEvent( QMouseEvent *event );
  virtual void wheelEvent( QWheelEvent *event );
  virtual void keyPressEvent( QKeyEvent *event );

  // Notifications from MapLink for when redraws are needed
  virtual void redrawRequired (const TSLEnvelope& extent, unsigned int pendingOperations);

  // Event filter used when we have a parent to ensure we
  // get mouse, keyboard and resize events.
  bool eventFilter(QObject *obj, QEvent *event);

private:
  // Application instance - this contains all the MapLink related code.
  Application *m_application;
};


#endif // MAPLINKWIDGET_H
