/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <QtGui>
#include <QMessageBox>
#include <sstream>

#include "application.h"

//! The name of our map layer. This is used when adding the data layer
//! to the drawing surface and used to reference the data layer from the 
//! drawing surface
const char * Application::m_mapLayerName = "map" ;

//! Controls how far the drawing surface rotates in one key press - this value is in radians
static const double rotationIncrement = M_PI / 360.0;

Application::Application(QWidget *parent) :
  m_mapDataLayer(NULL), 
  m_drawingSurface(NULL),
#ifndef WINNT
  m_display(NULL),
  m_drawable(0),
  m_screen(NULL),
  m_colourmap(0),
  m_visual(NULL),
#else
  m_window(NULL),
#endif
  m_modeManager(NULL),
  m_resetInteractionModesCallBack(NULL),
  m_viewChangedCallBack(NULL),
  m_selectedTrackCallBack(NULL),
  m_widgetWidth(1),
  m_widgetHeight(1),
  m_surfaceRotation(0.0),
  m_tobeCreated(true),
  m_parentWidget(parent)
{
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! Clear the error stack so that we can get the errors that occurred here.
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TSLErrorStack::clear( ) ;

  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! Initialise the drawing surface data files.
  //! This call uses the TSLUtilityFunctions::getMapLinkHome method to determine
  //! where MapLink is currently installed.  It then proceeds to load the
  //! following files : tsllinestyles.dat, tslfillstyles.dat, tslfonts.dat, tslsymbols.dat
  //! and tslcolours.dat
  //! When deploying your application, pass in a full path to the directory containing
  //! these files.
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TSLDrawingSurface::loadStandardConfig( );
  TSLCoordinateSystem::loadCoordinateSystems();

  //! Check for any errors that have occurred, and display them
  const char * msg = TSLErrorStack::errorString() ;
  if ( msg )
  {
    //! If we have any errors during initialisation, display the message
    //! and exit.
    QMessageBox::information(m_parentWidget, "Initialisation Error",
                              msg, QMessageBox::Cancel);
  }
}


Application::~Application()
{
  //! Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
  if (m_modeManager)
  {
    delete m_modeManager;
    m_modeManager = NULL;
  }

  while (m_drawingSurface->numTerrainDatabases() > 0) {
	  m_drawingSurface->removeTerrainDatabase(0u);
  }
  while (m_drawingSurface->getNumDataLayers() > 0) {
	  TSLDataLayer* layer = nullptr;
	  const char* layerName = nullptr;
	  m_drawingSurface->getDataLayerInfo(0, &layer, &layerName);
	  m_drawingSurface->removeDataLayer(layerName);
  }

  if (m_drawingSurface)
  {
	  delete m_drawingSurface;
	  m_drawingSurface = 0;
  }

  // Beyond this point MapLink will no longer be used - clear up all static data.
  // Once this is done no MapLink functions or classes can be used.
  TSLDrawingSurface::cleanup() ;
}

void Application::create() 
{
  if (!m_tobeCreated)
  {
    return ;  //! allready created all that we need
  }
  //! The first time ever, there will be no drawing surface.
  //! If we have no drawing surface, create it, otherwise remove the old layer.
  if ( !m_drawingSurface ) 
  {
    //! Create a double buffered drawing surface, and an interaction interface
    //! to control it.
    //! Set up the initial window extents.

#ifndef WINNT
	m_drawingSurface = new envitia::maplink::earth::Surface3D(m_display, m_drawable);
#else
	m_drawingSurface = new envitia::maplink::earth::Surface3D((TSLWindowHandle)m_window);
#endif

	// Set the size of the surface to match the window size
    m_drawingSurface->wndResize( 0, 0, m_widgetWidth, m_widgetHeight, false) ;
  }

  // Now create and initialse the mode manager and modes
  initializeModeManager();

  // Setup some basic styles for the created geometry
  GeometryManager::createStyles(m_drawingSurface);

  //! Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot initialise view\n" ) ;
  if (msg)
  {
    QMessageBox::critical(m_parentWidget, "Cannot initialise view", QString::fromUtf8( msg ) );
  }

  m_drawingSurface->lighting(true);
  m_drawingSurface->lightAmbient(TSLRGBA(20,60,60,255));
  m_drawingSurface->lightDiffuse(TSLRGBA(255,200,180,255));
  m_drawingSurface->lightPosition(earth::GeodeticPoint(45,0,800000));

  m_tobeCreated = false;
}

