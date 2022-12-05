/****************************************************************************
                Copyright (c) 2013-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAPLINKWIDGET_H
#define MAPLINKWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
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
class OffScreenHelper;
class QOpenGLTexture;
class QOpenGLShaderProgram;
class QOpenGLShader;

////////////////////////////////////////////////////////////////
// A very simple MapLink Pro 'Qt Widget' using the new
// QOpenGLWidget.
////////////////////////////////////////////////////////////////
class MapLinkWidget : public QOpenGLWidget, protected QOpenGLFunctions, public TSLDrawingSurfaceDrawCallback
{
  Q_OBJECT
public:
  MapLinkWidget( QWidget * parent = NULL, Qt::WindowFlags f = Qt::WindowFlags() );
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

  QSize minimumSizeHint() const ;
  QSize sizeHint() const ;

protected:
  // Qt OpenGL drawing overrides
  virtual void initializeGL();
  virtual void paintGL();
  virtual void resizeGL(int width, int height);

  virtual void paintEvent(QPaintEvent *e);


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

  // Used to draw the MapLink Drawing Surface Texture to the screen
  QOpenGLTexture *m_texture;
  QOpenGLShaderProgram *m_program;
  QOpenGLBuffer m_vbo;
  QOpenGLVertexArrayObject m_vao;
  QOpenGLShader *m_vShader;
  QOpenGLShader *m_fShader;

  QVector<GLfloat> vertData;
  
  // colour to clear the window too
  QColor m_clearColor;
  // Offscreen rendering helper for drawing the MapLink Pro drawing surface.
  OffScreenHelper *m_offScreenHelper;

  int m_width;
  int m_height;
};


#endif // MAPLINKWIDGET_H
