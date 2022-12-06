/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <QtGui>
#include <QApplication>
#include <QMessageBox>
#include <string>
#include "mainwindow.h"
#include <MapLink.h>

int main(int argc, char *argv[])
{
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! Qt setup
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  QApplication app(argc, argv);

  //! Parse the application's command line arguments
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

  MainWindow mainWindow;
  mainWindow.show();  

  //! if a map has been passed on the command line open it.
  if (!mapFilename.isEmpty())
  {
    mainWindow.loadLayer(mapFilename.toUtf8());
  }

  mainWindow.showMaximized();

  app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
  return app.exec();
}