void Application::redraw()
{
  if( m_drawingSurface )
  {
      //! Draw the map to the widget
	  m_drawingSurface->redraw();

    //! Don't forget to draw any echo rectangle that may be active.
    //if (m_modeManager)
    //{
    //  m_modeManager->onDraw(0, 0, m_widgetWidth, m_widgetHeight);
    //}
  }
}

void Application::resize(int width, int height)
{
  if (m_drawingSurface)
  {
    //! Inform the drawing surface of the new window size,
    //! attempting to keep the top left corner the same.
    //! Do not ask for an automatic redraw since we will get a call to redraw() to do so
    m_drawingSurface->wndResize(0, 0, width, height, false);
  }
  //if (m_modeManager)
  //{
  //  m_modeManager->onSize(width, height);
  //}
  m_widgetWidth = width;
  m_widgetHeight = height;
}

bool Application::loadLayer(std::string filePath, std::string &err_msg)
{
	if (filePath.empty())
	{
		err_msg = "Empty layer path.";
		return false;
	}
	if (!m_drawingSurface)
	{
		err_msg = "Invalid null drawing surface.";
		return false;
	}

	// Store the layer
	TSLSimpleString dir;
	TSLSimpleString base;
	TSLSimpleString ext;
	TSLFileHelper::decomposePathname(filePath.c_str(), dir, base, ext);

	TSLSimpleString layerName = ext + "_" + base;
	DatalayersManager& dataManager = datalayersManager();
	if (ext == "map")
	{
		bool result = dataManager.loadMapLayer(filePath, layerName.c_str(), err_msg);
		if (result)
		{
			auto layerIndex = dataManager.getLayersSize() - 1;
			auto layer = dataManager.getLayer(layerIndex);
			if (!layer) return false;
			auto layerName = dataManager.getLayerName(layerIndex);

			// Add the layer to the active view
			if (!addDataLayer(layer, layerName))
			{
				err_msg = "Failed to add data layer to the surface";
				return false;
			}

			// reactive trackball interaction mode
			if (m_modeManager->getCurrentMode() == ID_TOOLS_Trackball)
			{
				m_modeManager->setCurrentMode(ID_TOOLS_Trackball, true);
			}
		}
		return result;
	}
	else if (ext == "tdf")
	{
		bool result = dataManager.loadTerrainDatabase(filePath, layerName.c_str(), err_msg);
		if (result)
		{
			auto index = dataManager.getTerrainDatabasesSize() - 1;
			auto database = dataManager.getTerrainDatabase(index);
			if (!database) return false;
			auto layerName = dataManager.getTerrainDatabaseName(index);

			// Add the layer to the view
			if (!addTerrainDatabase(*database, layerName))
			{
				err_msg = "Failed to add terrain database to the surface";
				return false;
			}

			// reactive trackball interaction mode
			if (m_modeManager->getCurrentMode() == ID_TOOLS_Trackball)
			{
				m_modeManager->setCurrentMode(ID_TOOLS_Trackball, true);
			}
		}
		return result;
	}

	// File type not recognised
	err_msg = "CEarthSampleDoc::OnOpenDocument: File type not recognised/handled by application";
	return false;
}

#ifdef WINNT
void Application::drawingInfo(WId window)
{
	m_window = window;
}

#else
void Application::drawingInfo(Drawable drawable, Display *display, Screen *screen, Colormap colourmap, Visual *visual)
{
	m_display = display;
	m_drawable = drawable;
	m_screen = screen;
	m_colourmap = colourmap;
	m_visual = visual;
}
#endif

//! set the call back to update the GUI for reseting interaction modes.
void Application::ResetInteractionModesCallBack(resetInteractionModesCallBack func)
{
	m_resetInteractionModesCallBack = func;
}
void Application::ViewChangedCallBack(viewChangedCallBack func)
{
	m_viewChangedCallBack = func;
}
void Application::SelectedTrackCallBack(selectedTrackCallBack func)
{
	m_selectedTrackCallBack = func;
}

