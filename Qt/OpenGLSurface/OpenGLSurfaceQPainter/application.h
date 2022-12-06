/****************************************************************************
                Copyright (c) 2013-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

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

#include "MapLink.h"
#include "MapLinkIMode.h"
#include "MapLinkOpenGLSurface.h"


class OffScreenHelper;

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

  // Initialises this object and MapLink layers
  void create( TSLDrawingSurfaceDrawCallback *redrawCallback, TSLOpenGLSurface *surface, OffScreenHelper *offscreen);

  // Called when the size of the window has changed
  void resize(int width, int height);

  // Called to draw the map and layers
  GLuint draw();

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

  // Called when the menu item of the same name is triggered to turn buffered layer tiling on or off
  void enableBufferedLayerTiling( bool enable );

  // Interaction Mode request implementations.
  virtual void resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU);
  virtual void viewChanged(TSLDrawingSurface* drawingSurface);

  // load map/file and create layers
  bool loadFile(const char *fileFilename);

private:
  // The data layer containing the map
  TSLMapDataLayer *m_mapDataLayer;

  // Name of my map layer
  static const char *m_mapLayerName;

  // Vector overlay
  TSLStandardDataLayer *m_stdDataLayer;
  static const char *m_stdLayerName;

  // Drawing Surface Setup.
  // This is owned by the OffScreenHelper class
  TSLOpenGLSurface *m_drawingSurface;

  // Offscreen Helper class - this is used to provide a tighter integration with Qt.
  // This is not owned by this class.
  OffScreenHelper *m_offScreenHelper;

  // Interaction manager - this handles panning and zooming around the map
  // based on the active interaction mode
  TSLInteractionModeManagerGeneric *m_modeManager;

  // The size of the window the drawing surface is attached to
  TSLDeviceUnits m_widgetWidth;
  TSLDeviceUnits m_widgetHeight;

  // Rotation of the drawing surface in radians
  double m_surfaceRotation;

  QWidget *m_parentWidget;
};

#endif
