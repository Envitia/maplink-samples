/****************************************************************************
Copyright (c) 2008-2022 by Envitia Group PLC.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free 
Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more 
details.

You should have received a copy of the GNU Lesser General Public License 
along with this program. If not, see <https://www.gnu.org/licenses/>.

****************************************************************************/

#include <QtGui>
#include <QApplication>
#include <QMessageBox>
#include <string>
#include <QFileInfo>
#include <QFileDialog>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //! Qt setup
  //! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  QApplication app(argc, argv);

  //! Parse the application's command line arguments
  QStringList argumentList = app.arguments();
  QString mapFilename;
  QString configFilePath;
  for (int i = 1; i < argumentList.size(); ++i)
  {
    if (argumentList[i].compare("/help", Qt::CaseInsensitive) == 0 ||
      argumentList[i].compare("-help", Qt::CaseInsensitive) == 0)
    {
      QMessageBox::information(NULL, "Help",
        "Help:\n  SimpleGLSample /home path_to_install\t(The directory containing the config directory)");
      return 0;
    }
    else if ((argumentList[i].compare("/home", Qt::CaseInsensitive) == 0 ||
      argumentList[i].compare("-home", Qt::CaseInsensitive) == 0)
      && i + 1 < argumentList.size())
    {
      TSLUtilityFunctions::setMapLinkHome(argumentList[i + 1].toUtf8(), true);
      ++i;
    }
    else if ((argumentList[i].compare("/config", Qt::CaseInsensitive) == 0 ||
      argumentList[i].compare("-config", Qt::CaseInsensitive) == 0)
      && i + 1 < argumentList.size())
    {
      configFilePath = argumentList[i+1];
      ++i;
    }
    else
    {
      mapFilename = argumentList[i];
    }
  }

  //! If the configurations file is not provided, use a default config file in the same directory of the exe.
  QString defaultConfingFileName = "trackssampleconfig.xml";
  if (configFilePath.isEmpty())
  {
    configFilePath = defaultConfingFileName;
  }

  //! If the configurations file does not exist, show a message box.
  bool isConfigFileExists = false;
  do
  {
    QFileInfo check_file(configFilePath);
    isConfigFileExists = check_file.exists() && check_file.isFile();
    if (!isConfigFileExists)
    {
      QString msg = "Configuration file not found!\nPlease pass the configurations file using the following argument:\n -config {path_to_configurations_file}"
        "\nIf no config argument is passed, a default configuration file with name: (" + defaultConfingFileName + ") must exist in the executable directory."
        "\n\nClick (Yes) to manually choose the configuration file!\nClick (No) to exit.";
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(NULL, "Configuration file not found!", msg,
        QMessageBox::Yes | QMessageBox::No);
      if (reply == QMessageBox::Yes)
      {
        QString initDir = "";
        QString fileName = QFileDialog::getOpenFileName(NULL, "Choose configuration file", initDir, "XML files (*.xml)");

        if (!fileName.isEmpty())
        {
          configFilePath = fileName.toUtf8();
        }
      }
      else
      {
        return 0;
      }
    }
  } while (!isConfigFileExists);

  MainWindow mainWindow;
  //! parse Configuration File
  QString msgError;
  if (!mainWindow.parseConfigurationFile(configFilePath, msgError))
  {
    QMessageBox::critical(NULL, "Configuration file not parsed!",
      "Configuration file not parsed!\nPlease check the contents on the configuration file: " + configFilePath + "\nError: " + msgError);

    return 0;
  }

  mainWindow.show();

  //! if a map has been passed on the command line open it.
  if (!mapFilename.isEmpty())
  {
    mainWindow.loadMap(mapFilename.toUtf8());
  }

  mainWindow.showMaximized();

  app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
  return app.exec();
}
