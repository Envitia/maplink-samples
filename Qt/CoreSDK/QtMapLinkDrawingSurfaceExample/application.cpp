/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <QtGui>
#include <QMessageBox>

#ifdef WIN32
# include <windows.h>
#endif

#include "MapLink.h"
#include "application.h"

// The name of our map layer. This is used when adding the data layer
// to the drawing surface and used to reference the data layer from the 
// drawing surface
const char * Application::m_mapLayerName = "map" ;

Application::Application() : 
  m_mapDataLayer(NULL), 
  m_stdDataLayer(NULL),
  m_overlayType(ID_OVERLAYS_NONE),
  m_mapLoaded(false),
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
  m_width(1),
  m_height(1),
  m_initialUpdate(true),
  m_grabbed(false),
  m_checkForGrab(false)
{
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Clear the error stack so that we can get the errors that occurred here.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TSLErrorStack::clear( ) ;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Initialise the drawing surface data files.
  // This call uses the TSLUtilityFunctions::getMapLinkHome method to determine
  // where MapLink is currently installed.  It then proceeds to load the
  // following files : tsllinestyles.dat, tslfillstyles.dat, tslfonts.dat, tslsymbols.dat
  // and tslcolours.dat
  // When deploying your application, pass in a full path to the directory containing
  // these files.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TSLDrawingSurface::loadStandardConfig( );

  // Check for any errors that have occurred, and display them
  const char * msg = TSLErrorStack::errorString() ;
  if ( msg )
  {
    // If we have any errors during initialisation, display the message
    // and exit.
    QMessageBox::information( NULL, "Initialisation Error",
                              msg, QMessageBox::Cancel);
  }
}


Application::~Application()
{
  // Clean up by destroying the map and pathlist
  if ( m_mapDataLayer )
  {
    m_mapDataLayer->destroy() ;
    m_mapDataLayer = 0 ;
  }
  if ( m_stdDataLayer )
  {
    m_stdDataLayer->destroy();
    m_stdDataLayer = 0;
  }
  if (m_drawingSurface)
  {
    delete m_drawingSurface;
    m_drawingSurface = 0;
  }
  TSLDrawingSurface::cleanup() ;
}


bool Application::loadMap(const char *mapFilename)
{
  // Clear the error stack, load the map then check for errors.
  TSLErrorStack::clear() ;

  // Create a map data layer
  if (m_mapDataLayer == NULL)
  {
    m_mapDataLayer = new TSLMapDataLayer();

    // add the datalayer to the drawing surface
    // Note: any number of datalayers can be added to a surface (each has it's own name)
    m_drawingSurface->addDataLayer( m_mapDataLayer, m_mapLayerName ) ;
  }
  if (m_stdDataLayer == NULL)
  {
    m_stdDataLayer = new TSLStandardDataLayer();
    TSLStyleID black = TSLComposeRGB( 0, 0, 0 ) ;

    // Make up a feature name and numeric ID
    m_stdDataLayer->addFeatureRendering( "Airport", 123 ) ;

    // Associate some rendering with the new feature, use ID for efficiency
    m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolStyle, 6003 ) ;
    m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolColour, black ) ;
    m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolSizeFactor, 40.0 ) ;
    m_stdDataLayer->setFeatureRendering( 0, 123, TSLRenderingAttributeSymbolSizeFactorUnits, TSLDimensionUnitsPixels ) ;

    // overlay layer - mainly static data - order of adding is important
    m_drawingSurface->addDataLayer( m_stdDataLayer, "overlay");
    m_drawingSurface->bringToFront( "overlay" );
  }
  
  // load the map
  if ( !m_mapDataLayer->loadData( mapFilename ) )
  {
    QString messageBody( "Could not load map " + QString::fromUtf8( mapFilename ) );
    QMessageBox::information( NULL, "Could not load map",
                              messageBody, QMessageBox::Cancel );
    return false;
  }

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot load map\n" ) ;
  if ( msg )
  {
    QMessageBox::information( NULL, "Could not load map",
                              QString::fromUtf8(msg), QMessageBox::Cancel);
    return false;
  }

  return true;
}

