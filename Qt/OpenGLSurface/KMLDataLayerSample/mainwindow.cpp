/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#include <QtGui>
#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"

#include <string>
using namespace std;

// This class is the main window of the application. It receives events from the user and
// passes them to the widget containing the drawing surface

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  // Construct the window
  setupUi(this);

  // Connect the actions for the toolbars and menus to the slots that deal with them
  connect(action_Open, SIGNAL(triggered()), this, SLOT(loadMap()));
  connect(action_OpenKML, SIGNAL(triggered()), this, SLOT(loadKML()));
  connect(actionReset, SIGNAL(triggered()), this, SLOT(resetView()));
  connect(actionZoom_In, SIGNAL(triggered()), this, SLOT(zoomInOnce()));
  connect(actionZoom_Out, SIGNAL(triggered()), this, SLOT(zoomOutOnce()));
  connect(actionZoom_Mode, SIGNAL(triggered()), this, SLOT(activateZoomMode()));
  connect(actionPan_Mode, SIGNAL(triggered()), this, SLOT(activatePanMode()));
  connect(actionGrab_Mode, SIGNAL(triggered()), this, SLOT(activateGrabMode()));
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutBox()));
  connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

  // Create a group of actions for the interaction mode buttons and menus so that
  // the active interaction mode is reflected in the toolbar and menu display
  m_interactionModesGroup = new QActionGroup(mainToolBar);
  m_interactionModesGroup->addAction(actionZoom_Mode);
  m_interactionModesGroup->addAction(actionPan_Mode);
  m_interactionModesGroup->addAction(actionGrab_Mode);
}

MainWindow::~MainWindow()
{
  // Clean up
  delete m_interactionModesGroup;
}

void MainWindow::loadMap( const char *mapToLoad)
{
  mapLinkWidget->loadMap( mapToLoad );
}

void MainWindow::loadKML( const char *kmlToLoad)
{
  mapLinkWidget->loadKML( kmlToLoad, kmlAttributeTreeWidget );
}

void MainWindow::loadMap()
{
  // Show a file open dialog to let the user choose the map to load - 
  // MapLink Pro maps either have a '.map' or '.mpc' file ending.
  QString fileName = QFileDialog::getOpenFileName(this, tr("Load A Map"), QString(),
                                                  tr("All Map files (*.map *.mpc)"));

  if (!fileName.isEmpty())
  {
    // Tell the widget to load the new map
    loadMap( fileName.toUtf8() );
  }
}

void MainWindow::showAboutBox()
{
  // Display an about box
  QMessageBox::about(this, tr("MapLink Pro KML DataLayer Sample"),
                           tr("<img src=\":/images/envitia.png\"/>"
                              "<p>Copyright &copy; 1998-2014 Envitia Group PLC. All rights reserved.</p>"
#ifdef WIN32
                              "<p align=center style=\"color:#909090\">Portions developed using LEADTOOLS<br>"
                              "Copyright &copy; 1991-2009 LEAD Technologies, Inc. ALL RIGHTS RESERVED</p>"
#endif
                              ));
}

void MainWindow::loadKML()
{
  // Show a file open dialog to let the user choose the map to load - 
  // MapLink Pro maps either have a '.map' or '.mpc' file ending.
  QString fileName = QFileDialog::getOpenFileName(this, tr("Load KML"), QString(),
                                                  tr("All KML files (*.kml *.kmz)"));

  if (!fileName.isEmpty())
  {
    // Tell the widget to load the new map
    loadKML( fileName.toUtf8() );
  }
}

void MainWindow::resetView()
{
  // Tell the widget to reset the viewing area to its maximum extent
  mapLinkWidget->resetView();
}

void MainWindow::zoomInOnce()
{
  // Tell the widget to zoom in by a fixed percentage
  mapLinkWidget->zoomInOnce();
}

void MainWindow::zoomOutOnce()
{
  // Tell the widget to zoom out by a fixed percentage
  mapLinkWidget->zoomOutOnce();
}

void MainWindow::activatePanMode()
{
  // Tell the widget to activate the pan interaction mode
  mapLinkWidget->activatePanMode();
}

void MainWindow::activateGrabMode()
{
  // Tell the widget to activate the grab interaction mode
  mapLinkWidget->activateGrabMode();
}

void MainWindow::activateZoomMode()
{
  // Tell the widget to activate the zoom interaction mode
  mapLinkWidget->activateZoomMode();
}

void MainWindow::exit()
{
  close();
}