/******** drawing surface functions ********/
bool Application::addDataLayer(TSLDataLayer* layer, std::string layerName) {
	if (!m_drawingSurface) return false;
	return m_drawingSurface->addDataLayer(layer, layerName.c_str());
}

bool Application::addTerrainDatabase(TSLTerrainDatabase& database, std::string databaseName) {
	if (!m_drawingSurface) return false;
	return m_drawingSurface->addTerrainDatabase(database);
}

bool Application::addTracks(std::vector<earth::Track>& tracks) {
	if (!m_drawingSurface) return false;
	if (tracks.empty()) return false;

	for (auto& t : tracks) {
		m_drawingSurface->addTrack(t);
	}
	m_drawingSurface->redraw();
	return true;
}

bool Application::addGeometry(std::vector<std::unique_ptr<earth::geometry::Geometry>>& geom) {
	if (!m_drawingSurface) return false;
	if (geom.empty()) return false;

	for (auto& g : geom) {
		m_drawingSurface->addGeometry(*g);
	}
	return true;
}

bool Application::drawToHDC(TSLDeviceContext& hdc, TSLDeviceUnits width, TSLDeviceUnits height)
{
	return m_drawingSurface->drawToHDC(hdc, width, height);
}

//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Sets the Map background colour by querying the Map Datalayer
//! for the colour and either clearing the draw surface background
//! colour or setting it to the colour specified in the datalayer.
//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::setMapBackgroundColour()
{
	//! Set the Map background colour.
	//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//! Query the colour from tha Map datalayer
	int backgroundColour = m_mapDataLayer->getBackgroundColour();

	//! If there is no colour then clear the colour set in the
	//! drawing surface or we will keep the old colour (the
	//! default colour is white).
	//
	//! If there is a colour set it.
	//
	//! If we have multiple map data layers attached to the drawing
	//! surface we would need to decide at application level
	//! what colour to use.
	//
	//! When we originally attach a datalayer the drawing surface
	//! sets the background colour using the colour in the datalayer
	//! however on subsequent load's the background colour is not
	//! read, as there is a knock on effect depending on which
	//! drawing surfaces a layer is attached to and the order
	//! and number of other attached layers.
	//
	if (backgroundColour == -1)
	{
		m_drawingSurface->clearBackgroundColour();
	}
	else
	{
		m_drawingSurface->setBackgroundColour(backgroundColour);
	}
}

void Application::startTracks(int numTracks)
{
	m_tracksManager.initialiseTracks(numTracks);

	// Add the tracks to the view
	auto& tracks = m_tracksManager.getTracks();
	addTracks(tracks);
}

void Application::removeTracks()
{
	// Clear the tracks from the surface
	m_drawingSurface->removeAllTracks();
	// Now we can delete them
	m_tracksManager.removeTracks();
	m_drawingSurface->redraw();
}
/******** Interaction modes ********/
void Application::initializeModeManager()
{
	if (m_modeManager == NULL)
	{
		m_modeManager = new InteractionModeManager(this, m_drawingSurface, 30, true);
		m_modeManager->addMode(new SelectInteraction(ID_TOOLS_Select), false);
		m_modeManager->addMode(new CreatePolygonInteraction(ID_TOOLS_CreatePolygon, "polygon"), false);
		m_modeManager->addMode(new CreatePolylineInteraction(ID_TOOLS_CreatePolyline, "polyline"), false);
		m_modeManager->addMode(new CreateTextInteraction(ID_TOOLS_CreateText), false);
		m_modeManager->addMode(new CreateSymbolInteraction(ID_TOOLS_CreateSymbol), false);
		m_modeManager->addMode(new CreatePolygonInteraction(ID_TOOLS_CreateExtrudedPolygon, "extruded-polygon"), false);
		m_modeManager->addMode(new CreatePolylineInteraction(ID_TOOLS_CreateExtrudedPolyline, "extruded-polyline"), false);
		m_modeManager->addMode(new DeleteGeometryInteraction(ID_TOOLS_DeleteGeometry), false);
		m_modeManager->addMode(new TrackballViewInteraction(ID_TOOLS_Trackball), true);
		m_modeManager->setCurrentMode(ID_TOOLS_Trackball, true);
	}
}