void Application::OnInitialUpdate() 
{
  if (!m_initialUpdate)
  {
    return ;  // allready created all that we need
  }
  // The first time ever, there will be no drawing surface.
  // If we have no drawing surface, create it, otherwise remove the old layer.
  if ( !m_drawingSurface ) 
  {
    // Create a double buffered drawing surface, and an interaction interface
    // to control it.
    // Set up the initial window extents.

#ifndef WINNT
    m_drawingSurface = new TSLMotifSurface( m_display, m_screen, m_colourmap, m_drawable, 0, m_visual);
#else
    m_drawingSurface = new TSLNTSurface( (void *)m_window, false);
#endif

    m_drawingSurface->setOption( TSLOptionDoubleBuffered, true ) ;
    m_drawingSurface->setOption( TSLOptionDynamicArcSupportEnabled, true ) ; // could do this based on map
    m_drawingSurface->wndResize( 0, 0, m_width, m_height, false, TSLResizeActionMaintainTopLeft ) ;
  }

  // and reset the current view to display the entire map.
  m_drawingSurface->reset( ) ;

  // Display any errors that have occurred
  const char * msg = TSLErrorStack::errorString( "Cannot initialise view\n" ) ;
  if (msg)
  {
    QMessageBox::critical( NULL, "Cannot initialise view", QString::fromUtf8( msg ) );
  }

  m_initialUpdate = false;
}

void Application::reset()
{
  if (m_drawingSurface)
  {
    m_drawingSurface->reset( ) ;
  }
}

void Application::OnDraw(TSLEnvelope *envelope)
{
  if ( m_drawingSurface )
  {
    TSLDeviceUnits x1, y1, x2, y2 ;
    if (envelope == NULL)
    {
      m_drawingSurface->getDUExtent( &x1, &y1, &x2, &y2 ) ;
    }
    else
    {
      // extract overall envelope
      x1 = envelope->xMin();
      y1 = envelope->yMin();
      x2 = envelope->xMax();
      y2 = envelope->yMax();
    }
 
    m_drawingSurface->drawDU( x1, y1, x2, y2, true ) ;
  }
  const char * msg = TSLErrorStack::errorString() ;
  if ( msg )
  {
    // If we have any errors during initialisation, display the message
    // and exit.
    printf( msg ) ;
  }
}

void Application::redraw()
{
  if( m_drawingSurface )
  {
    m_drawingSurface->redraw();
  }
}

void Application::OnSize(int cx, int cy)
{
  m_height = cy;
  m_width = cx;

  if ( m_drawingSurface )
  {
    // Inform the drawing surface of the new window size,
    // attempting to keep the top left corner the same.
    // Do not ask for an automatic redraw since we will get an OnDraw call
    m_drawingSurface->wndResize( 0, 0, cx, cy, false, TSLResizeActionMaintainTopLeft ) ;
  }
}

////////////////////////////////////////////////////////////////////////
bool Application::OnMouseMove(unsigned int, bool , bool, int mx, int my)
{
  if (   m_checkForGrab 
      && ( m_grabbed || abs( mx - m_rmb.m_x ) > 3 || abs( my - m_rmb.m_y ) > 3 ) )
  {

    // Calculate offset between the last point and the new point
    int du_offset_x = m_lastGrabPoint.m_x - mx ;
    int du_offset_y = my - m_lastGrabPoint.m_y ;

    // Indicate a grab and remember the last grabbed point
    m_lastGrabPoint = Point(mx, my) ;
    m_grabbed = true ;

    // Convert the offset into user units
    double tmcPerDUX, tmcPerDUY;
    m_drawingSurface->TMCperDU( tmcPerDUX, tmcPerDUY );
    double uu_per_du_x = tmcPerDUX / m_drawingSurface->TMCperUU() ;
    double uu_per_du_y = tmcPerDUY / m_drawingSurface->TMCperUU() ;

    double uu_offset_x = du_offset_x * uu_per_du_x;
    double uu_offset_y = du_offset_y * uu_per_du_y;

    // Get the current centre point of the drawing surface
    double x, y, x1, y1, x2, y2 ;

    m_drawingSurface->getUUExtent( &x1, &y1, &x2, &y2 ) ;
    x = ( x1 + x2 ) / 2 ;
    y = ( y1 + y2 ) / 2 ;
  
    x += uu_offset_x ;
    y += uu_offset_y ;

    // Pan and redraw the drawing surface
    // Request redraw only if pan is successful
    if ( m_drawingSurface->pan( x, y, false ) )
    {
      return true;
    }
  }
  return false;
}

bool Application::OnLButtonDown(bool, bool, int X, int Y)
{
  m_lmb.m_x = X;
  m_lmb.m_y = Y;
  return false;
}

bool Application::OnMButtonDown(bool, bool, int, int)
{
  return false;
}

bool Application::OnRButtonDown(bool, bool, int mx, int my) 
{
  Point point(mx, my);
  m_rmb = m_lastGrabPoint = point ;
  m_grabbed = false ;
  m_checkForGrab = true ;
  return false;
}

