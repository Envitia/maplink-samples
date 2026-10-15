/****************************************************************************
		Copyright (c) 2001-2017 by Envitia Group PLC.
=============================================================================
MODULE          : vxworks.cpp
PACKAGE         : MapLink
AUTHOR(s)       : D.Dixon
=============================================================================
DESCRIPTION     : VxWorks specific code.
  Missing standard functions
  X11 Environment setup
  MapLink Environment setup
  'start_prog' entry point which calls main(argc, argv).

****************************************************************************/
#ifdef VXWORKS

#include "vxworks.h"

#include <string.h>

#ifdef MOTIF
# include <Xm/Xm.h>
# include <Xm/Protocols.h>
#endif

#if _WRS_VXWORKS_MAJOR > 5
# include <wrn/coreip/nfsDriver.h>
#else
# include <nfsDrv.h>
# include <nfsLib.h>
#endif

extern int main( int argc, char *argv[] );


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Callback from Motif When requested font is not available
// Ensures that a font which should always be available is returned.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef MOTIF
void no_font_callback(Widget w, XtPointer client_data, XtPointer call_data)
{
  XmDisplayCallbackStruct *dp = (XmDisplayCallbackStruct *) call_data;

  if (dp->reason == XmCR_NO_FONT) {
    Arg args[3];
    Cardinal n = 0;
    XtSetArg(args[n], XmNfontName, "fixed"); n ++;
    XtSetArg(args[n], XmNfontType, XmFONT_IS_FONT); n ++;
    XtSetArg(args[n], XmNloadModel, XmLOAD_IMMEDIATE); n ++;
    XmRenditionUpdate(dp->rendition, args, n);
  }
}
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// realpath
//   Missing function.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
char *realpath(const char *file_name, char *resolved_name)
{
  strcpy(resolved_name, file_name);
  return resolved_name;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Setup the MapLink and X11 environment variables.
// These will need to be modified for your VxWorks environment.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void setupVxWorksEnv()
{
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Setup the X11 environment
  //
  // In our developement environment we use 'castor' as
  // our X-server (Solaris 8, Workstation)
  //
  // The X-Server directory is exported and mounted on our
  // VxWorks rig as follows:
  //
  //   hostAdd "castor","192.168.0.15"
  //   nfsMount "apollo", "/user3/vxworks/vx_x11/X11R6", "/X11R6"
  //   nfsMount "apollo", "/user3/vxworks/vx_x11/X11R6", "/usr/X11R6"
  //   @cd "/X11R6"
  //
  // Mount one of our developement directories:
  //
  //   nfsMount "castor","/usr1","/castor_usr1"
  //
  */

  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Define where to pick up MapLink default
  // configuration files.
  */

  /* copy global environment - required for vxworks 6.x */
#if _WRS_VXWORKS_MAJOR > 5
  envPrivateCreate(0, 0);
#endif

  // where to display the client window
  printf("DISPLAY         = %s\n", getenv("DISPLAY"));
  // root of maplink installation (where to find the config directory)
  printf("MAPL_HOME       = %s\n", getenv("MAPL_HOME"));
  // library path - should point to where the MapLink shared libraries
  // are - specifically the .o files in the redist directory
  printf("LD_LIBRARY_PATH = %s\n", getenv("LD_LIBRARY_PATH"));
  // language - usually set to C
  printf("LANG            = %s\n", getenv("LANG"));
  // X11 locale directory
  printf("XLOCALEDIR      = %s\n", getenv("XLOCALEDIR"));
  // Home directory
  printf("HOME            = %s\n", getenv("HOME"));
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Main entry point for VxWorks
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
extern "C" int start_prog(char *mapFilename)
{
  /*
  // We can not easily start a VxWorks task and pass a variable
  // list of arguments. The following minics what this example
  // requires as input.
  //
  // Start the task as follows:
  //
  //   taskSpawn "main", 100, 8, 100000, start_prog, "fullpath_to_map_file"
  //
  // Also please read the Release Notes and look at 'setupVxWorksEnv()'
  // function to ensure that your environment is setup correctly.

  cd "/maplink/samples/Unix/simpleC++"
  ld < example_vxworks_gcc
  taskSpawn "main", 100, 8, 200 * 1024, start_prog, "/maps/World/World.map"

  */
  int argc = 4;
  char *argv[8];

  setupVxWorksEnv();

  /* copy the environment variable contents */
  char display[256];
  strcpy(display, getenv("DISPLAY") == NULL ? "127.0.0.0:0" : getenv("DISPLAY"));
  argv[0] = "main";
  argv[1] = "-display"; /* required for VxWorks 6.x for starting X applications */
  argv[2] = display;
  argv[3] = mapFilename;
  argv[4] = NULL;

  printf("Using -display %s\n", display);

  return main(argc, argv);	
}
#endif /* VxWorks */


