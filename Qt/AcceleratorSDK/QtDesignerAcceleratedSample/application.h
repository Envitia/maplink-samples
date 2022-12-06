/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QLabel>
#include <QGLWidget>

/////////////////////////////////////////////////////////////////////
// Include MapLink Pro Headers...
//
// Define some required Macro's and include X11 and Win32 headers as
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
#else
# include <GL/glx.h>
# ifndef X11
#  define X11
# endif
#endif

#define tslwin32printcontext_h
#include "MapLink.h"
#include "MapLinkIMode.h"
#include "maplinkaccelerator.h"
/////////////////////////////////////////////////////////////////////

//
enum { ID_OVERLAYS_NONE, ID_OVERLAYS_LINE, ID_OVERLAYS_POLYGON, ID_OVERLAYS_TEXT, ID_OVERLAYS_SYMBOL, ID_OVERLAYS_FEATURE } ;

class AcceleratedSampleCustomDataLayer;

////////////////////////////////////////////////////////////////
// Main Application class.
//
// Contains the calls to MapLink and the simple application
// code.
////////////////////////////////////////////////////////////////
class Application : public TSLInteractionModeRequest
{
public:
  Application(QGLWidget *widget, QLabel *statusLabel);
  virtual ~Application();

  // Drawing
  void OnInitialUpdate();
  void OnSize(int cx, int cy) ;
  void OnDraw();
  void redraw();
  void reset(); // reset the map to full map view.

  // Mouse & Keyboard
  bool OnMouseMove(unsigned int button, bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool OnLButtonDown(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool OnMButtonDown(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool OnRButtonDown(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool OnLButtonUp(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool OnMButtonUp(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool OnRButtonUp(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool OnKeyPress(bool shiftPressed, bool controlPressed, int keySym);
  bool OnMouseWheel(bool shiftPressed, bool controlPressed, short zDelta, int X, int Y);

  // Interaction Mode control - if method returns a bool true indicates that we need to redraw.
  bool OnToolsZoomin();
  bool OnToolsZoomout();
  bool OnToolsReset();
  void OnToolsPan();
  void OnToolsZoom();
  void OnToolsGrab();
  //void OnToolsMagnify(); // Does not work with Accelerator SDK.

  // Goto View controls
  bool OnSavedviewsGoto1();
  bool OnSavedviewsGoto2();
  bool OnSavedviewsGoto3();

  // Reset the set views
  void OnSavedviewsReset();

  // Save a view
  bool OnSavedviewsSet1();
  bool OnSavedviewsSet2();
  bool OnSavedviewsSet3();

  // Move up and down the view stack
  bool OnViewstackPreviousview();
  bool OnViewstackNextview();

  // Interaction Mode implementations.
  virtual void resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU);
  virtual void viewChanged(TSLDrawingSurface* drawingSurface);

  // load map and create layers
  bool loadMap(const char *mapFilename);

  // Functionality demo - not accesible from GUI
  bool createOverlay( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds ) ;
  void OnOverlaysPolygon();
  void OnOverlaysPolyline();
  void OnOverlaysSymbol();
  void OnOverlaysText();
  void OnOverlaysFeature();


  // information to enable Drawing surface to draw.
#ifdef Q_WS_X11
  void drawingInfo(Drawable drawable, Display *display, Screen *screen, Colormap colourmap, Visual *visual);
#else
  void drawingInfo(WId window);
#endif

  // Widget for displaying position of cursor
  void statusLabel(QLabel *label)
  {
    m_statusLabel = label;
  }

protected:
  void setMapBackgroundColour();
  bool addToSurface();

  bool createText( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds  ) ;
  bool createPolygon( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds ) ;
  bool createPolyline( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds  ) ;
  bool createSymbol( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds ) ;
  bool createFeature( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds ) ;

private:
  static void redrawCallback(void *arg);
  void createGeometry(TSLStandardDataLayer *layer);

  // The data layer containing the map
  TSLMapDataLayer * m_mapDataLayer ;

  TSLStandardDataLayer *m_stdDataLayer;
  int m_overlayType;

  // Name of my map layer
  static const char * m_mapLayerName ;

  // Flag to say whether valid map is loaded.
  bool m_mapLoaded ;

  // Drawing Surface Setup.
#ifdef Q_WS_X11
  // Drawing Surface
  TSLX11GLAcceleratedSurface *m_drawingSurface;

  // Interaction manager
  TSLInteractionModeManagerX11 * m_modeManager ;

  // What to draw to!
  Display *m_display;
  Drawable m_drawable;
  Screen *m_screen;
  Colormap m_colourmap;
  Visual *m_visual;

  GLXContext m_context;
#else
  // Drawing Surface
  TSLWGLAcceleratedSurface *m_drawingSurface;

  // Interaction manager
  TSLInteractionModeManagerNT * m_modeManager ;

  // What to draw to!
  WId m_window;
  void * m_hglrc; // stores a HGLRC
#endif

  // Widget we are attached to.
  QGLWidget *m_widget;
  QLabel *m_statusLabel;

  // Accelerator SDK Custom data layer.
  TSLAcceleratedCustomDataLayer *m_customDataLayer;

  // Client to add to the custom Layer : does the user drawing.
  AcceleratedSampleCustomDataLayer *m_customClient;

  // Multithreaded Render Control
  // Should track if the thread has been started or stoped.
  TSLAcceleratedMTRenderControl *m_mtRenderControl;

  // Blocking Render Control
  TSLAcceleratedBlockingRenderControl *m_blockingRenderControl;

  // Surface Configuraton
  TSLAcceleratorConfiguration *m_configuration;

  // height and width of area we are drawing to
  int m_width;
  int m_height;

  // flag to see if we have setup the Drawing Surface and Interaction modes.
  bool m_initialUpdate;
};

#endif

