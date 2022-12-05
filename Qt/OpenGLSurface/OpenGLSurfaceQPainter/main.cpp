/****************************************************************************
Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QApplication>
#include <QGLFormat>
#include <QMessageBox>
#include <stdlib.h>
#ifdef WIN32
# include <direct.h>
#else
# include <unistd.h>
#endif
#include "mainwindow.h"
#include "MapLink.h"



#ifdef WIN32
//
// The MapLink Pro TSLOpenGLSurface works best with either NVIDIA or AMD GPUs.
//
// The following should force the selection of either on Windows if the machine
// has multiple GPUs and the device drivers are sufficently new enough.
//
// The MapLink Pro TSLOpenGLSurface will work with Intel GPUs however you
// need the latest drivers directly from Intel on Windows and the latest Mesa
// drivers on Linux (potentially with patches for Haswell and older, ideally you
// should use newer GPUs from Intel).
//
// If an application crashes inside of the OpenGL drivers then we would highly
// recommend you first upgrade with drivers directly from the GPU manufactorer
// and retest.
//
extern "C" {
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
  __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif


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

  QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  QApplication application(argc, argv);

  // Parse the application's command line arguments
  QStringList argumentList = application.arguments();
  QString mapFilename;
  for( int i = 1; i < argumentList.size(); ++i )
  {
    if( argumentList[i].compare( "/help", Qt::CaseInsensitive ) == 0 ||
        argumentList[i].compare( "-help", Qt::CaseInsensitive ) == 0 )
    {
      QMessageBox::information( NULL, "Help",
                                "Help:\n  SampleExe /home path_to_install\t(The directory containing the config directory)" );
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
    else
    {
      mapFilename = argumentList[i];
    }
  }

  // QPainter requires a compatability profile as it is currently limited to OpenGL 2.0.
  // Qt 5.9 has a number of fixes that allow CoreProfile to work see bug report:
  //
  // 	https://bugreports.qt.io/browse/QTBUG-33535
  //
  // This means that when Qt 5.9 is release there may be a few changes required.
  //
  // Because of the issues around setting the capabilities of OpenGL required by MapLink Pro
  // we have left the choice upto Qt for the default settings.
  //
  // In the method OffScreenHelper::setup() we setup the minimum capabilities we require
  // for MapLink Pro.
  //
  // There is a slight risk that the OpenGL implementation will not permit sharing of resources
  // between OpenGL contexts. If this occurs then the two OpenGL contexts will need to be
  // configured in a compatabile way that is driver specific. In this case it might be more
  // sensible to update to Qt 5.9 or newer.
  //

  // Show the application window
  MainWindow window;
  window.show();

  // if a map has been passed on the command line open it.
  if( !mapFilename.isEmpty() )
  {
    window.loadFile( mapFilename.toUtf8() );
  }
  return application.exec();
}

