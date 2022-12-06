/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#ifndef WINNT
# include <X11/Xlib.h>
#else
#endif

enum { ID_OVERLAYS_NONE, ID_OVERLAYS_LINE, ID_OVERLAYS_POLYGON, ID_OVERLAYS_TEXT, ID_OVERLAYS_SYMBOL, ID_OVERLAYS_FEATURE } ;

class Point
{
public:
  Point() : m_x(0), m_y(0) {}
  Point(const Point &p) : m_x(p.m_x), m_y(p.m_y) {}
  Point(int x, int y) : m_x(x), m_y(y) {}
  int m_x;
  int m_y;
};

class Application
{
public:
  Application();
  virtual ~Application();

  // Drawing
  void OnInitialUpdate();
  void OnSize(int cx, int cy) ;
  void OnDraw(TSLEnvelope *envelope = NULL);
  void redraw();
  void reset();

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

  // load map and create layers
  bool loadMap(const char *mapFilename);

  //
  bool createOverlay( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds ) ;
 
  //
  void OnOverlaysPolygon();
  void OnOverlaysPolyline();
  void OnOverlaysSymbol();
  void OnOverlaysText();
  void OnOverlaysFeature();
 
  // information to enable Drawing surface to draw.
#ifndef WINNT
  void drawingInfo(Drawable drawable, Display *display, Screen *screen, Colormap colourmap, Visual *visual);
#else
  void drawingInfo(WId window);
#endif

protected:
  void setMapBackgroundColour();
  
  bool createText( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds  ) ;
  bool createPolygon( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds ) ;
  bool createPolyline( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds  ) ;
  bool createSymbol( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds ) ;
  bool createFeature( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds ) ;

private:
  // The data layer containing the map
  TSLMapDataLayer * m_mapDataLayer ;

  TSLStandardDataLayer *m_stdDataLayer;
  int m_overlayType;

  // Name of my map layer
  static const char * m_mapLayerName ;

  // Flag to say whether valid map is loaded.
  bool m_mapLoaded ;

#ifndef WINNT
  // Drawing Surface
  TSLMotifSurface *m_drawingSurface;

  // What to draw to!
  Display *m_display;
  Drawable m_drawable;
  Screen *m_screen;
  Colormap m_colourmap;
  Visual *m_visual;
#else
  TSLNTSurface *m_drawingSurface;

  WId m_window;
#endif

  // height and width of area we are drawing to
  int m_width;
  int m_height;

  bool m_initialUpdate;

  // mouse
  Point m_lmb ;
  Point m_rmb ;
  Point m_lastGrabPoint ;
  bool m_grabbed ;
  bool m_checkForGrab ;
};

#endif

