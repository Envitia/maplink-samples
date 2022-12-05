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

//! point of the current main window to be used with static methods.
static MainWindow* m_MainWindow;

//! static call back method
void resetInteractionModes_CallBack()
{
  m_MainWindow->resetInteractionModes();
}

//! This class is the main window of the application. It receives events from the user and
//! passes them to the widget containing the drawing surface
MainWindow::MainWindow(QWidget *parent) 
  : QMainWindow(parent) 
{
  m_MainWindow = this;

  //! Construct the window
  setupUi(this);

  //! Connect the actions for the toolbars and menus to the slots that deal with them
  connect(action_Open, SIGNAL(triggered()), this, SLOT(loadMap()));
  connect(actionReset, SIGNAL(triggered()), this, SLOT(resetView()));
  connect(actionZoom_In, SIGNAL(triggered()), this, SLOT(zoomInOnce()));
  connect(actionZoom_Out, SIGNAL(triggered()), this, SLOT(zoomOutOnce()));
  connect(actionZoom_Mode, SIGNAL(triggered()), this, SLOT(activateZoomMode()));
  connect(actionPan_Mode, SIGNAL(triggered()), this, SLOT(activatePanMode()));
  connect(actionGrab_Mode, SIGNAL(triggered()), this, SLOT(activateGrabMode()));
  connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutBox()));
  connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

  //! Create a group of actions for the interaction mode buttons and menus so that
  //! the active interaction mode is reflected in the toolbar and menu display
  m_interactionModesGroup = new QActionGroup(mainToolBar);
  m_interactionModesGroup->addAction(actionZoom_Mode);
  m_interactionModesGroup->addAction(actionPan_Mode);
  m_interactionModesGroup->addAction(actionGrab_Mode);

  if (maplinkWidget)
  {
    maplinkWidget->setCursor(Qt::CrossCursor);
    maplinkWidget->ResetInteractionModesCallBack(resetInteractionModes_CallBack);
  }
}  

MainWindow::~MainWindow()
{
  //! Clean up
  delete m_interactionModesGroup;
}

void MainWindow::loadMap(const char *filename)
{
  if (maplinkWidget)
  {
    maplinkWidget->loadMap(filename);
  }
}

void MainWindow::loadMap()
{
  QString initDir = "";
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), initDir, tr("All Map files (*.map *.mpc)"));

  if (!fileName.isEmpty() && maplinkWidget)
  {
    maplinkWidget->loadMap( fileName.toUtf8() );
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
  if (maplinkWidget)
  {
    //! Tell the widget to reset the viewing area to its maximum extent
    maplinkWidget->resetView();
  }
}

void MainWindow::zoomInOnce()
{
  if (maplinkWidget)
  {
    //! Tell the widget to zoom in by a fixed percentage
    maplinkWidget->zoomInOnce();
  }
}

void MainWindow::zoomOutOnce()
{
  if (maplinkWidget)
  {
    //! Tell the widget to zoom out by a fixed percentage
    maplinkWidget->zoomOutOnce(); 
  }
}

void MainWindow::activateZoomMode()
{
  if (maplinkWidget)
  {
    //! Tell the widget to activate the zoom interaction mode
    maplinkWidget->activateZoomMode();
    maplinkWidget->setCursor(Qt::CrossCursor);
  }
}

void MainWindow::activatePanMode()
{
  if (maplinkWidget)
  {
    //! Tell the widget to activate the pan interaction mode
    maplinkWidget->activatePanMode();
    maplinkWidget->setCursor(Qt::SizeAllCursor);
  }
}

void MainWindow::activateGrabMode()
{
  if (maplinkWidget)
  {
    //! Tell the widget to activate the grab interaction mode
    maplinkWidget->activateGrabMode();
    maplinkWidget->setCursor(Qt::OpenHandCursor);
  }
}

void MainWindow::exit()
{
  close();
}

//! set the call back to update the GUI for reseting interaction modes.
void MainWindow::resetInteractionModes()
{
  actionZoom_Mode->setChecked(true);
  maplinkWidget->setCursor(Qt::CrossCursor);
}