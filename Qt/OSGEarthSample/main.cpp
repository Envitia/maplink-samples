/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>

#include <osgViewer/Viewer>
#include <osgEarthQt/ViewerWidget>

#include "mainwindow.h"

#include "MapLink.h"

#ifndef WINNT
# include <unistd.h>
#endif

int main(int argc, char** argv)
{
#ifdef WIN32
  wchar_t *buffer = _wgetcwd(NULL, 0);
  QString currentWorkingDir = QString::fromWCharArray( buffer );
#else
  char *buffer = getcwd(NULL, 0);
  QString currentWorkingDir = QString::fromUtf8( buffer );

  // osgQt will use X11 resources from the
  // Qt event thread. XInitThreads must be
  // called for the sample to function
  // correctly
  if( !XInitThreads() )
  {
    return 1;
  } 
#endif
  free( buffer );
  
  ////////////////////////////////////////////////////
  // Normally the following would not be required.
  // However to ensure that an application using Qt
  // for its GUI can run standalone without interfering
  // with a user's installed Qt, we need to do the
  // following.
#ifdef ENVITIA_BUILD
  QString path( currentWorkingDir );

  QString qtPluginPath( "QT_PLUGIN_PATH=" );
  qtPluginPath += currentWorkingDir;
  qtPluginPath += "/qtplugins";

# ifdef WIN32
  _wputenv( qtPluginPath.toStdWString().c_str() );
# else
  putenv( qtPluginPath.toUtf8() );
# endif

  QCoreApplication::addLibraryPath( path );
#endif // ENVITIA_BUILD
  ////////////////////////////////////////////////////

  QApplication application(argc, argv);
    
  // Parse the application's command line arguments
  QStringList argumentList = application.arguments();
  for( int i = 1; i < argumentList.size(); ++i )
  {
    if( argumentList[i].compare( "/help", Qt::CaseInsensitive ) == 0 ||
        argumentList[i].compare( "-help", Qt::CaseInsensitive ) == 0 )
    {
      QMessageBox::information( NULL, "Help",
                                "Help:\n  osgearthsample /home path_to_install\t(The directory containing the config directory)" );
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
  
  // Display the window, and start the application
  MainWindow window;
  window.show();

  TSLErrorStack::clear();
  if( !window.initMapLink() )
  {
    return 1;
  }

  osg::ArgumentParser args( &argc, argv);
  if( !window.initOsgEarth( args ) )
  {
    return 1;
  }
  
  return application.exec();
}
