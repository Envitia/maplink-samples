/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/
#ifndef MAPLINKWIDGET_H
#define MAPLINKWIDGET_H

#include <QMainWindow>
#include <QEvent>
#include <QWidget>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPaintEngine>
#include <QWidget>
#include <QInputEvent>
#include <QGLWidget>

#include "application.h"
#include <string>

// true - Qt performs the OpenGL Buffer swap
// false - MapLink Pro performs the OpenGL Buffer swap.
#define ML_QT_BUFFER_SWAP true

// true - Qt performs the OpenGL screen clear.
// false - MapLink performs the OpenGL screen clear. NB. The colour MapLink uses is
//         the map background colour - see Application::setMapBackgroundColour.
#define ML_QT_CLEAR_OGL   true

////////////////////////////////////////////////////////////////
// A very simple MapLink Qt Widget.
////////////////////////////////////////////////////////////////
class MapLinkWidget : public QGLWidget 
{
  Q_OBJECT
public:
  MapLinkWidget( const QGLFormat &format, QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );

  // Map to load.
  void loadMap(const QString &filename)
  {
    m_filename = filename;
    if (m_application)
    {
      m_application->loadMap(filename.toUtf8());
      m_application->reset();
    }
    updateGL();
  }

  virtual bool close();

protected:
  // Qt OpenGL drawing overrides
  void initializeGL();
  void paintGL();
  void resizeGL(int width, int height);

  // Handle Keyboard and Mouse events.
  virtual void mouseMoveEvent( QMouseEvent *event );
  virtual void mousePressEvent( QMouseEvent *event );
  virtual void mouseReleaseEvent( QMouseEvent *event );
  virtual void wheelEvent( QWheelEvent *event );
  
  virtual void keyPressEvent( QKeyEvent *event );
  virtual void keyReleaseEvent( QKeyEvent *event );

  // Important for Qt4 to stop Qt drawing to this widget.
  //virtual QPaintEngine *paintEngine() const
  //{
  //  return 0;
  //}
private:
  void create();

  Application *m_application;
  QString m_filename;

  // Background clear colour.
  QColor qtWhite;

  ////////////////////////////////////////////////////////
  // We need to notify the MapLinkWidget when a redraw is
  // required. This needs to be thread safe.
  //
  // http://doc.qt.nokia.com/4.6/signalsandslots.html
  ////////////////////////////////////////////////////////
public:
signals:
  void changed();

  friend class Application;
};


#endif // MAPLINKSAMPLE2_H
