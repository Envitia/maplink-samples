#include <QFileDialog>
#include <QIntValidator>
#include "surfacecontroller.h"
#include "ui_surfacecontroller.h"
#include <sstream>
#include <stdio.h>
#include <iostream>

//! point of the current main window to be used with static methods.
static SurfaceController* m_surfaceContrllerWindow;

//! static call back method
void resetInteractionModes_CallBack()
{
	m_surfaceContrllerWindow->resetInteractionModes();
}
void viewChanged_CallBack()
{
	m_surfaceContrllerWindow->handleViewChange();
}
void selectedTrack_CallBack(envitia::maplink::earth::Track * track, uint16_t id)
{
	m_surfaceContrllerWindow->selectedTrack(track, id);
}

SurfaceController::SurfaceController(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SurfaceController)
{
	m_surfaceContrllerWindow = this;
	ui->setupUi(this);

	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->setCursor(Qt::OpenHandCursor);
		ui->maplinkWidget->ResetInteractionModesCallBack(resetInteractionModes_CallBack);
		ui->maplinkWidget->ViewChangedCallBack(viewChanged_CallBack);
		ui->maplinkWidget->SelectedTrackCallBack(selectedTrack_CallBack);

		ui->cameracontrollerwidget->attachMapLinkWidget(ui->maplinkWidget);
	}
}

SurfaceController::~SurfaceController()
{
	delete ui;
}

void SurfaceController::loadLayer(const char *filename)
{
	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->loadLayer(filename);
	}
}

void SurfaceController::loadLayer()
{
	QString initDir = "";
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load layer"), initDir, tr("All Map files (*.map *.mpc *.tdf)"));

	if (!fileName.isEmpty() && ui->maplinkWidget)
	{
		loadLayer(fileName.toUtf8());
	}
}

void SurfaceController::resetView()
{
	if (ui->maplinkWidget)
	{
		//! Tell the widget to reset the viewing area to its maximum extent
		ui->maplinkWidget->resetView();

		// resetview in the dialog gui
		if (ui->cameracontrollerwidget)
			ui->cameracontrollerwidget->resetView();

		//re-activate interaction mode
		if (ui->maplinkWidget->currentInteractionMode() == ID_TOOLS_Trackball)
			ui->maplinkWidget->interaction(ID_TOOLS_Trackball, true);
	}
}

void SurfaceController::fullScreen()
{
	if (ui->cameracontrollerwidget)
	{
		ui->cameracontrollerwidget->setVisible(!ui->cameracontrollerwidget->isVisible());
	}
}

void SurfaceController::activate_Trackball_Mode()
{
	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->activate_Trackball_Mode();
		ui->maplinkWidget->setCursor(Qt::OpenHandCursor);
	}
}
void SurfaceController::activate_Select_Mode()
{
	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->activate_Select_Mode();
		ui->maplinkWidget->setCursor(Qt::SizeAllCursor);
	}
}
void SurfaceController::activate_CreatePolygon_Mode()
{
	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->activate_CreatePolygon_Mode();
		ui->maplinkWidget->setCursor(Qt::CrossCursor);
	}
}
void SurfaceController::activate_CreatePolyline_Mode()
{
	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->activate_CreatePolyline_Mode();
		ui->maplinkWidget->setCursor(Qt::CrossCursor);
	}
}
void SurfaceController::activate_CreateText_Mode()
{
	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->activate_CreateText_Mode();
		ui->maplinkWidget->setCursor(Qt::CrossCursor);
	}
}
void SurfaceController::activate_CreateSymbol_Mode()
{
	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->activate_CreateSymbol_Mode();
		ui->maplinkWidget->setCursor(Qt::CrossCursor);
	}
}
void SurfaceController::activate_CreateExtrudedPolygon_Mode()
{
	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->activate_CreateExtrudedPolygon_Mode();
		ui->maplinkWidget->setCursor(Qt::CrossCursor);
	}
}
void SurfaceController::activate_CreateExtrudedPolyline_Mode()
{
	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->activate_CreateExtrudedPolyline_Mode();
		ui->maplinkWidget->setCursor(Qt::CrossCursor);
	}
}
void SurfaceController::activate_DeleteGeometry_Mode()
{
	if (ui->maplinkWidget)
	{
		ui->maplinkWidget->activate_DeleteGeometry_Mode();
		ui->maplinkWidget->setCursor(Qt::SizeAllCursor);
	}
}

//! set the call back to update the GUI for reseting interaction modes.
void SurfaceController::resetInteractionModes()
{
	ui->maplinkWidget->setCursor(Qt::CrossCursor);
}

void SurfaceController::handleViewChange()
{
	if (ui->cameracontrollerwidget)
		ui->cameracontrollerwidget->handleViewChange();
}

void SurfaceController::selectedTrack(envitia::maplink::earth::Track * track, uint16_t id)
{
	if (ui->cameracontrollerwidget)
		ui->cameracontrollerwidget->selectedTrack(track, id);
}

void SurfaceController::reactivateTrackball()
{
  //re-activate interaction mode
  if (ui->maplinkWidget->currentInteractionMode() == ID_TOOLS_Trackball)
    ui->maplinkWidget->interaction(ID_TOOLS_Trackball, true);
}
