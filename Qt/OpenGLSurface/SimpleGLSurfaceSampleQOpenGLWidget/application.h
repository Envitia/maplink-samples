/****************************************************************************
                Copyright (c) 2013-2017 by Envitia Group PLC.
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

#include "MapLink.h"
#include "MapLinkIMode.h"
#include "MapLinkOpenGLSurface.h"
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
  void create( TSLDrawingSurfaceDrawCallback *redrawCallback );

  // Called when the size of the window has changed
  void resize(int width, int height);

  /// Called to redraw the map
  /// @param fbo The frame buffer to render to
  void redraw(GLuint fbo);

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

  // Information to enable Drawing surface to draw.
  // The provided context must be active when this method is called,
  // this is usually performed by calling QOpenGLWidget::makeCurrent()
  void drawingInfo(QOpenGLContext* context);

private:
  // The data layer containing the map
  TSLMapDataLayer *m_mapDataLayer;

  // Name of my map layer
  static const char *m_mapLayerName;

  // Vector overlay
  TSLStandardDataLayer *m_stdDataLayer;
  static const char *m_stdLayerName;

  // Drawing Surface Setup.
  QOpenGLContext* m_context = nullptr;
#ifdef X11_BUILD
  // The MapLink OpenGL drawing surface
  TSLGLXSurface *m_drawingSurface = nullptr;
#else
  static QWindow* windowForWidget(const QWidget* widget);

  // The MapLink OpenGL drawing surface
  TSLWGLSurface *m_drawingSurface = nullptr;
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
};

#endif
