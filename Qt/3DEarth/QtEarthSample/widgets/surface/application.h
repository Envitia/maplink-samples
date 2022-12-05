/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

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
#include "interactions/MapLink3DIMode.h"
#include "MapLinkEarth.h"
#include <memory>

#include "managers/datalayers/datalayersmanager.h"
#include "managers/tracks/tracksmanager.h"
#include "managers/geometry/geometrymanager.h"

typedef void(*resetInteractionModesCallBack)();
typedef void(*viewChangedCallBack)();
typedef void(*selectedTrackCallBack)(envitia::maplink::earth::Track * track, uint16_t id);

//! Interaction mode IDs - these can be any numbers
#define ID_TOOLS_NoInteraction            0
#define ID_TOOLS_Select                   1
#define ID_TOOLS_CreatePolygon            2
#define ID_TOOLS_CreatePolyline           3
#define ID_TOOLS_CreateText               4
#define ID_TOOLS_CreateSymbol             5
#define ID_TOOLS_CreateExtrudedPolygon    6
#define ID_TOOLS_CreateExtrudedPolyline   7
#define ID_TOOLS_DeleteGeometry           8
#define ID_TOOLS_Trackball                9

////////////////////////////////////////////////////////////////
//! Main Application class.
//
//! Contains the calls to MapLink and the simple application
//! code.
////////////////////////////////////////////////////////////////
class Application : public InteractionModeRequest
{
public:
  Application(QWidget *parent);
  virtual ~Application();

  //! Creates the MapLink drawing surface and associated map data layer
  void create();

  //! Called when the size of the window has changed
  void resize(int width, int height) ;

  //! Called to redraw the map
  void redraw();

  // load map layer/ terrain database
  bool loadLayer(std::string filePath, std::string &err_msg);

  //! information to enable Drawing surface to draw.
#ifndef WINNT
  void drawingInfo(Drawable drawable, Display *display, Screen *screen, Colormap colourmap, Visual *visual);
#else
  void drawingInfo(WId window);
#endif

  //! set the call back to update the GUI for reseting interaction modes.
  void ResetInteractionModesCallBack(resetInteractionModesCallBack func);
  //! set the call back to update the GUI for view changed.
  void ViewChangedCallBack(viewChangedCallBack func);
  //! set the call back to update the GUI for selected track info.
  void SelectedTrackCallBack(selectedTrackCallBack func);

public:    /******** drawing surface functions ********/
	// Add a datalayer to the view
	bool addDataLayer(TSLDataLayer* layer, std::string layerName);

	// Add a terrain database to the view
	bool addTerrainDatabase(TSLTerrainDatabase& database, std::string databaseName);

	// Add tracks to the view
	bool addTracks(std::vector<earth::Track>& tracks);

	// Add geometry to the view
	bool addGeometry(std::vector<std::unique_ptr<earth::geometry::Geometry>>& geom);

	// Render the view to an HDC
	bool drawToHDC(TSLDeviceContext& hdc, TSLDeviceUnits width, TSLDeviceUnits height);

	void setMapBackgroundColour();

	// Initialize and add tracks to the view
	void startTracks(int numTracks);

	// Remove tracks from the view
	void removeTracks();

	//! The MapLink drawing surface
	envitia::maplink::earth::Surface3D* drawingSurface() { return m_drawingSurface; };

public:        /******** Interaction modes ********/
	// initialize interaction mode manager
	void initializeModeManager();

	// Set the interaction of the view
	void interaction(int i, bool reactivate = false);
	int currentInteractionMode();

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

	//! Interaction Mode request implementations.
	virtual void resetMode(Interaction * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU) override final;
	virtual void viewChanged(envitia::maplink::earth::Surface3D* drawingSurface) override final;
	virtual void geometryAdded(earth::geometry::Geometry* geom) override final;
	virtual void geometryRemoved(earth::geometry::Geometry* geom) override final;
	virtual void geometrySelected(earth::geometry::Geometry* geom) override final;
	virtual void trackSelected(earth::Track* track) override final;
	virtual TSLEnvelope GetWindowEnvelope() override final;

	//! Interaction Mode control - if the method returns true it indicates that the widget needs to redraw
	void activate_Trackball_Mode();
	void activate_Select_Mode();
	void activate_CreatePolygon_Mode();
	void activate_CreatePolyline_Mode();
	void activate_CreateText_Mode();
	void activate_CreateSymbol_Mode();
	void activate_CreateExtrudedPolygon_Mode();
	void activate_CreateExtrudedPolyline_Mode();
	void activate_DeleteGeometry_Mode();

	bool zoomIn();
	bool zoomOut();
	void resetView();

public:	/******** properties control ********/
	// object to manage the map and terrain data layers
	DatalayersManager& datalayersManager();

	// object to manage the tracks
	TracksManager& tracksManager();

	// geometry manager
	GeometryManager& geometryManager();

private:
  //! The data layer containing the map
  TSLMapDataLayer * m_mapDataLayer ;

  //! Name of my map layer
  static const char * m_mapLayerName ;

  //! The MapLink drawing surface
  envitia::maplink::earth::Surface3D* m_drawingSurface;
#ifndef WINNT
  //! The display connection and screen to use
  Display *m_display;
  Drawable m_drawable;
  Screen *m_screen;
  Colormap m_colourmap;
  Visual *m_visual;
#else
  //! The window to draw to
  WId m_window;
#endif

  //! Interaction manager - this handles panning and zooming around the map
  //! based on the active interaction mode
  //TSLInteractionModeManagerGeneric *m_modeManager;
  InteractionModeManager * m_modeManager;

  //! call back to update the GUI for reseting interaction modes.
  resetInteractionModesCallBack m_resetInteractionModesCallBack;

  //! call back to update the GUI for viewChanged modes.
  viewChangedCallBack m_viewChangedCallBack;

  //! set the call back to update the GUI for selected track info.
  selectedTrackCallBack m_selectedTrackCallBack;

  //! The size of the window the drawing surface is attached to
  int m_widgetWidth;
  int m_widgetHeight;

  //! Rotation of the drawing surface in radians
  double m_surfaceRotation;

  //! flag set if the window is not created
  bool m_tobeCreated;

  //! parent widget
  QWidget *m_parentWidget;

  /** data managers */
// object to manage the map and terrain data layers
  DatalayersManager m_datalayersManager;

  // object to manage the tracks
  TracksManager m_tracksManager;

  // geometry manager
  GeometryManager m_geometryManager;
};

#endif

