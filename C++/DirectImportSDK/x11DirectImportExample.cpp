/****************************************************************************
        Copyright (c) 2023 by Envitia Group PLC.
=============================================================================
MODULE          : x11DirectImportExample.cpp
PACKAGE         : MapLink
=============================================================================
DESCRIPTION     : This contains a simple example of using the MapLink Direct
                  Import API in an x11 application.
****************************************************************************/

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Shell.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "MapLink.h"
#include "directimport/tsldirectimportdatalayer.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH  600
#define PATH_MAX 256

// The name of our map layer. This is used when adding the data layer
// to the drawing surface and used to reference the data layer from the
// drawing surface
static const char * MAP_LAYER_NAME = "map" ;

// The order of expose or resize depends on the X-Server being used
// and the Window Manager.
// We try to setup the MapLink Drawing Surface on the first receipt of
// either message.
static bool initial_update = true ;

static Atom wm_protocols = None;
static Atom wm_delete_window = None;

// X11 Drawing surface - global as we can always pass as client data
TSLMotifSurface * ds = NULL;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// close - handles the Window Manager Close option (window destroy)
// Must be added as a WMProtocol handler rather than a widget callback
// In this simple one window application, cleans up the drawing surface.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void close(Widget w, XtPointer clientdata, XEvent *event,Boolean *b)
{
  if (((XClientMessageEvent *)event)->type != ClientMessage)
    return;

  if ((((XClientMessageEvent *)event)->message_type == wm_protocols) &&
      (((XClientMessageEvent *)event)->data.l[0] == wm_delete_window))
  {
    /* clientdata should be the surface - however this is not always the
     * case
     */
    if (ds == NULL)
      return ;
    TSLDataLayer * dataLayer = ds->getDataLayer( MAP_LAYER_NAME ) ;

    if ( ds )
    {
      ds->removeDataLayer( MAP_LAYER_NAME ) ;
      delete ds ;
      ds = NULL ;
    }

    if ( dataLayer )
    {
      dataLayer->destroy() ;
      dataLayer = NULL ;
    }

    // Clean up - free up the configuration files
    TSLDrawingSurface::cleanup() ;
  }
  exit(0); // does not always exit
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// resize - informs the drawing surface of any change in size of the window
// Relys on the ResizeAction to maintain the view of the map sensibly
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void resize(Widget widget, XtPointer data, XEvent *event, Boolean *cont)
{
  TSLMotifSurface * ds = (TSLMotifSurface *)data ;

  if ( event->type != ConfigureNotify )
    return ;

  Dimension ww = event->xconfigure.width ;
  Dimension wh = event->xconfigure.height ;

  if ( initial_update )
  {
    ds->wndResize( 0, 0, ww, wh, false ) ;

    ds->reset( ) ;

    initial_update = false ;
  }
  else
  {
    ds->wndResize( 0, 0, ww, wh, true,
           TSLResizeActionMaintainTopLeft ) ;
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// press - simple input handler to allow zooming and panning around
// the map. Use shift key to provide zoom/pan and reset.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void press(Widget widget, XtPointer data, XEvent *event, Boolean *cont)
{
  TSLMotifSurface * ds = (TSLMotifSurface *)data ;

  bool updated = false ;

  // Get the position pressed in user units
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  bool shiftPressed = ( event->xbutton.state & ShiftMask ) ;
  double x = 0.0 ;
  double y = 0.0 ;
  ds->DUToUU( event->xbutton.x, event->xbutton.y, &x, &y ) ;


  if ( event->xbutton.button == Button1 )
  {
    // Zoom in, possibly with pan
    if ( shiftPressed )
    {
      updated = ds->zoom( 30, true, false ) ;
      if ( updated )
      {
    ds->pan( x, y, true ) ;
      }
    }
    else
    {
      updated = ds->zoom( 30, true, true ) ;
    }
  }
  else  if ( event->xbutton.button == Button2 )
  {
    if ( shiftPressed )
    {
      // Reset to original view
      updated = ds->reset( ) ;
    }
    else
    {
      // Pan to point
      updated = ds->pan( x, y, true ) ;
    }
  }
  else if ( event->xbutton.button == Button3 )
  {
    // Zoom out, possible with pan
    if ( shiftPressed )
    {
      updated = ds->zoom( 30, false, false ) ;
      if ( updated )
      {
    ds->pan( x, y, true ) ;
      }
    }
    else
    {
      updated = ds->zoom( 30, false, true ) ;
    }
  }

  // If we haven't updated the view, then we have reached the edge of the world
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if ( !updated )
  {
    printf( "Coordinate space limits reached\n" ) ;
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// expose - handles window exposures.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void expose(Widget widget, XtPointer data, XEvent *event, Boolean *cont)
{
  TSLMotifSurface * ds = (TSLMotifSurface *)data ;

  if ( initial_update )
  {
    Dimension width;
    Dimension height;

    // Query the actual Map Widget Height & Width
    XtVaGetValues(widget,
                  XtNwidth, &width,
                  XtNheight, &height,
                  NULL);
    ds->wndResize( 0, 0, width, height, false ) ;

    ds->reset( ) ;

    initial_update = false ;
  }

  long x1 = event->xexpose.x ;
  long y2 = event->xexpose.y ;
  long x2 = event->xexpose.x + event->xexpose.width ;
  long y1 = event->xexpose.y + event->xexpose.height ;

  ds->drawDU( x1, y1, x2, y2, true, true ) ;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main - main entry point of the application.
//   Creates top level shell,
//   Loads map
//   Binds drawing surface to shell
//   Adds event handlers
//   Executes event loop
//
//   For VxWorks main entry point examine the 'start()' function.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main( int argc, char *argv[] )
{
  char * filename = NULL ;
  Widget toplevel ;
  XtAppContext app;


  // Create a top level shell to display the map
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  toplevel = XtVaAppInitialize( &app, "MapLink",(XrmOptionDescList)NULL,0,
                &argc, argv, (String *)NULL,
                XtNtitle, "Simple MapLink Viewer",
                XtNwidth, WINDOW_WIDTH, XtNheight, WINDOW_HEIGHT, NULL);

  Display* display = XtDisplay( toplevel ) ;

  // catch the closing of the window so we tidy up MapLink
  wm_protocols = XInternAtom(display, "WM_PROTOCOLS", False);
  assert(wm_protocols != None);
  wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
  assert(wm_delete_window != None);
  XtAddEventHandler(toplevel, NoEventMask, True, close, &ds);

  // Display the window so that we can query dimensions in the
  // drawing surface constructor - needed to double buffer
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  XtRealizeWidget( toplevel ) ;

  // Clear MapLink's error stack
  TSLErrorStack::clear() ;

  // SETUP THE DRAWING SURFACE
  // Initialise the drawing surface data files.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TSLDrawingSurface::loadStandardConfig();
  TSLCoordinateSystem::loadCoordinateSystems();

  // SETUP THE MAP DATA LAYER
  // Create a data layer and load the map
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  std::string path = "<maplink-support-repo-location>//maplink-support//DirectImport//Unix//";
  std::string fileName = "circle";
  std::string shppath = path + fileName + ".shp";
  std::string sldpath = path + fileName + ".sld";

  TSLDirectImportDataLayer *pDirectImportDataLayer = new TSLDirectImportDataLayer();

  pDirectImportDataLayer->maxMemoryCacheSize(1024*1024);
  pDirectImportDataLayer->maxRasterDrawCacheSize(1024 * 1024);

  TSLUTF8Encoder pathOut(shppath.c_str());

  TSLvector<TSLDirectImportDataSet*>* dataSets( pDirectImportDataLayer->createDataSets(pathOut , TSLDirectImportDriver::OverviewTypeAny ) );

  const TSLCoordinateSystem* coordSystem = TSLCoordinateSystem::findByEPSG(4326);
  pDirectImportDataLayer->setCoordinateSystem(coordSystem);
  pDirectImportDataLayer->addScaleBand(0.00004, "band");

  TSLSimpleString st;
  TSLThreadedErrorStack::errorString( st, "Direct Import Errors: \n");

  bool res = false;
  if(dataSets && dataSets->size() >= 1)
  {
      TSLDirectImportDataSet *pDataSet((*dataSets)[0]);
      TSLFeatureClassConfig *pFcc = new TSLFeatureClassConfig;
      res = pFcc->load(sldpath.c_str());
      res = pDirectImportDataLayer->loadData(pDataSet, pFcc);
  }
  else
  {
    // failed to load
  }


  // Create a drawing surface displaying in the toplevel shell and
  // bind the data layer to it.
  //
  // Historical: Not Really a Motif-Surface as only Xlib calls are used.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ds = new TSLMotifSurface( display, XtWindow( toplevel ));

  ds->addDataLayer( pDirectImportDataLayer, MAP_LAYER_NAME ) ;

  // Make the drawing surface double buffered so that it looks pretty
  ds->setOption(TSLOptionDoubleBuffered, true);

  // Output any errors that occured during setup
  const char * msg1 = TSLErrorStack::errorString( "Errors :\n" ) ;
  if ( msg1 )
  {
    fprintf( stderr, "%s", msg1 ) ;

  }

  // Add in my event handlers, including one to trap shell closure
  // Pass the drawing surface, since it is useful. Could obviously
  // encapsulate the MapLink management in a class and pass a
  // pointer to a class instance, removing dependance upon global
  // variables.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  XtAddEventHandler( toplevel, StructureNotifyMask, FALSE, resize, ds ) ;
  XtAddEventHandler( toplevel, ButtonPressMask,     FALSE, press,  ds ) ;
  XtAddEventHandler( toplevel, ExposureMask,        FALSE, expose, ds ) ;

  // attach the property
  XSetWMProtocols(display, XtWindow(toplevel), &wm_delete_window, 1);

  // Finally start processing events
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  XtAppMainLoop( app ) ;
}
