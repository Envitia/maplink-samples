/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/
#ifndef MAPLINKWIDGET_H
#define MAPLINKWIDGET_H

//#include <QMainWindow>
//#include <QEvent>
//#include <QWidget>
//#include <QResizeEvent>
//#include <QPaintEvent>
//#include <QPaintEngine>
//#include <QtGui/QWidget>
//#include <QtGui/QInputEvent>
#include <QGLWidget>
#include <QLabel>

//#include "application.h"
#include <string>

// true - Qt performs the OpenGL Buffer swap
// false - MapLink Pro performs the OpenGL Buffer swap.
#define ML_QT_BUFFER_SWAP true

// true - Qt performs the OpenGL screen clear.
// false - MapLink performs the OpenGL screen clear. NB. The colour MapLink uses is
//         the map background colour - see Application::setMapBackgroundColour.
#define ML_QT_CLEAR_OGL   true

class Application;

////////////////////////////////////////////////////////////////
// A very simple MapLink Pro 'Qt Widget' - MapLink Accelerator SDK
////////////////////////////////////////////////////////////////
class MapLinkWidget : public QGLWidget
{
  Q_OBJECT
public:
  // Constructor
  MapLinkWidget( QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );

  // Map to load.
  void loadMap(const QString &filename);

  // Reset the map view to display the full map.
  void reset();
  void zoomInOnce();
  void zoomOutOnce();
  void pan();
  void grab();
  void zoom();

  // view control
  void previousView();
  void nextView();
  void viewOne();
  void viewTwo();
  void viewThree();

  void resetViews();
  bool setViewOne();
  bool setViewTwo();
  bool setViewThree();


  // Close Widget down.
  virtual bool close();

  // Status label for updating cursor position.
  void statusLabel(QLabel *label);

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

  // Event filter used when we have a parent to ensure we
  // get mouse, keyboard and resize events.
  bool eventFilter(QObject *obj, QEvent *event);

  // Important for Qt4 to stop Qt drawing to this widget.
  // Does not need to be defined when using an QOWidget.
  //virtual QPaintEngine *paintEngine() const
  //{
  //  return 0;
  //}

  // QWidget Resize Event - do not define for a QGLWidget as it
  // causes a recursive call into the Qt event handling.
  //void resizeEvent( QResizeEvent* event );
private:
  // Create Our Application - Wraps the MapLink Sample logic and
  // calls to MapLink.
  void create();

  // Application instance.
  Application *m_application;

  // Map Filename
  QString m_filename;

  // Background clear colour.
  QColor qtWhite;

  // Status Label for writing mouse position too.
  QLabel *m_statusLabel;

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
