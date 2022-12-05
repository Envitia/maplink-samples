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

#include "application.h"

//! Qt and X11 use Bool.
#ifdef Bool
#undef Bool
#endif

class MapLinkWidget : public QWidget
{
  Q_OBJECT
public:
  MapLinkWidget(QWidget *parent = 0);//, Qt::WFlags flags = 0);
  virtual ~MapLinkWidget();

  //! Loads a map/file
  void loadMap(const char *filename);

  virtual bool close();
  
  //! Event handlers invoked by the main window
  void resetView();
  void zoomInOnce();
  void zoomOutOnce();
  void activatePanMode();
  void activateGrabMode();
  void activateZoomMode();

  //! set the call back to update the GUI for reseting interaction modes.
  void ResetInteractionModesCallBack(resetInteractionModesCallBack func);

signals:
  void mapDrawn();

protected:
  //! resize - informs the drawing surface of any change in size of the window
  //! Relys on the ResizeAction to maintain the view of the map sensibly
  virtual void resizeEvent( QResizeEvent * );

  //! paint the window
  virtual void paintEvent ( QPaintEvent * );

  //! Keyboard and Mouse events.
  virtual void mouseMoveEvent( QMouseEvent *event );
  virtual void mousePressEvent( QMouseEvent *event );
  virtual void mouseReleaseEvent( QMouseEvent *event );
  virtual void wheelEvent( QWheelEvent *event ); 
  virtual void keyPressEvent( QKeyEvent *event );
  virtual void keyReleaseEvent( QKeyEvent *event );

  //! Important for Qt4 to stop Qt drawing to this widget.
  virtual QPaintEngine *paintEngine() const
  {
    return 0;
  }

private:
  //! Creates the MapLink drawing surface and associated map data layer
  void create();

  //! Application instance - this contains all the MapLink related code.
  Application *m_application;
  
  //! flag to be set when the map is initialized
  bool m_initialized;
};


#endif //! MAPLINKSAMPLE2_H
