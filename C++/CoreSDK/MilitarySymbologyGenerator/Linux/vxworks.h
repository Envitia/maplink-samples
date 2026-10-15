/****************************************************************************
                Copyright (c) 2003-2017 by Envitia Group PLC.
=============================================================================*/
//      ********************************************************
//      *           Source Control Information
//      ********************************************************
//      $Header$
//      $Workfile$
//      $Author$
//      $Date$
//      $Revision$
//      ********************************************************
#ifndef _VXWORKS_H_
# define _VXWORKS_H_
# include <stdlib.h>
# include <stdio.h>
# include <X11/Xlib.h>
# include <X11/Intrinsic.h>
# ifdef VXWORKS
#  include <vxWorks.h>

#  include <version.h>
#  include <envLib.h>
#  include <unistd.h>

extern char *realpath(const char *file_name, char *resolved_name);
#  ifdef MOTIF
extern void no_font_callback(Widget w, XtPointer client_data, XtPointer call_data);
#  endif

// Can't have main function called from another function.
#  define main main_entry

# endif
#endif