bool Application::OnLButtonUp(bool, bool controlPressed, int mx, int my)
{
  if ( m_drawingSurface )
  {
    if ( controlPressed )
    {
      TSLTMC x, y ;
      m_drawingSurface->DUToTMC( mx, my, &x, &y ) ;

      if ( createOverlay( x, y, m_drawingSurface  ) )
      {
        // Request a redraw
        return true;
      }
    }
    else if ( abs( mx - m_lmb.m_x ) <= 3 && abs( my - m_lmb.m_y ) <= 3 )
    { 
      // Pan to point and zoom
      double x, y ;
      if ( m_drawingSurface->DUToUU( mx, my, &x, &y ) )
      {
        if ( m_drawingSurface->pan( x, y, false ) )
        {
          if ( m_drawingSurface->zoom( 25, true, false ) )
          {
            // Request a redraw
            return true;
          }
        }
      }
    }
    else
    {
      // Zoom to rectangle
      double x1, y1, x2, y2 ;
      if (   m_drawingSurface->DUToUU( mx, my, &x1, &y1 )
          && m_drawingSurface->DUToUU( m_lmb.m_x, m_lmb.m_y, &x2, &y2 ) )
      {
        if ( m_drawingSurface->resize( x1, y1, x2, y2, false, true ) )
        {
          // Request a redraw
          return true;
        }
      }
    }
  }
  return false;
}

bool Application::OnMButtonUp(bool, bool, int mx, int my)
{
  if ( m_drawingSurface )
  {
    double x, y ;
    if ( m_drawingSurface->DUToUU( mx, my, &x, &y ) )
    {
      if ( m_drawingSurface->pan( x, y, false ) )
      {
        // Request a redraw
      }
    }
  }
  return true;
}

bool Application::OnRButtonUp(bool, bool, int, int)
{
  if ( m_drawingSurface && !m_grabbed )
  {
    if ( m_drawingSurface->zoom(25, false) )
    {
      // Request a redraw
    }
  }
  m_grabbed = m_checkForGrab = false;
  return true;
}

bool Application::OnKeyPress(bool, bool, int keySym)
{
  switch (toupper(keySym))
  {
    case 'N':
      m_overlayType = ID_OVERLAYS_NONE;
      break;
    case 'L':
      m_overlayType = ID_OVERLAYS_LINE;
      break;
    case 'P':
      m_overlayType = ID_OVERLAYS_POLYGON;
      break;
    case 'T':
      m_overlayType = ID_OVERLAYS_TEXT;
      break;
    case 'S':
      m_overlayType = ID_OVERLAYS_SYMBOL;
      break;
    case 'F':
      m_overlayType = ID_OVERLAYS_FEATURE;
      break;
  }
  return false;
}

