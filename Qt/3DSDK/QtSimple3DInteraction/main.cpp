/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

#ifndef WIN32
# include <X11/Xlib.h>
#else
# include <windows.h>
#endif

#include "MapLink.h"

int main(int argc, char *argv[])
{
#ifndef WIN32
  // The 3D SDK uses Xlib in multiple threads, therefore it is necessary to call XInitThreads
  // to ensure global data is protected before any other XLib calls are made. The best way to
  // ensure this is to do this at the start of main
  XInitThreads();
#endif

  // Qt setup - creates the application window
  QApplication app(argc, argv);

   // Parse the application's command line arguments
  QStringList argumentList = app.arguments();
  QString mapFilename;
  for( int i = 1; i < argumentList.size(); ++i )
  {
    if( argumentList[i].compare( "/help", Qt::CaseInsensitive ) == 0 ||
        argumentList[i].compare( "-help", Qt::CaseInsensitive ) == 0 )
    {
      QMessageBox::information( NULL, "Help",
                                "Help:\n  SimpleGLSample /home path_to_install\t(The directory containing the config directory)" );
      return 0;
    }
    else if( (argumentList[i].compare( "/home", Qt::CaseInsensitive ) == 0 ||
              argumentList[i].compare( "-home", Qt::CaseInsensitive ) == 0)
             && i+1 < argumentList.size() )
    {
      TSLUtilityFunctions::setMapLinkHome( argumentList[i+1].toUtf8(), true );
      ++i;
    }
    else
    {
      mapFilename = argumentList[i];
    }
  }

  MainWindow *window = new MainWindow();
  window->show();

  int result = app.exec();

  // This function should be called to ensure all static data created by MapLink is cleaned up.
  // Once this function has been called no MapLink functions should be used.
  TSLDrawingSurface::cleanup() ;

  return result;
}

