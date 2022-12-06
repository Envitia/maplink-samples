/****************************************************************************
				Copyright (c) 1998 to 2021 by Envitia Group PLC.
****************************************************************************/

#include <QtGui>
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <string>
#include "mainwindow.h"

//! 
//! Detect editor configuration file
//!
//! Check if the editor config ini file is valid. Otherwise, try to use a default file or ask the user to provide one.
//!
//! @param editor configuration file path to detect.
//! @return true if successful. False otherwise.
//!
bool checkConfigFilePath(QString &configFilePath)
{
	// check if the provided config path through the command line arguements is valid.
	if (!configFilePath.isEmpty())
	{
		QFileInfo check_file(configFilePath);
		bool isConfigFileExists = check_file.exists() && check_file.isFile();
		if (isConfigFileExists)
		{
			return true;
		}
	}

	//! If the configurations file is not provided, use a default config file in the same directory of the exe.
	const QString defaultConfingFileName = "editor.ini";
	QFileInfo check_file_def(defaultConfingFileName);
	bool isConfigFileExists = check_file_def.exists() && check_file_def.isFile();
	if (isConfigFileExists)
	{
		configFilePath = defaultConfingFileName;
		return true;
	}

	//! If the default configurations file does not exist in the same folder, use the file in the sample's MapLink installation if exists.
	QString maplIniFile;
	const char * maplinkHome = TSLUtilityFunctions::getMapLinkHome();
	if (maplinkHome)
	{
		maplIniFile.append(maplinkHome);
		maplIniFile.append("\\samples\\qt\\qteditorinteractionmodes\\editor.ini");
	}
	QFileInfo check_file_mapl(maplIniFile);
	isConfigFileExists = check_file_mapl.exists() && check_file_mapl.isFile();
	if (isConfigFileExists)
	{
		configFilePath = maplIniFile;
		return true;
	}

	// if we still can't detect the ini file, ask the user to provide one.
	do
	{
		QFileInfo check_file(configFilePath);
		isConfigFileExists = check_file.exists() && check_file.isFile();
		if (!isConfigFileExists)
		{
			QString msg = "Editor configuration file not found!\nPlease pass the editor configurations file using the following argument:\n -config {path_to_configurations_file}"
				"\nIf no config argument is passed, a default configuration file with name: (" + defaultConfingFileName + ") must exist in the executable directory."
				"\n\nClick (Yes) to manually choose the configuration file!\nClick (No) to exit.";
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(NULL, "Configuration file not found!", msg,
				QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes)
			{
				QString initDir = "";
				QString fileName = QFileDialog::getOpenFileName(NULL, "Choose editor configuration file", initDir, "INI files (*.ini)");

				if (!fileName.isEmpty())
				{
					configFilePath = fileName.toUtf8();
				}
			}
			else
			{
				return false;
			}
		}
	} while (!isConfigFileExists);

	return true;
}

int main(int argc, char *argv[])
{
	//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//! Qt setup
	//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	QApplication app(argc, argv);

	//! Parse the application's command line arguments
	QStringList argumentList = app.arguments();
	QString mapFilename, tmfFilepath, configFilePath;
	for (int i = 1; i < argumentList.size(); ++i)
	{
		if (argumentList[i].compare("/help", Qt::CaseInsensitive) == 0 ||
			argumentList[i].compare("-help", Qt::CaseInsensitive) == 0)
		{
			QMessageBox::information(NULL, "Help",
				"Help(arguments):\n============\n"
				"  -home [path_to_install_maplink (The directory containing the config directory)]\n"
				"  -config [path_to_editor_config_file (The editor configuration ini file)]\n"
				"  -map [path_to_map_file_path (The map file to load when the application starts)]\n"
				"  -tmf [path_to_tmf_file_path (The tmf file to load when the application starts)]\n");

			return 0;
		}
		else if ((argumentList[i].compare("/home", Qt::CaseInsensitive) == 0 ||
			argumentList[i].compare("-home", Qt::CaseInsensitive) == 0)
			&& i + 1 < argumentList.size())
		{
			TSLUtilityFunctions::setMapLinkHome(argumentList[i + 1].toUtf8(), true);
			++i;
		}
		else if ((argumentList[i].compare("/tmf", Qt::CaseInsensitive) == 0 ||
			argumentList[i].compare("-tmf", Qt::CaseInsensitive) == 0)
			&& i + 1 < argumentList.size())
		{
			tmfFilepath = argumentList[i + 1];
			++i;
		}
		else if ((argumentList[i].compare("/map", Qt::CaseInsensitive) == 0 ||
			argumentList[i].compare("-map", Qt::CaseInsensitive) == 0)
			&& i + 1 < argumentList.size())
		{
			mapFilename = argumentList[i + 1];
			++i;
		}
		else if ((argumentList[i].compare("/config", Qt::CaseInsensitive) == 0 ||
			argumentList[i].compare("-config", Qt::CaseInsensitive) == 0)
			&& i + 1 < argumentList.size())
		{
			configFilePath = argumentList[i + 1];
			++i;
		}
		else
		{
			mapFilename = argumentList[i];
		}
	}

	// detect the config ini file path if not provided
	if (!checkConfigFilePath(configFilePath))
		return 0;

	// setup the main window
	MainWindow mainWindow;
	mainWindow.initializeEditMode(configFilePath.toUtf8());

	//! if a map has been passed on the command line open it.
	if (!mapFilename.isEmpty())
	{
		mainWindow.loadMap(mapFilename.toUtf8());
	}

	//! if a tmf has been passed on the command line open it.
	if (!tmfFilepath.isEmpty())
	{
		mainWindow.loadGeometryFile(tmfFilepath.toUtf8());
	}

	mainWindow.showMaximized();
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	return app.exec();
}
