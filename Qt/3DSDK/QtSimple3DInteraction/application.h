/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#ifndef WIN32
# include <X11/Xlib.h>
#else
# include <windows.h>
#endif

#include "MapLink.h"
#include "MapLink3D.h"
#include "MapLink3DIMode.h"

class Application : public TSL3DInteractionModeRequest
{
public:
  Application();
  virtual ~Application();

  // Drawing
  void onInitialUpdate( TSL3DRenderingCallback callbackFunc, void *arg );
  void onSize(int cx, int cy) ;
  void onDraw();
  void redraw();
  void reset();

  // Mouse event handlers
  bool onMouseMove(TSLButtonType button, bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool onLButtonDown(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool onMButtonDown(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool onRButtonDown(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool onLButtonUp(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool onMButtonUp(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool onRButtonUp(bool shiftPressed, bool controlPressed, int X, int Y) ;
  bool onMouseWheel(short zDelta, int X, int Y);

  virtual void resetMode( TSL3DInteractionMode* newMode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU );
  virtual void viewChanged( TSL3DDrawingSurface* drawingSurface );

  // Functions invoked from the application via the user interfae
  bool loadMap( const char *mapFilename );
  bool loadTerrain( const char *terrainFilename );
  bool zoomIn();
  bool zoomOut();
  bool resetView();
  void activateEyePointInteractionMode();
  void activateWorldInteractionMode();
  void setWireframeMode( bool wireframe );
  void setTerrainExaggeration( bool exaggerate );
  void setCameraAltitudeLimit( bool limit );

  // Query functions to get information to display in the window
  const char* interactionModePrompt() const;

  // Display items necessary to construct the drawing surface
#ifndef WIN32
  void drawingInfo( GLXDrawable drawable, Display *display, Screen *screen, GLXContext context, Colormap colourmap, Visual *visual );
#else
  void drawingInfo( HGLRC context, WId hwnd );
#endif

private:
  // The data layer containing the map
  TSLMapDataLayer *m_mapDataLayer;

  // The data layer for terrain databases
  TSL3DTerrainDataLayer *m_terrainDataLayer;

  // Name of my map layer
  static const char *m_mapLayerName;

  // Name of my terrain layer
  static const char *m_terrainLayerName;

  // The location of the earth texture we are using
  std::string m_earthTexture;

#ifndef WIN32
  // Drawing Surface
  TSL3DX11GLSurface *m_drawingSurface;

  // What to draw to
  Display *m_display;
  GLXDrawable m_drawable;
  GLXContext m_context;
  Screen *m_screen;
  Colormap m_colourmap;
  Visual *m_visual;

  // Interaction manager
  TSL3DInteractionModeManagerX11 *m_modeManager;
#else
  // Drawing Surface
  TSL3DWinGLSurface *m_drawingSurface;

  HGLRC m_context;
  WId m_hwnd;

  // Interaction manager
  TSL3DInteractionModeManagerNT *m_modeManager;
#endif

  // height and width of area we are drawing to
  int m_width;
  int m_height;

  // Whether we have set up the drawing surface or not
  bool m_initialUpdate;
};

#endif

