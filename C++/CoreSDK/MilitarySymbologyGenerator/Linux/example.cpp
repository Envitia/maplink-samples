/****************************************************************************
		Copyright (c) 2017 by Envitia Group PLC.
=============================================================================
MODULE          : example.cpp
PACKAGE         : MapLink
SCCS            : %W% %G%
AUTHOR(s)       : D.Vint
=============================================================================
DESCRIPTION     : This contains a simple example of using the MapLink 4 API
                  in a Motif application.
****************************************************************************/
#include "vxworks.h"

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include <X11/Shell.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>

#include "MapLink.h"
#undef Status
#include <tslmilitarysymbologygenerator.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH  600

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
// usage - description of command line options
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void usage( const char * appname ) 
{
  printf( "Usage : %s [-help] mapname.map\n", appname ) ;
  printf( "        LMB press       = zoom in\n" ) ;
  printf( "        RMB press       = zoom out\n" ) ;
  printf( "        MMB press       = pan to point\n" ) ;
  printf( "        Shift/LMB press = zoom in and pan to point\n" ) ;
  printf( "        Shift/RMB press = zoom out and pan to point\n" ) ;
  printf( "        Shift/MMB press = reset\n" ) ;
  exit( 0 ) ;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main - main entry point of the application. Pretty simple really.
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

  // Check command line arguments
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for ( int i = 1 ; i < argc ; i++ )
  {
    if ( !strcmp( argv[i], "-help" ) || !strcmp( argv[i], "-h" ) )
    {
      usage( argv[0] ) ;
    }
    else
    {
      filename = argv[i] ;
    }
  }

  if ( !filename )
  {
    usage( argv[0] ) ;
  }

  // Figure out the real path to the Map file.
  char resolvedname[PATH_MAX];

  if (realpath(filename, resolvedname) == NULL)
  {
    fprintf(stderr, "Unable to resolve path to '%s'\n", filename);
    exit(0);
  }

  // Create a top level shell to display the map
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  toplevel = XtVaAppInitialize( &app, "MapLink",(XrmOptionDescList)NULL,0,
				&argc, argv, (String *)NULL,
				XtNtitle, "Simple MapLink Viewer",
				XtNwidth, WINDOW_WIDTH, XtNheight, WINDOW_HEIGHT, NULL);

  Display* display = XtDisplay( toplevel ) ;

#ifdef VXWORKS  
#ifdef MOTIF
  // This is to handle missing Font's only and usually only occurs
  // where a full set of fonts is not available.
  Widget xmdisplay = XmGetXmDisplay(display);
  XtAddCallback(xmdisplay, XmNnoFontCallback, no_font_callback, NULL);
#endif
#endif

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

  // SETUP THE MAP DATA LAYER
  // Create a data layer and load the map
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  TSLMapDataLayer * dataLayer = new TSLMapDataLayer();

  if ( !dataLayer->loadData( resolvedname ) )
  {
    printf( "Cannot load map '%s'\n", resolvedname ) ;
    exit( 0 ) ;
  }

  // Create a drawing surface displaying in the toplevel shell and
  // bind the data layer to it.
  // 
  // Historical: Not Really a Motif-Surface as only Xlib calls are used.
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ds = new TSLMotifSurface( display, XtWindow( toplevel )); 

  ds->addDataLayer( dataLayer, MAP_LAYER_NAME ) ;

  // Make the drawing surface double buffered so that it looks pretty
  ds->setOption(TSLOptionDoubleBuffered, true);


  TSLStandardDataLayer * dynamic_layer = new TSLStandardDataLayer();
  ds->addDataLayer( dynamic_layer, "symbols overlay" ) ;

  static std::vector<char const*> const sidcStrings = {
	//"10051000001417010000",
	//"10031000001652000000",
	//"10030100001101010000",
	
  "10031000001211000000",
//"10031000001630000000",
	
  //"10061000001211000000",
//	"10031000001211010000",
//	"10030100001101000000",
//	"10030100001201000000",
//	"10060100001101000000",
//	"10060100001201000000",
//	"10034000001101070000",
//	"10032000001208010000",
	"10131000001211000000",
	"10231000001630000000",
	"10031010001630000000",
	"10031020001630000000",
	"10031030001630000000",
	"10031040001630000000",
	"10031050001630000000",
	"10031007001630000000",
	"10031000001417000000",
	"10031000001417010000",
	"10031000001417020000",
	"10031000001417950000",
	"10031000001417960000",
	"10031000001417970000",
	"10031000001417980000",
	//"10431037171211059797"
	//"10031037141211050000"
  };

  static auto it = sidcStrings.begin();
  const char* maplinkHome = getenv("MAPL_HOME");
  TSLMilitarySymbologyGenerator symbolGenerator;
  if (!maplinkHome || !symbolGenerator.loadSymbols(std::filesystem::path(maplinkHome) / "config/symbols/MILSTD_2525D"))
  {
    fprintf(stderr, "Unable to load military symbol components\n");
    exit(1);
  }

  TSLSymbol* symbol = symbolGenerator.getSymbol(*it);
  TSLTMC x1, y1, x2, y2;
  if (!symbol || !dataLayer->getTMCExtent(&x1, &y1, &x2, &y2))
  {
    fprintf(stderr, "Unable to create or position military symbol\n");
    exit(1);
  }
  symbol->move(TSLCoord(x1 + (x2 - x1) / 2, y1 + (y2 - y1) / 2), {0,0});
  assert(dynamic_layer->entitySet()->insert(symbol));
  dynamic_layer->notifyChanged(true);


  // Output any errors that occured during setup
  const char * msg1 = TSLErrorStack::errorString( "Errors :\n" ) ;
  if ( msg1 )
  {
    fprintf( stderr, "%s", msg1 ) ;
    exit( 0 ) ;
  }

#if defined(AXPO)
  // DEC-UNIX for some reason does not give us an StructureNotify event when
  // other flavours of UNIX do.  Therefore the drawing surface is not resized 
  // correctly before being told to draw.
  // So give the drawing surface an arbritrary window size of 1x1, and reset it
  // now.
  ds->wndResize( 0, 0, 1, 1, 0, TSLResizeActionNone );
  ds->reset( 0 );
#endif

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

#if defined(VXWORKS)
  // Depending on if a window manager is present we may not get
  // a resize and/or expose so setup the drawing surface
  // manually.
  ds->wndResize( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, false ) ;
  ds->reset( true );
#endif

  // Finally start processing events
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  XtAppMainLoop( app ) ;
}

