/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef DRAWINGSURFACEINTERACTIONS_H
#define DRAWINGSURFACEINTERACTIONS_H

#include "drawingsurfacewidget.h"

#include "tslatomic.h"
#include "tslbuttontype.h"
#include "MapLinkIModeDLL.h"
#include "tslinteractionmoderequest.h"

class QLabel;
class TSLInteractionModeManagerGeneric;

// An extension of the basic DrawingSurfaceWidget that adds support for the MapLink
// interaction modes through mouse events on the widget.
// This can be added to dialogs through the Qt designer through the promoted widget system

class DrawingSurfaceInteractions : public DrawingSurfaceWidget, public TSLInteractionModeRequest
{
  Q_OBJECT
public:
  DrawingSurfaceInteractions( QWidget *parent );
  virtual ~DrawingSurfaceInteractions();

  // Interaction Mode request implementations.
  virtual void resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU);
  virtual void viewChanged(TSLDrawingSurface* drawingSurface);

  void setStatusBarMUWidget( QLabel *widget );
  void setStatusBarlatLonWidget( QLabel *widget );

public slots:
  // Slot handlers for toolbar buttons triggered from the main view
  void zoomIn();
  void zoomOut();
  void activateZoomMode();
  void activatePanMode();
  void activateGrabMode();

protected:
  // Mouse events.
  virtual void mouseMoveEvent( QMouseEvent *event );
  virtual void mousePressEvent( QMouseEvent *event );
  virtual void mouseReleaseEvent( QMouseEvent *event );
  virtual void wheelEvent( QWheelEvent *event );

private:
  // Interaction manager - this handles panning and zooming around the map
  // based on the active interaction mode
  TSLInteractionModeManagerGeneric *m_modeManager;

  // Labels for displaying the current cursor position
  QLabel *m_statusBarMUPosition;
  QLabel *m_statusBarLatLonPosition;
};

inline void DrawingSurfaceInteractions::setStatusBarMUWidget( QLabel *widget )
{
  m_statusBarMUPosition = widget;
}

inline void DrawingSurfaceInteractions::setStatusBarlatLonWidget( QLabel *widget )
{
  m_statusBarLatLonPosition = widget;
}
#endif
