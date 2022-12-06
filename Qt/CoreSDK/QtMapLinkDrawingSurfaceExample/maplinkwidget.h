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

//#include <windows.h>
#define tslwin32printcontext_h

#include "MapLink.h"
#include "application.h"

#include <string>


class MapLinkWidget : public QWidget
{
  Q_OBJECT
public:
  MapLinkWidget(QWidget *parent = 0);//, Qt::WFlags flags = 0);
  void loadMap(const char *filename)
  {
    if (m_application)
    {
      m_application->loadMap(filename);
      m_application->reset();
    }
  }

  virtual bool close();

signals:
  void mapDrawn();

protected:
  virtual void resizeEvent( QResizeEvent * );
  virtual void paintEvent ( QPaintEvent * );

  virtual void mouseMoveEvent( QMouseEvent *event );
  virtual void mousePressEvent( QMouseEvent *event );
  virtual void mouseReleaseEvent( QMouseEvent *event );
  virtual void wheelEvent( QWheelEvent *event );
  
  virtual void keyPressEvent( QKeyEvent *event );
  virtual void keyReleaseEvent( QKeyEvent *event );

  // Important for Qt4 to stop Qt drawing to this widget.
  virtual QPaintEngine *paintEngine() const
  {
    return 0;
  }
private:
  void create();
  void resizeCanvas();

  Application *m_application;
};


#endif // MAPLINKSAMPLE2_H
