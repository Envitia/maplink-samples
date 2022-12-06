/****************************************************************************
Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <QApplication>
#include <QGLFormat>
#include <QMessageBox>
#include "mainwindow.h"
#include "MapLink.h"

int main(int argc, char *argv[])
{
  QApplication application(argc, argv);

  // Parse the application's command line arguments
  QStringList argumentList = application.arguments();
  for( int i = 1; i < argumentList.size(); ++i )
  {
    if( argumentList[i].compare( "/help", Qt::CaseInsensitive ) == 0 ||
      argumentList[i].compare( "-help", Qt::CaseInsensitive ) == 0 )
    {
      QMessageBox::information( NULL, "Help",
        "Help:\n  qtdesignersample /home path_to_install\t(The directory containing the config directory)" );
      return 0;
    }
    else if( (argumentList[i].compare( "/home", Qt::CaseInsensitive ) == 0 ||
      argumentList[i].compare( "-home", Qt::CaseInsensitive ) == 0)
      && i+1 < argumentList.size() )
    {
      TSLUtilityFunctions::setMapLinkHome( argumentList[i+1].toUtf8(), true );
      ++i;
    }
  }

  // Set the default OpenGL framebuffer format used by Qt to include an alpha channel as
  // this is necessary for optimal use of the Accelerator SDK
  QGLFormat f;
  f.setAlpha( true );
  QGLFormat::setDefaultFormat(f);

  MainWindow w;
  w.setWindowTitle("MapLink Accelerator Example");
  w.show();
  return application.exec();
}