bool Application::OnMouseWheel(bool, bool, short zDelta, int, int)
{
  if (m_drawingSurface && m_drawingSurface->zoom(30.0, zDelta>0, false))
  {
    // Request a redraw
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Sets the Map background colour by querying the Map Datalayer
// for the colour and either clearing the draw surface background
// colour or setting it to the colour specified in the datalayer.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::setMapBackgroundColour()
{
  // Set the Map background colour.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Query the colour from tha Map datalayer
  int backgroundColour = m_mapDataLayer->getBackgroundColour();

  // If there is no colour then clear the colour set in the
  // drawing surface or we will keep the old colour (the
  // default colour is white).
  //
  // If there is a colour set it.
  //
  // If we have multiple map data layers attached to the drawing
  // surface we would need to decide at application level
  // what colour to use.
  //
  // When we originally attach a datalayer the drawing surface
  // sets the background colour using the colour in the datalayer
  // however on subsequent load's the background colour is not
  // read, as there is a knock on effect depending on which
  // drawing surfaces a layer is attached to and the order
  // and number of other attached layers.
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


//////////////////////////////////////////////////////////
bool Application::createOverlay( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds )
{
  switch ( m_overlayType )
  {
    case ID_OVERLAYS_LINE :    return createPolyline( x, y, ds ) ;
    case ID_OVERLAYS_POLYGON : return createPolygon( x, y, ds ) ;
    case ID_OVERLAYS_TEXT :    return createText( x, y, ds ) ;
    case ID_OVERLAYS_SYMBOL :  return createSymbol( x, y, ds ) ;
    case ID_OVERLAYS_FEATURE : return createFeature( x, y, ds ) ;
  }
  return false ;
}

bool Application::createText( TSLTMC x, TSLTMC y, TSLDrawingSurface * )
{
  TSLEntitySet * es = m_stdDataLayer->entitySet() ;
  TSLText * txt = es->createText( 0, x, y, "Hello World" ) ;
  if ( !txt )
    return false ;

  int black = TSLComposeRGB( 0, 0, 0 ) ;

  // Set the rendering of the text to be Vector Text 25 pixels high
  txt->setRendering( TSLRenderingAttributeTextFont, 56 ) ;
  txt->setRendering( TSLRenderingAttributeTextColour, black ) ;
  txt->setRendering( TSLRenderingAttributeTextSizeFactor, 25.0 ) ;
  txt->setRendering( TSLRenderingAttributeTextSizeFactorUnits,
                     TSLDimensionUnitsPixels ) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;

  return true ;
}

bool Application::createPolygon( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds )
{
  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  // Create a coordinate list forming a triangle around the position
  // Use the Drawing Surafce to calculate the coordinates
  // We will make our triangle 25 pixels either side of the position
  // Note that the pixels are at the current zoom factor - the polygon
  // is always completely scalable
  TSLCoordSet * coords = new TSLCoordSet() ;
  if ( !coords )
    return false ;

  double tmcPerDUX, tmcPerDUY;
  ds->TMCperDU( tmcPerDUX, tmcPerDUY ) ;
  coords->add( x - 25 * tmcPerDUX, y - 25 * tmcPerDUY ) ;
  coords->add( x + 25 * tmcPerDUX, y - 25 * tmcPerDUY ) ;
  coords->add( x,                 y + 25 * tmcPerDUY ) ;

  // Hand ownership of the coordset to the polygon
  TSLPolygon * poly = es->createPolygon( 0, coords, true ) ;
  if ( !poly )
    return false ;

  TSLStyleID yellow = TSLComposeRGB( 255, 255, 0 ) ;
  TSLStyleID black = TSLComposeRGB( 0, 0, 0 ) ;
  poly->setRendering( TSLRenderingAttributeFillStyle, 1 ) ;
  poly->setRendering( TSLRenderingAttributeFillColour, yellow ) ;
  poly->setRendering( TSLRenderingAttributeEdgeStyle, 1 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeColour, black ) ;
  poly->setRendering( TSLRenderingAttributeEdgeThickness, 1.0 ) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;

  return true ;
}

bool Application::createPolyline( TSLTMC x, TSLTMC y, TSLDrawingSurface * ds )
{
  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  // Create a coordinate list forming a triangle around the position
  // Use the Drawing Surafce to calculate the coordinates
  TSLCoordSet * coords = new TSLCoordSet() ;
  if ( !coords )
    return false ;

  // Make a triangle, 1km either side of the specified position
  // This assumes that 1 Map Unit == 1 Metre in the display coordinate system
  double tmcPerMU = ds->TMCperMU() ;
  coords->add( x - 1000 * tmcPerMU, y + 1000 * tmcPerMU ) ;
  coords->add( x,                   y - 1000 * tmcPerMU ) ;
  coords->add( x + 1000 * tmcPerMU, y + 1000 * tmcPerMU ) ;

  // Hand ownership of the coordset to the polygon
  TSLPolyline * poly = es->createPolyline( 0, coords, true ) ;
  if ( !poly )
    return false ;

  // Use MapUnit thickness so the line thickness scales as we zoom in/out
  // Set it to 20m. Complex line styles - like style 48 have thickness
  // clamping applied automatically to avoid performance
  // or aesthetic problems
  TSLStyleID yellow = TSLComposeRGB( 255, 255, 0 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeStyle, 48 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeColour, yellow ) ;
  poly->setRendering( TSLRenderingAttributeEdgeThickness, 20.0 ) ;
  poly->setRendering( TSLRenderingAttributeEdgeThicknessUnits,
                      TSLDimensionUnitsMapUnits) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;

  return true ;
}

bool Application::createSymbol( TSLTMC x, TSLTMC y, TSLDrawingSurface * )
{
  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  TSLSymbol * symbol = es->createSymbol( 0, x, y ) ;
  if ( !symbol )
    return false ;

  // Create a green star, 1000m high. This looks sensible on the Dorset map!
  TSLStyleID green = TSLComposeRGB( 0, 255, 0 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolStyle, 14 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolColour, green ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactor, 1000.0 ) ;
  symbol->setRendering( TSLRenderingAttributeSymbolSizeFactorUnits,
                        TSLDimensionUnitsMapUnits ) ;

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;

  return true ;
}

bool Application::createFeature( TSLTMC x, TSLTMC y, TSLDrawingSurface * )
{
  TSLEntitySet * es = m_stdDataLayer->entitySet() ;

  // 123 is the numeric feature code we assigned on the Data Layer
  TSLSymbol * symbol = es->createSymbol( 123, x, y ) ;

  if ( !symbol )
    return false ;

  // No need to configure any rendering, MapLink will look it up
  // from the Data Layer at display time.

  // Tell the layer that its contents have changed
  m_stdDataLayer->notifyChanged( true ) ;

  return true ;
}