int Application::currentInteractionMode()
{
	if (m_modeManager)
		return m_modeManager->getCurrentMode();

	return ID_TOOLS_NoInteraction;
}

void Application::interaction(int i, bool reactivate) {
	if (m_modeManager)
		m_modeManager->setCurrentMode(i, reactivate);
}

bool Application::mouseMoveEvent(unsigned int buttonPressed, bool shiftPressed, bool controlPressed, int mx, int my)
{
	// add point lat/lon to status bar
#if 0
	double lat, lon;
	std::stringstream str;
	if (!m_drawingSurface->DUToLatLong(mx, my, &lat, &lon)) {
		str << "Mouse: " << mx << "," << my;
	}
	else {
		str << "Mouse: " << mx << "," << my << " : ENU:" << lon << "," << lat;
		double x, y, z;
		if (m_drawingSurface->geodeticToGeocentric(lat, lon, 0, x, y, z)) {
			str << ": ECEF: " << x << "," << y << "," << z;
		}
	}
	//wnd->setStatusText(str.str());
#endif

  //! If the user is in the middle of an interaction, pass the event onto the handler
  if (m_modeManager)
  {
    //! Request a redraw if the interaction hander requires it
    return m_modeManager->onMouseMove((TSLButtonType)buttonPressed, mx, my, shiftPressed, controlPressed);
  }
  return false;
}

bool Application::OnLButtonDown(bool shiftPressed, bool controlPressed, int X, int Y)
{
  //! If the user is in the middle of an interaction, pass the event onto the handler
  if (m_modeManager)
  {
    //! Request a redraw if the interaction hander requires it
    return m_modeManager->onLButtonDown(X, Y, shiftPressed, controlPressed);
  }
  return false;
}

bool Application::OnMButtonDown(bool shiftPressed, bool controlPressed, int X, int Y)
{
  //! If the user is in the middle of an interaction, pass the event onto the handler
  if (m_modeManager)
  {
    //! Request a redraw if the interaction hander requires it
    return m_modeManager->onMButtonDown(X, Y, shiftPressed, controlPressed);
  }
  return false;
}

bool Application::OnRButtonDown(bool shiftPressed, bool controlPressed, int mx, int my)
{
  //! If the user is in the middle of an interaction, pass the event onto the handler
  if (m_modeManager)
  {
    //! Request a redraw if the interaction hander requires it
    return m_modeManager->onRButtonDown(mx, my, shiftPressed, controlPressed);
  }
  return false;
}

bool Application::OnLButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  //! If the user is in the middle of an interaction, pass the event onto the handler
  if (m_modeManager)
  {
    //! Request a redraw if the interaction hander requires it
    return m_modeManager->onLButtonUp(mx, my, shiftPressed, controlPressed);
  }
  return false;
}

bool Application::OnMButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  //! If the user is in the middle of an interaction, pass the event onto the handler
  if (m_modeManager)
  {
    //! Request a redraw if the interaction hander requires it
    return m_modeManager->onMButtonUp(mx, my, shiftPressed, controlPressed);
  }
  return true;
}

bool Application::OnRButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
  //! If the user is in the middle of an interaction, pass the event onto the handler
  if (m_modeManager)
  {
    //! Request a redraw if the interaction hander requires it
    return m_modeManager->onRButtonUp(mx, my, shiftPressed, controlPressed);
  }
  return true;
}

bool Application::OnKeyPress(bool, bool, int keySym)
{
  //! The left and right arrow keys allow the drawing surface to be rotated
  switch (keySym)
  {
  case Qt::Key_Left:
    m_surfaceRotation += rotationIncrement;
//    m_drawingSurface->rotate(m_surfaceRotation);
    return true;

  case Qt::Key_Right:
    m_surfaceRotation -= rotationIncrement;
//    m_drawingSurface->rotate(m_surfaceRotation);
    return true;

  default:
    break;
  }
  return false;
}

