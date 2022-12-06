/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <string>
using namespace std;

#include "mainwindow.h"

#include "MapLink.h"

// ************************************************************
// This class represents the main application window. It handles forwarding the appropriate
// UI events to the contained MapLink widget.
// ************************************************************


MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
 : QMainWindow( parent, flags )
{
  setupUi( this );

  // Connect up the signals for the toolbar buttons and menu items to the appropriate slots. These slot functions
  // will be invoked when the UI item is activated.
  connect( actionOpen_Map, SIGNAL( triggered() ), this, SLOT( openMap() ) );
  connect( actionOpen_Terrain, SIGNAL( triggered() ), this, SLOT( openTerrain() ) );
  connect( actionZoomIn, SIGNAL( triggered() ), this, SLOT( zoomIn() ) );
  connect( actionZoomOut, SIGNAL( triggered() ), this, SLOT( zoomOut() ) );
  connect( actionResetView, SIGNAL( triggered() ), this, SLOT( resetView() ) );
  connect( actionActivateEyePointMode, SIGNAL( triggered() ), this, SLOT( activateEyePointInteractionMode() ) );
  connect( actionActivateWorldMode, SIGNAL( triggered() ), this, SLOT( activateWorldInteractionMode() ) );
  connect( actionWireframe, SIGNAL( triggered() ), this, SLOT( toggleWireframe() ) );
  connect( actionExaggerate_Terrain, SIGNAL( triggered() ), this, SLOT( toggleTerrainExaggeration() ) );
  connect( actionLimit_Camera, SIGNAL( triggered() ), this, SLOT( toggleCameraAltitudeLimit() ) );
}

MainWindow::~MainWindow()
{
}

void MainWindow::openMap()
{
  // Bring up the standard file open dialog to let the user tell us what map they would like to load
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), QString(), tr("All Map files (*.map *.mpc)"));

  if (!fileName.isEmpty())
  {
    // Tell the widget about the map the user wants to display, the widget handles the loading of this for us
    if( !mapLinkWidget->loadMap( fileName.toUtf8() ) )
    {
      // The map couldn't be loaded. There may be information on the error stack that would explain why, so display
      // this as part of the message
      const char *errorMsg = TSLErrorStack::errorString( "Failed to load map\n" ) ;
      QMessageBox::information(this, tr("Failed to load map"),
                               errorMsg ? QString::fromUtf8(errorMsg) : tr("Cannot load map."), QMessageBox::Cancel);
      
      // Clear the error stack to avoid reporting the same error again
      TSLErrorStack::clear();
    }
  }
}

void MainWindow::openTerrain()
{
  // Bring up the standard file open dialog to let the user tell us what terrain database they would like to load
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Terrain Database"), QString(), tr("All Terrain Database files (*.tdf)"));

  if (!fileName.isEmpty())
  {
    // Tell the widget about the terrain database the user wants to use, the widget handles the loading of this for us
    if( !mapLinkWidget->loadTerrain( fileName.toUtf8() ) )
    {
      // The database couldn't be loaded. There may be information on the error stack that would explain why, so display
      // this as part of the message
      const char *errorMsg = TSLErrorStack::errorString( "Failed to load terrain database\n" ) ;
      QMessageBox::information(this, tr("Failed to load terrain database"),
             errorMsg ? QString::fromUtf8(errorMsg) : tr("Cannot load terrain database."), QMessageBox::Cancel);

      // Clear the error stack to avoid reporting the same error again
      TSLErrorStack::clear();
    }
  }
}

void MainWindow::zoomIn()
{
  // The zoom in button was pressed, forward this on to the MapLink widget
  mapLinkWidget->zoomIn();
}

void MainWindow::zoomOut()
{
  // The zoom out button was pressed, forward this on to the MapLink widget
  mapLinkWidget->zoomOut();
}

void MainWindow::resetView()
{
  // The view reset button was pressed, forward this on to the MapLink widget
  mapLinkWidget->resetView();
}

void MainWindow::activateEyePointInteractionMode()
{
  // Tell the widget to switch to using the TSL3DInteractionModeTrackballEyepoint interaction mode
  if( actionActivateWorldMode->isChecked() )
  {
    mapLinkWidget->activateEyePointInteractionMode();

    // Ask the widget what text we should show in the status bar - this should be the help for the
    // interaction mode we just activated
    const char *statusBarText = mapLinkWidget->statusBarText();
    if( statusBarText )
    {
      statusbar->showMessage( QString::fromUtf8(statusBarText) );
    }
    else
    {
      statusbar->showMessage( "" );
    }

    actionActivateWorldMode->setChecked( false );
  }
}

void MainWindow::activateWorldInteractionMode()
{
  // Tell the widget to switch to using the TSL3DInteractionModeTrackballWorld interaction mode
  if( actionActivateEyePointMode->isChecked() )
  {
    mapLinkWidget->activateWorldInteractionMode();

    // Ask the widget what text we should show in the status bar - this should be the help for the
    // interaction mode we just activated
    const char *statusBarText = mapLinkWidget->statusBarText();
    if( statusBarText )
    {
      statusbar->showMessage( QString::fromUtf8(statusBarText) );
    }
    else
    {
      statusbar->showMessage( "" );
    }

    actionActivateEyePointMode->setChecked( false );
  }
}

void MainWindow::toggleWireframe()
{
  // Tell the widget to toggle wireframe mode
  mapLinkWidget->setWireframeMode( actionWireframe->isChecked() );
}

void MainWindow::toggleTerrainExaggeration()
{
  // Tell the widget to toggle using terrain exaggeration
  mapLinkWidget->setTerrainExaggeration( actionExaggerate_Terrain->isChecked() );
}

void MainWindow::toggleCameraAltitudeLimit()
{
  // Tell the widget to toggle prevention of the camera going below the terrain
  mapLinkWidget->setCameraAltitudeLimit( actionLimit_Camera->isChecked() );
}

