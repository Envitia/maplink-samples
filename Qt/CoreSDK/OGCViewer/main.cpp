/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QApplication>
#include <QMessageBox>
#include <stdlib.h>
#ifdef WIN32
# include <direct.h>
#else
# include <unistd.h>
#endif
#include "mainwindow.h"
#include "MapLink.h"
#include "MapLinkDrawing.h"

// This file contains the application's entry point.

int main(int argc, char *argv[])
{
#ifdef WIN32
   wchar_t *buffer = _wgetcwd(NULL, 0);
   QString currentWorkingDir = QString::fromWCharArray( buffer );
#else
  char *buffer = getcwd(NULL, 0);
  QString currentWorkingDir = QString::fromUtf8( buffer );
#endif
  free( buffer );

  ////////////////////////////////////////////////////
  // Normally the following would not be required.
  // However to ensure that an application using Qt
  // for its GUI can run standalone with out interfering
  // with a users installed Qt we need to do the
  // following.
#ifdef ENVITIA_BUILD
  QString path(currentWorkingDir);

  QString qtPluginPath("QT_PLUGIN_PATH=");
  qtPluginPath += currentWorkingDir;
  qtPluginPath += "/qtplugins";

# ifdef WIN32
  _wputenv(qtPluginPath.toStdWString().c_str());
# else
  putenv(qtPluginPath.toUtf8());
# endif

  QCoreApplication::addLibraryPath (path);
#endif // ENVITIA_BUILD
  ////////////////////////////////////////////////////

  QCoreApplication::setOrganizationName("Envitia");
  QCoreApplication::setOrganizationDomain("envitia.com");
  QCoreApplication::setApplicationName("WMS/WMTS Service Viewer");

  QApplication application(argc, argv);
  //application.setAttribute( Qt::AA_NativeWindows );

  // Parse the application's command line arguments
  QStringList argumentList = application.arguments();
  for( int i = 1; i < argumentList.size(); ++i )
  {
    if( argumentList[i].compare( "/help", Qt::CaseInsensitive ) == 0 ||
        argumentList[i].compare( "-help", Qt::CaseInsensitive ) == 0 )
    {
      QMessageBox::information( NULL, "Help",
                                "Help:\n  OGCServiceViewer /home path_to_install\t(The directory containing the config directory)" );
      return 0;
    }
    else if( (argumentList[i].compare( "/home", Qt::CaseInsensitive ) == 0 ||
              argumentList[i].compare( "-home", Qt::CaseInsensitive ) == 0)
             && i+1 < argumentList.size() )
    {
      QString homePath = argumentList[i+1];
      if( argumentList[i+1][0] == '.' )
      {
        homePath = currentWorkingDir + "/" + argumentList[i+1];
      }
      TSLUtilityFunctions::setMapLinkHome( homePath.toUtf8(), true );
      ++i;
    }
  }

  // Load the standard MapLink configuration files
  TSLDrawingSurface::loadStandardConfig();
  TSLCoordinateSystem::loadCoordinateSystems();

  // Show the application window
  MainWindow window;
  window.show();

  return application.exec();
}
