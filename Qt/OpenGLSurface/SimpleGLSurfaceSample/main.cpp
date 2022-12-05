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
// The following should force the selection of either on WIndows if the machine
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
                                "Help:\n  SimpleGLSample /home path_to_install\t(The directory containing the config directory)" );
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

  // Qt will be creating the OpenGL context for us. In order for the drawing surface to
  // work at its best we ask it to choose a framebuffer configuration with a specific set of
  // parameters.
  QGLFormat f;

  // Request a 24-bit depth buffer. A 16-bit depth buffer will also work.
  f.setDepthBufferSize( 24 );

  // Request a double-buffered configuration to eliminate flickering when moving around the map
  f.setDoubleBuffer( true );

  // Request 4x multisampling anti-aliasing if supported by the hardware
  f.setSamples( 4 );

  // Request an OpenGL 3.2 core profile context if supported by the hardware
  f.setVersion( 3, 2 );
  f.setProfile( QGLFormat::CoreProfile );

  QGLFormat::setDefaultFormat( f );

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
