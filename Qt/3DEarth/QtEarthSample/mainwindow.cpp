/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <QtGui>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <string>
using namespace std;

#include "mainwindow.h"

//! This class is the main window of the application. It receives events from the user and
//! passes them to the widget containing the drawing surface
MainWindow::MainWindow(QWidget *parent) 
  : QMainWindow(parent) 
  , m_surfaceController(nullptr)
{ 
  //! Construct the window
  setupUi(this);

  //! Connect the actions for the toolbars and menus to the slots that deal with them
  connect(action_Open, SIGNAL(triggered()), this, SLOT(loadLayer()));
  connect(actionReset, SIGNAL(triggered()), this, SLOT(resetView()));
  connect(actionFull_Screen, SIGNAL(triggered()), this, SLOT(fullScreen()));
  connect(actionTrackball_Mode, SIGNAL(triggered()), this, SLOT(activate_Trackball_Mode()));
  connect(actionSelect_Mode, SIGNAL(triggered()), this, SLOT(activate_Select_Mode()));
  connect(actionCreate_Polygon_Mode, SIGNAL(triggered()), this, SLOT(activate_CreatePolygon_Mode()));
  connect(actionCreate_Polyline_Mode, SIGNAL(triggered()), this, SLOT(activate_CreatePolyline_Mode()));
  connect(actionCreate_Text_Mode, SIGNAL(triggered()), this, SLOT(activate_CreateText_Mode()));
  connect(actionCreate_Symbol_Mode, SIGNAL(triggered()), this, SLOT(activate_CreateSymbol_Mode()));
  connect(actionCreate_Extruded_Polygon_Mode, SIGNAL(triggered()), this, SLOT(activate_CreateExtrudedPolygon_Mode()));
  connect(actionCreate_Extruded_Polyline_Mode, SIGNAL(triggered()), this, SLOT(activate_CreateExtrudedPolyline_Mode()));
  connect(actionDelete_Geometry_Mode, SIGNAL(triggered()), this, SLOT(activate_DeleteGeometry_Mode()));

  connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutBox()));
  connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

  //! Create a group of actions for the interaction mode buttons and menus so that
  //! the active interaction mode is reflected in the toolbar and menu display
  m_interactionModesGroup = new QActionGroup(mainToolBar);
  m_interactionModesGroup->addAction(actionTrackball_Mode);
  m_interactionModesGroup->addAction(actionSelect_Mode);
  m_interactionModesGroup->addAction(actionCreate_Polygon_Mode);
  m_interactionModesGroup->addAction(actionCreate_Polyline_Mode);
  m_interactionModesGroup->addAction(actionCreate_Text_Mode);
  m_interactionModesGroup->addAction(actionCreate_Symbol_Mode);
  m_interactionModesGroup->addAction(actionCreate_Extruded_Polygon_Mode);
  m_interactionModesGroup->addAction(actionCreate_Extruded_Polyline_Mode);
  m_interactionModesGroup->addAction(actionDelete_Geometry_Mode);

  m_surfaceController = new SurfaceController(this);
  setCentralWidget(m_surfaceController);
}  

MainWindow::~MainWindow()
{
  //! Clean up
  delete m_interactionModesGroup;

  if(m_surfaceController)
      delete m_surfaceController;
}


void MainWindow::loadLayer(const char *filename)
{
  if (m_surfaceController)
  {
	  m_surfaceController->loadLayer(filename);
  }
}

void MainWindow::loadLayer()
{
  QString initDir = "";
  QString fileName = QFileDialog::getOpenFileName(this, tr("Load layer"), initDir, tr("All Map files (*.map *.mpc *.tdf)"));

  if (!fileName.isEmpty() && m_surfaceController)
  {
	  m_surfaceController->loadLayer( fileName.toUtf8() );
  }
}


void MainWindow::showAboutBox()
{
  //! Display an about box
  QMessageBox::about(this, tr("MapLink Pro Qt Interaction modes Sample"),
    tr("<img src=\":/images/envitia.png\"/>"
      "<p>Copyright &copy; 1998-2017 Envitia Group PLC. All rights reserved.</p>"
#ifdef WIN32
      "<p align=center style=\"color:#909090\">Portions developed using LEADTOOLS<br>"
      "Copyright &copy; 1991-2009 LEAD Technologies, Inc. ALL RIGHTS RESERVED</p>"
#endif
    ));
}

void MainWindow::resetView()
{
  if (m_surfaceController)
  {
    //! Tell the widget to reset the viewing area to its maximum extent
	  m_surfaceController->resetView();
  }
}

void MainWindow::fullScreen()
{
	if (m_surfaceController)
	{
		//! Tell the widget to show/hide camera tool gui
		m_surfaceController->fullScreen();
	}
}

void MainWindow::activate_Trackball_Mode()
{
	if (m_surfaceController)
	{
		m_surfaceController->activate_Trackball_Mode();
	}
}
void MainWindow::activate_Select_Mode()
{
	if (m_surfaceController)
	{
		m_surfaceController->activate_Select_Mode();
	}
}
void MainWindow::activate_CreatePolygon_Mode()
{
	if (m_surfaceController)
	{
		m_surfaceController->activate_CreatePolygon_Mode();
	}
}
void MainWindow::activate_CreatePolyline_Mode()
{
	if (m_surfaceController)
	{
		m_surfaceController->activate_CreatePolyline_Mode();
	}
}
void MainWindow::activate_CreateText_Mode()
{
	if (m_surfaceController)
	{
		m_surfaceController->activate_CreateText_Mode();
	}
}
void MainWindow::activate_CreateSymbol_Mode()
{
	if (m_surfaceController)
	{
		m_surfaceController->activate_CreateSymbol_Mode();
	}
}
void MainWindow::activate_CreateExtrudedPolygon_Mode()
{
	if (m_surfaceController)
	{
		m_surfaceController->activate_CreateExtrudedPolygon_Mode();
	}
}
void MainWindow::activate_CreateExtrudedPolyline_Mode()
{
	if (m_surfaceController)
	{
		m_surfaceController->activate_CreateExtrudedPolyline_Mode();
	}
}
void MainWindow::activate_DeleteGeometry_Mode()
{
	if (m_surfaceController)
	{
		m_surfaceController->activate_DeleteGeometry_Mode();
	}
}
void MainWindow::exit()
{
  close();
}
