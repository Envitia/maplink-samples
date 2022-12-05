/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAPLINKGLSURFACEWIDGET_H
#define MAPLINKGLSURFACEWIDGET_H

// This class is a simple Qt widget that contains a MapLink OpenGL 2D drawing
// surface and the basic MapLink interaction modes.

#include <QGLWidget>
#include <QLabel>

#include <string>

class TSLOpenGLSurface;
class TSLWGLSurface;
class TSLGLXSurface;
class TSLDrawingSurface;
class TSLInteractionModeManagerGeneric;
class TrackSelectionMode;

#include "tsldrawingsurfacedrawcallback.h"
#include "tslatomic.h"
#include "tslbuttontype.h"
#include "MapLinkIModeDLL.h"
#include "tslinteractionmoderequest.h"

class MapLinkGLSurfaceWidget : public QGLWidget, public TSLDrawingSurfaceDrawCallback, public TSLInteractionModeRequest
{
  Q_OBJECT
public:
  MapLinkGLSurfaceWidget( QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
  virtual ~MapLinkGLSurfaceWidget();

  // Interaction Mode request implementations.
  virtual void resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU);
  virtual void viewChanged(TSLDrawingSurface* drawingSurface);

  // Query function to access the MapLink drawing surface for the widget
  TSLOpenGLSurface* drawingSurface();

  void resetOnResize( bool reset );

public slots:
  // Slot handlers for toolbar buttons triggered from the main view
  void resetView();
  void zoomIn();
  void zoomOut();
  void activateZoomMode();
  void activatePanMode();
  void activateGrabMode();
  void activateTrackSelectMode();

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

  // Notifications from MapLink for when redraws are needed
  virtual void redrawRequired (const TSLEnvelope& extent, unsigned int pendingOperations);

  // Event filter used when we have a parent to ensure we
  // get mouse, keyboard and resize events.
  bool eventFilter(QObject *obj, QEvent *event);

private:
  // The MapLink drawing surface responsible for drawing to the widget
#ifdef WIN32
  TSLWGLSurface *m_drawingSurface;
#else
  TSLGLXSurface *m_drawingSurface;
#endif

  // Interaction manager - this handles panning and zooming around the map
  // based on the active interaction mode
  TSLInteractionModeManagerGeneric *m_modeManager;

  // A custom interaction mode used to select tracks for further operations
  TrackSelectionMode *m_trackSelectMode;

  // Current dimensions of the widget
  int m_widgetWidth;
  int m_widgetHeight;

  bool m_resetOnResize;
};

inline void MapLinkGLSurfaceWidget::resetOnResize( bool reset )
{
  m_resetOnResize = reset;
}

#endif // MAPLINKGLSURFACEWIDGET_H