bool Application::OnMouseWheel(bool, bool, short zDelta, int X, int Y)
{
  if (m_modeManager)
  {
    //! Request a redraw if the interaction hander requires it
    return m_modeManager->onMouseWheel(zDelta, X, Y);
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////
//! TSLInteractionModeRequest callback functions
///////////////////////////////////////////////////////////////////////////
void Application::resetMode(Interaction * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
	//! call back when reset interaction modes
	if (m_resetInteractionModesCallBack)
	{
		m_resetInteractionModesCallBack();
	}
}

void Application::viewChanged(envitia::maplink::earth::Surface3D* drawingSurface)
{
	if (m_viewChangedCallBack)
	{
		m_viewChangedCallBack();
	}

  auto camPos = m_drawingSurface->camera().position();
  camPos.x(camPos.x()+45);
  camPos.z(8000000);
  m_drawingSurface->lightPosition(camPos);
}

void Application::geometryAdded(earth::geometry::Geometry* geom)
{
	m_geometryManager.addGeometry(geom);
	m_drawingSurface->redraw();
}

void Application::geometryRemoved(earth::geometry::Geometry* geom)
{
	m_geometryManager.removeGeometry(*geom);
	m_drawingSurface->redraw();
}

void Application::geometrySelected(earth::geometry::Geometry* geom)
{
	m_geometryManager.switchGeometryStyle(geom);
	m_drawingSurface->redraw();
}

void Application::trackSelected(earth::Track* track)
{
	if (!track) return;

	// unselect track in the gui dialog
	if (m_selectedTrackCallBack)
	{
		m_selectedTrackCallBack(nullptr, 0);
	}

	// select track in the drawing surface by changing its style
	bool isSelected = m_tracksManager.selectTrack(track);

	// select track in the gui dialog
	if (isSelected && m_selectedTrackCallBack)
	{
		m_selectedTrackCallBack(track, m_tracksManager.getLastPickedTrack());
	}
}

TSLEnvelope Application::GetWindowEnvelope()
{
	return TSLEnvelope(0, 0, m_widgetWidth, m_widgetHeight);
}

///////////////////////////////////////////////////////////////////////////
//! Event handler functions - these are invoked from the widget
///////////////////////////////////////////////////////////////////////////

void Application::activate_Trackball_Mode()
{
	interaction(ID_TOOLS_Trackball);
}
void Application::activate_Select_Mode()
{
	interaction(ID_TOOLS_Select);
}
void Application::activate_CreatePolygon_Mode()
{
	interaction(ID_TOOLS_CreatePolygon);
}
void Application::activate_CreatePolyline_Mode()
{
	interaction(ID_TOOLS_CreatePolyline);
}
void Application::activate_CreateText_Mode()
{
	interaction(ID_TOOLS_CreateText);
}
void Application::activate_CreateSymbol_Mode()
{
	interaction(ID_TOOLS_CreateSymbol);
}
void Application::activate_CreateExtrudedPolygon_Mode()
{
	interaction(ID_TOOLS_CreateExtrudedPolygon);
}
void Application::activate_CreateExtrudedPolyline_Mode()
{
	interaction(ID_TOOLS_CreateExtrudedPolyline);
}
void Application::activate_DeleteGeometry_Mode()
{
	interaction(ID_TOOLS_DeleteGeometry);
}
bool Application::zoomIn()
{
  //if (m_modeManager)
  //{
  //  //! Request a redraw if the interaction hander requires it
  //  return m_modeManager->zoomIn(30);
  //}
  return false;
}

bool Application::zoomOut()
{
  //if (m_modeManager)
  //{
  //  //! Request a redraw if the interaction hander requires it
  //  return m_modeManager->zoomOut(30);
  //}
  return false;
}

void Application::resetView()
{
  //! Reset the view to the full extent of the map being loaded
  if (m_drawingSurface)
  {
    //! Reset the drawing surface rotation as well
    m_surfaceRotation = 0.0;
//    m_drawingSurface->rotate(m_surfaceRotation);

    m_drawingSurface->reset(false);
  }
}

/******** properties control ********/
// object to manage the map and terrain data layers
DatalayersManager& Application::datalayersManager()
{
	return m_datalayersManager;
}

// object to manage the tracks
TracksManager& Application::tracksManager()
{
	return m_tracksManager;
}

// geometry manager
GeometryManager& Application::geometryManager()
{
	return m_geometryManager;
}
