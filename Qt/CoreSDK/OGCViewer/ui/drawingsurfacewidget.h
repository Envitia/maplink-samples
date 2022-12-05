/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DRAWING_SURFACE_WIDGET_H
#define DRAWING_SURFACE_WIDGET_H

#include <QWidget>
#include <QEvent>

class TSLDrawingSurface;

// A simple Qt widget that contains a MapLink drawing surface
// attached to the widget. This can be added to dialogs through the Qt
// designer through the promoted widget system

class DrawingSurfaceWidget : public QWidget
{
  Q_OBJECT
public:
  DrawingSurfaceWidget( QWidget * parent = NULL );
  virtual ~DrawingSurfaceWidget();

  TSLDrawingSurface* drawingSurface();

signals:
  void signalRefreshView();
  void signalResetView();
  void mapDrawn();

public slots:
  void refreshView();
  void resetView();

protected:
  virtual void paintEvent( QPaintEvent *event );
  virtual void resizeEvent( QResizeEvent *event );
  virtual QPaintEngine *paintEngine() const;
  virtual bool event(QEvent * event);

  TSLDrawingSurface *m_surface;
};

inline TSLDrawingSurface* DrawingSurfaceWidget::drawingSurface()
{
  return m_surface;
}

#endif // DRAWING_SURFACE_WIDGET_H
