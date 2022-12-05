/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QtGui>
#ifndef WINNT
# include <X11/Xlib.h>
#else
#endif

/////////////////////////////////////////////////////////////////////
//! Include MapLink Pro Headers...
//
//! Define some required Macros and include X11 and Win32 headers as
//! necessary.
//
//! Define: TTLDLL & WIN32 within the project make settings.
//
/////////////////////////////////////////////////////////////////////
#if defined(WIN32) || defined(_WIN64) || defined(Q_WS_WIN)
# ifndef WINVER
#  define WINVER 0x502
# endif
# ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN		//! Exclude rarely-used stuff from Windows headers
# endif
# ifndef WIN32
#  define WIN32  1
# endif
# include <windows.h>
#endif

#include "MapLink.h"
#include "MapLinkIMode.h"

#include "clientmanager.h"

typedef void(*resetInteractionModesCallBack)();

////////////////////////////////////////////////////////////////
//! Main Application class.
//
//! Contains the calls to MapLink and the simple application
//! code.
////////////////////////////////////////////////////////////////
class Application : public TSLInteractionModeRequest
{
public:
  Application(QWidget *parent);
  virtual ~Application();

  //! Creates the MapLink drawing surface and associated map data layer
  void create();

  //! Called when the size of the window has changed
  void resize(int width, int height);

  //! Called to redraw the map
  void redraw();

  //! Mouse and Keyboard events - if the method returns true it indicates that the widget needs to redraw
  bool mouseMoveEvent(unsigned int button, bool shiftPressed, bool controlPressed, int X, int Y);
  bool OnLButtonDown(bool shiftPressed, bool controlPressed, int X, int Y);
  bool OnMButtonDown(bool shiftPressed, bool controlPressed, int X, int Y);
  bool OnRButtonDown(bool shiftPressed, bool controlPressed, int X, int Y);
  bool OnLButtonUp(bool shiftPressed, bool controlPressed, int X, int Y);
  bool OnMButtonUp(bool shiftPressed, bool controlPressed, int X, int Y);
  bool OnRButtonUp(bool shiftPressed, bool controlPressed, int X, int Y);
  bool OnKeyPress(bool shiftPressed, bool controlPressed, int keySym);
  bool OnMouseWheel(bool shiftPressed, bool controlPressed, short zDelta, int X, int Y);

  //! Interaction Mode control - if the method returns true it indicates that the widget needs to redraw
  bool zoomIn();
  bool zoomOut();
  void resetView();
  void activatePanMode();
  void activateZoomMode();
  void activateGrabMode();
  void activateTracksMode();

  //! Interaction Mode request implementations.
  virtual void resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU);
  virtual void viewChanged(TSLDrawingSurface* drawingSurface);

  //! load map and create layers
  bool loadMap(const char *mapFilename);

  //! information to enable Drawing surface to draw.
#ifndef WINNT
  void drawingInfo(Drawable drawable, Display *display, Screen *screen, Colormap colourmap, Visual *visual);
#else
  void drawingInfo(WId window);
#endif

  //! set the call back to update the GUI for reseting interaction modes.
  void ResetInteractionModesCallBack(resetInteractionModesCallBack func);

  //! get client manager
  ClientManager* getClientManager();

  //! handles clicking left click in the tracks mode.
  bool onTracksModeLeftClick(TSLDeviceUnits x, TSLDeviceUnits y);

protected:
  void setMapBackgroundColour();

private:
  //! The data layer containing the map
  TSLMapDataLayer * m_mapDataLayer;

  //! Name of my map layer
  static const char * m_mapLayerName;

#ifndef WINNT
  //! The MapLink drawing surface
  TSLMotifSurface *m_drawingSurface;

  //! The display connection and screen to use
  Display *m_display;
  Drawable m_drawable;
  Screen *m_screen;
  Colormap m_colourmap;
  Visual *m_visual;
#else

  //! The MapLink drawing surface
  TSLNTSurface *m_drawingSurface;

  //! The window to draw to
  WId m_window;
#endif

  //! Interaction manager - this handles panning and zooming around the map
  //! based on the active interaction mode
  TSLInteractionModeManagerGeneric *m_modeManager;

  //! call back to update the GUI for reseting interaction modes.
  resetInteractionModesCallBack m_resetInteractionModesCallBack;

  //! The size of the window the drawing surface is attached to
  int m_widgetWidth;
  int m_widgetHeight;

  //! Rotation of the drawing surface in radians
  double m_surfaceRotation;

  //! flag set if the window is not created
  bool m_tobeCreated;

  //! parent widget
  QWidget *m_parentWidget;

  //! client manager
  ClientManager* m_clientManager;
};

#endif

