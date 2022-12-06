/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QGLWidget>

/////////////////////////////////////////////////////////////////////
// Include MapLink Pro Headers...
//
// Define some required Macros and include X11 and Win32 headers as
// necessary.
//
// Define: TTLDLL & WIN32 within the project make settings.
//
/////////////////////////////////////////////////////////////////////
#if defined(WIN32) || defined(_WIN64) || defined(Q_WS_WIN)
# ifndef WINVER
#  define WINVER 0x502
# endif
# ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
# endif
# ifndef WIN32
#  define WIN32  1
# endif
# include <windows.h>
#endif

#include <qtreewidget.h>
#include "MapLink.h"
#include "MapLinkIMode.h"
#include "MapLinkOpenGLSurface.h"

#include "attributetreewidget.h"

class TSLKMLDataLayer;
/////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// Main Application class.
//
// Contains the calls to MapLink and the simple application
// code.
////////////////////////////////////////////////////////////////
class Application : public TSLInteractionModeRequest
{
public:
  Application( QWidget *parent );
  virtual ~Application();

  // Creates the MapLink drawing surface and associated map data layer
  void create();

  // Called when the size of the window has changed
  void resize(int width, int height);

  // Called to redraw the map
  void redraw();

  // Mouse and Keyboard events - if the method returns true it indicates that the widget needs to redraw
  bool mouseMoveEvent(unsigned int button, bool shiftPressed, bool controlPressed, int x, int y) ;
  bool onLButtonDown(bool shiftPressed, bool controlPressed, int x, int y) ;
  bool onMButtonDown(bool shiftPressed, bool controlPressed, int x, int y) ;
  bool onRButtonDown(bool shiftPressed, bool controlPressed, int x, int y) ;
  bool onLButtonUp(bool shiftPressed, bool controlPressed, int x, int y) ;
  bool onMButtonUp(bool shiftPressed, bool controlPressed, int x, int y) ;
  bool onRButtonUp(bool shiftPressed, bool controlPressed, int x, int y) ;
  bool onKeyPress(bool shiftPressed, bool controlPressed, int keySym);
  bool onMouseWheel(bool shiftPressed, bool controlPressed, short zDelta, int x, int y);

  // Interaction Mode control - if the method returns true it indicates that the widget needs to redraw
  bool zoomIn();
  bool zoomOut();
  void resetView();
  void activatePanMode();
  void activateZoomMode();
  void activateGrabMode();

  // Interaction Mode request implementations.
  virtual void resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU);
  virtual void viewChanged(TSLDrawingSurface* drawingSurface);

  // load map and create layers
  bool loadMap(const char *mapFilename);

  // load map and populate KML attribute tree
  bool loadKML(const char* kmlFilename, AttributeTreeWidget* attributeTree);

  // Information to enable Drawing surface to draw.
#ifdef X11_BUILD
  void drawingInfo(Display *display, Screen *screen);
#else
  void drawingInfo(WId window);
#endif

private:
  // The data layer containing the map
  TSLMapDataLayer *m_mapDataLayer;

  // Name of my map layer
  static const char *m_mapLayerName;

  // Drawing Surface Setup.
#ifdef X11_BUILD
  // The MapLink OpenGL drawing surface
  TSLGLXSurface *m_drawingSurface;

  // The display connection and screen to use
  Display *m_display;
  Screen *m_screen;
#else
  // The MapLink OpenGL drawing surface
  TSLWGLSurface *m_drawingSurface;
  
  // The window to draw to
  WId m_window;
#endif

  // Interaction manager - this handles panning and zooming around the map
  // based on the active interaction mode
  TSLInteractionModeManagerGeneric *m_modeManager;

  // The size of the window the drawing surface is attached to
  TSLDeviceUnits m_widgetWidth;
  TSLDeviceUnits m_widgetHeight;

  // Rotation of the drawing surface in radians
  double m_surfaceRotation;

  QWidget *m_parentWidget;

  TSLKMLDataLayer *m_kmlLayer;
  AttributeTreeWidget* m_attributeTree;
};

#endif
