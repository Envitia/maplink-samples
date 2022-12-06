/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <QtGui>
#include <QMessageBox>

#include "application.h"

//! Interaction mode IDs - these can be any numbers
#define ID_TOOLS_ZOOM                   1
#define ID_TOOLS_PAN                    2
#define ID_TOOLS_GRAB                   3

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
  if (m_drawingSurface)
  {
    delete m_drawingSurface;
    m_drawingSurface = 0;
  }

  // Beyond this point MapLink will no longer be used - clear up all static data.
  // Once this is done no MapLink functions or classes can be used.
  TSLDrawingSurface::cleanup() ;
}


bool Application::loadMap(const char *mapFilename)
{
  //! Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  if (mapFilename == NULL)
  {
    return false;
  }
  
  //! load the map
  if (!m_mapDataLayer->loadData(mapFilename))
  {
    QString messageBody("Could not load map " + QString::fromUtf8(mapFilename));
    QMessageBox::information(m_parentWidget, "Could not load map",
      messageBody, QMessageBox::Cancel);
    return false;
  }
  setMapBackgroundColour();
  
  
  if (m_modeManager)
  {
    //! Loading a map invalidates any stored views in mode manager  - this sample
    //! doesn't create any
    m_modeManager->resetViews();
  }

  //! Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    QMessageBox::information(m_parentWidget, "Could not load map",
                              QString::fromUtf8(msg), QMessageBox::Cancel);
    return false;
  }

  return true;
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
    m_drawingSurface = new TSLMotifSurface( m_display, m_screen, m_colourmap, m_drawable, 0, m_visual);
#else
    m_drawingSurface = new TSLNTSurface( (void *)m_window, false);
#endif

    m_drawingSurface->setOption( TSLOptionDoubleBuffered, true ) ;
    m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true ) ; //! could do this based on map
    m_drawingSurface->wndResize( 0, 0, m_widgetWidth, m_widgetHeight, false, TSLResizeActionMaintainTopLeft ) ;
  }

  //! Create a map data layer
  if (m_mapDataLayer == NULL)
  {
    m_mapDataLayer = new TSLMapDataLayer();

    //! add the datalayer to the drawing surface
    //! Note: any number of datalayers can be added to a surface (each has it's own name)
    m_drawingSurface->addDataLayer(m_mapDataLayer, m_mapLayerName);
  }

  //
  if (m_modeManager == NULL)
  {
    //! Now create and initialse the mode manager and modes
    m_modeManager = new TSLInteractionModeManagerGeneric(this, m_drawingSurface);

    //! Add the three interaction mode types to the manager - the zoom mode is the default
    m_modeManager->addMode(new TSLInteractionModeZoom(ID_TOOLS_ZOOM), true);
    m_modeManager->addMode(new TSLInteractionModePan(ID_TOOLS_PAN), false);
    m_modeManager->addMode(new TSLInteractionModeGrab(ID_TOOLS_GRAB), false);

    m_modeManager->setCurrentMode(ID_TOOLS_ZOOM);
  }

  //! and reset the current view to display the entire map.
  m_drawingSurface->reset( ) ;

  //! Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot initialise view\n" ) ;
  if (msg)
  {
    QMessageBox::critical(m_parentWidget, "Cannot initialise view", QString::fromUtf8( msg ) );
  }

  m_tobeCreated = false;
}

void Application::redraw()
{
  if( m_drawingSurface )
  {
    //! Draw the map to the widget
    m_drawingSurface->drawDU(0, 0, m_widgetWidth, m_widgetHeight, true);

    //! Don't forget to draw any echo rectangle that may be active.
    if (m_modeManager)
    {
      m_modeManager->onDraw(0, 0, m_widgetWidth, m_widgetHeight);
    }
  }
}

void Application::resize(int width, int height)
{
  if (m_drawingSurface)
  {
    //! Inform the drawing surface of the new window size,
    //! attempting to keep the top left corner the same.
    //! Do not ask for an automatic redraw since we will get a call to redraw() to do so
    m_drawingSurface->wndResize(0, 0, width, height, false, TSLResizeActionMaintainTopLeft);
  }
  if (m_modeManager)
  {
    m_modeManager->onSize(width, height);
  }
  m_widgetWidth = width;
  m_widgetHeight = height;
}

////////////////////////////////////////////////////////////////////////
bool Application::mouseMoveEvent(unsigned int buttonPressed, bool shiftPressed, bool controlPressed, int mx, int my)
{
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
    m_drawingSurface->rotate(m_surfaceRotation);
    return true;

  case Qt::Key_Right:
    m_surfaceRotation -= rotationIncrement;
    m_drawingSurface->rotate(m_surfaceRotation);
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

///////////////////////////////////////////////////////////////////////////
//! Event handler functions - these are invoked from the widget
///////////////////////////////////////////////////////////////////////////

void Application::activatePanMode()
{
  //! Activate the pan interaction mode
  if (m_modeManager)
  {
    m_modeManager->setCurrentMode(ID_TOOLS_PAN);
  }
}

void Application::activateZoomMode()
{
  //! Activate the zoom interaction mode
  if (m_modeManager)
  {
    m_modeManager->setCurrentMode(ID_TOOLS_ZOOM);
  }
}

void Application::activateGrabMode()
{
  //! Activate the grab interaction mode
  if (m_modeManager)
  {
    m_modeManager->setCurrentMode(ID_TOOLS_GRAB);
  }
}

bool Application::zoomIn()
{
  if (m_modeManager)
  {
    //! Request a redraw if the interaction hander requires it
    return m_modeManager->zoomIn(30);
  }
  return false;
}

bool Application::zoomOut()
{
  if (m_modeManager)
  {
    //! Request a redraw if the interaction hander requires it
    return m_modeManager->zoomOut(30);
  }
  return false;
}

void Application::resetView()
{
  //! Reset the view to the full extent of the map being loaded
  if (m_drawingSurface)
  {
    //! Reset the drawing surface rotation as well
    m_surfaceRotation = 0.0;
    m_drawingSurface->rotate(m_surfaceRotation);

    m_drawingSurface->reset(false);
  }
}

///////////////////////////////////////////////////////////////////////////
//! TSLInteractionModeRequest callback functions
///////////////////////////////////////////////////////////////////////////
void Application::resetMode(TSLInteractionMode *, TSLButtonType, TSLDeviceUnits, TSLDeviceUnits)
{
  //! call back when reset interaction modes
  if (m_resetInteractionModesCallBack)
  {
    m_resetInteractionModesCallBack();
  }
}

void Application::viewChanged(TSLDrawingSurface*)
{
  //! Do nothing
}

//! set the call back to update the GUI for reseting interaction modes.
void Application::ResetInteractionModesCallBack(resetInteractionModesCallBack func)
{
  m_resetInteractionModesCallBack = func;
}