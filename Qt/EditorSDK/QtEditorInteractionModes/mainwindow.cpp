/****************************************************************************
				Copyright (c) 1998 to 2021 by Envitia Group PLC.
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

// handle the call back method from the surface to update the GUI when reseting interaction modes.
void resetInteractionModes_CallBack()
{
	m_MainWindow->resetInteractionModes();
}

// handle the call back method from the surface to update the GUI when the selected entity changes.
void selectionChanged_CallBack(int numEntities)
{
	m_MainWindow->selectionChanged(numEntities);
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
	connect(actionLoad_geometry_file, SIGNAL(triggered()), this, SLOT(loadGeometryFile()));
	connect(actionSave_geometry_file, SIGNAL(triggered()), this, SLOT(saveGeometryFile()));
	connect(actionNew_geometry_file, SIGNAL(triggered()), this, SLOT(newGeometryFile()));
	connect(actionRendering_attributes, SIGNAL(triggered()), this, SLOT(showRenderingAttributes()));

	// connect interaction modes
	connect(actionReset, SIGNAL(triggered()), this, SLOT(resetView()));
	connect(actionZoom_In, SIGNAL(triggered()), this, SLOT(zoomInOnce()));
	connect(actionZoom_Out, SIGNAL(triggered()), this, SLOT(zoomOutOnce()));
	connect(actionZoom_Mode, SIGNAL(triggered()), this, SLOT(activateZoomMode()));
	connect(actionPan_Mode, SIGNAL(triggered()), this, SLOT(activatePanMode()));
	connect(actionGrab_Mode, SIGNAL(triggered()), this, SLOT(activateGrabMode()));
	connect(actionMagnifier_Mode, SIGNAL(triggered()), this, SLOT(activateMagnifierMode()));
	connect(actionEdit_Mode, SIGNAL(triggered()), this, SLOT(activateEditMode()));
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutBox()));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

	// connect editor select
	connect(actionSelect_All, SIGNAL(triggered()), this, SLOT(activateSelectAllMode()));
	connect(actionSelect_by_Rectangle, SIGNAL(triggered()), this, SLOT(activateSelectByRectangleMode()));
	connect(actionSelect_by_Polygon, SIGNAL(triggered()), this, SLOT(activateSelectByPolygonMode()));

	// connect editor primitives
	connect(actionPolygon, SIGNAL(triggered()), this, SLOT(activateCreatePolygonMode()));
	connect(actionPolyline, SIGNAL(triggered()), this, SLOT(activateCreatePolylineMode()));
	connect(actionMarker, SIGNAL(triggered()), this, SLOT(activateCreateMarkerMode()));
	connect(actionCircle, SIGNAL(triggered()), this, SLOT(activateCreateCircleMode()));
	connect(actionText, SIGNAL(triggered()), this, SLOT(activateCreateTextMode()));

	// connect editor transformation
	connect(actionCopy, SIGNAL(triggered()), this, SLOT(activateTransformationCopyMode()));
	connect(actionMove, SIGNAL(triggered()), this, SLOT(activateTransformationMoveMode()));
	connect(actionScale, SIGNAL(triggered()), this, SLOT(activateTransformationScaleMode()));
	connect(actionRotate, SIGNAL(triggered()), this, SLOT(activateTransformationRotateMode()));
	connect(actionChange_text, SIGNAL(triggered()), this, SLOT(activateTransformationChangeTextMode()));
	connect(actionAdd_point, SIGNAL(triggered()), this, SLOT(activateTransformationAddPointMode()));
	connect(actionDelete_point, SIGNAL(triggered()), this, SLOT(activateTransformationDeletePointMode()));
	connect(actionMove_point, SIGNAL(triggered()), this, SLOT(activateTransformationMovePointMode()));
	connect(actionFront, SIGNAL(triggered()), this, SLOT(activateTransformationFrontMode()));
	connect(actionBack, SIGNAL(triggered()), this, SLOT(activateTransformationBackMode()));
	connect(actionGroup, SIGNAL(triggered()), this, SLOT(activateTransformationGroupMode()));
	connect(actionUngroup, SIGNAL(triggered()), this, SLOT(activateTransformationUngroupMode()));
	connect(actionDelete, SIGNAL(triggered()), this, SLOT(activateTransformationDeleteMode()));
	connect(actionUndo, SIGNAL(triggered()), this, SLOT(activateTransformationUndo()));
	connect(actionRedo, SIGNAL(triggered()), this, SLOT(activateTransformationRedo()));
	connect(actionClear, SIGNAL(triggered()), this, SLOT(activateTransformationClear()));

	// connect editor constraint
	connect(actionEqual, SIGNAL(triggered()), this, SLOT(activateConstraintEqual()));
	connect(actionUnequal, SIGNAL(triggered()), this, SLOT(activateConstraintUnequal()));
	connect(actionHorizontal, SIGNAL(triggered()), this, SLOT(activateConstraintHorizontal()));
	connect(actionVertical, SIGNAL(triggered()), this, SLOT(activateConstraintVertical()));
	connect(actionAngular, SIGNAL(triggered()), this, SLOT(activateConstraintAngular()));
	connect(actionDistance, SIGNAL(triggered()), this, SLOT(activateConstraintDistance()));

	//! Create a group of actions for the interaction mode buttons and menus so that
	//! the active interaction mode is reflected in the toolbar and menu display
	m_interactionModesGroup = new QActionGroup(mainToolBar);
	m_interactionModesGroup->addAction(actionZoom_Mode);
	m_interactionModesGroup->addAction(actionPan_Mode);
	m_interactionModesGroup->addAction(actionGrab_Mode);
	m_interactionModesGroup->addAction(actionMagnifier_Mode);
	m_interactionModesGroup->addAction(actionEdit_Mode);
	m_interactionModesGroup->addAction(actionSelect_All);
	m_interactionModesGroup->addAction(actionSelect_by_Rectangle);
	m_interactionModesGroup->addAction(actionSelect_by_Polygon);
	m_interactionModesGroup->addAction(actionPolygon);
	m_interactionModesGroup->addAction(actionPolyline);
	m_interactionModesGroup->addAction(actionMarker);
	m_interactionModesGroup->addAction(actionCircle);
	m_interactionModesGroup->addAction(actionText);

	m_transformationModesGroup = new QActionGroup(transformationToolBar);
	m_interactionModesGroup->addAction(actionCopy);
	m_interactionModesGroup->addAction(actionMove);
	m_interactionModesGroup->addAction(actionScale);
	m_interactionModesGroup->addAction(actionRotate);
	m_interactionModesGroup->addAction(actionChange_text);
	m_interactionModesGroup->addAction(actionAdd_point);
	m_interactionModesGroup->addAction(actionDelete_point);
	m_interactionModesGroup->addAction(actionMove_point);
	m_interactionModesGroup->addAction(actionFront);
	m_interactionModesGroup->addAction(actionBack);
	m_interactionModesGroup->addAction(actionGroup);
	m_interactionModesGroup->addAction(actionUngroup);
	m_interactionModesGroup->addAction(actionDelete);

	m_constraintModesGroup = new QActionGroup(constraintToolBar);
	m_constraintModesGroup->addAction(actionEqual);
	m_constraintModesGroup->addAction(actionUnequal);
	m_constraintModesGroup->addAction(actionHorizontal);
	m_constraintModesGroup->addAction(actionVertical);
	m_constraintModesGroup->addAction(actionAngular);
	m_constraintModesGroup->addAction(actionDistance);
	actionEqual->setChecked(true);

	// setup the callback handling methods
	if (maplinkWidget)
	{
		maplinkWidget->setCursor(Qt::CrossCursor);
		maplinkWidget->ResetInteractionModesCallBack(resetInteractionModes_CallBack);
		maplinkWidget->SelectionChangedCallBack(selectionChanged_CallBack);

#ifndef WIN32
		connect(maplinkWidget, SIGNAL(mapDrawn()), this, SLOT(update()));
#endif
	}

	// hide rendering attributes toolbar if not supported
#ifndef USE_RENDERING_ATTRIBUTES_PANEL
	actionRendering_attributes->setVisible(false);
#endif
}

MainWindow::~MainWindow()
{
	//! Clean up
	delete m_interactionModesGroup;
	delete m_transformationModesGroup;
	delete m_constraintModesGroup;
}

void MainWindow::initializeEditMode(const char *iniFile)
{
	if (maplinkWidget)
	{
		maplinkWidget->initializeEditMode(iniFile);
	}
}

void MainWindow::loadMap(const char *filename)
{
	if (maplinkWidget)
	{
		maplinkWidget->loadMap(filename);
		maplinkWidget->activateEditMode();
	}
}

void MainWindow::loadGeometryFile(const char *filename)
{
	if (maplinkWidget)
	{
		maplinkWidget->loadGeometryFile(filename);
	}
}

//! set the call back to update the GUI for reseting interaction modes.
void MainWindow::resetInteractionModes()
{
	// checkbox the gui's edit mode toolbar
	actionEdit_Mode->setChecked(true);
	maplinkWidget->setCursor(Qt::ArrowCursor);
}

//! set the call back to update the GUI when the selected entity changes.
void MainWindow::selectionChanged(int numEntities)
{
	// enable/disable the transformation toolbar
	if (transformationToolBar)
	{
		bool isEnabled = transformationToolBar->isEnabled();
		if (numEntities && !isEnabled)
		{
			transformationToolBar->setEnabled(true);
		}
		else if (!numEntities && isEnabled)
		{
			transformationToolBar->setEnabled(false);
		}
	}

	// enable/disable the undo/redo
	actionUndo->setEnabled(maplinkWidget->checkTransformationUndoPossible());
	actionRedo->setEnabled(maplinkWidget->checkTransformationRedoPossible());
}

void MainWindow::loadMap()
{
	QString initDir = "";
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), initDir, tr("All Map files (*.map *.mpc)"));

	if (!fileName.isEmpty() && maplinkWidget)
	{
		loadMap(fileName.toUtf8());
	}
}

void MainWindow::loadGeometryFile()
{
	QString initDir = "";
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Geometry File"), initDir, tr("All Geometry files (*.tmf)"));

	if (!fileName.isEmpty() && maplinkWidget)
	{
		loadGeometryFile(fileName.toUtf8());
	}
}

void MainWindow::saveGeometryFile()
{
	QString initDir = "";
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Geometry File"), initDir, tr("All Geometry files (*.tmf)"));

	if (!fileName.isEmpty() && maplinkWidget)
	{
		if (maplinkWidget)
		{
			maplinkWidget->saveGeometryFile(fileName.toUtf8());
		}
	}
}

void MainWindow::newGeometryFile()
{
	// ask the user to confirm the clearing
	QMessageBox::StandardButton reply = QMessageBox::question(this, "Clear confirmation", "Clear will remove all the entities. do you want to continue?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No)
		return;

	if (maplinkWidget)
	{
		maplinkWidget->newGeometryFile();
	}
}

void MainWindow::showRenderingAttributes()
{
	if (maplinkWidget)
	{
		maplinkWidget->showRenderingAttributes();
	}
}

void MainWindow::showAboutBox()
{
	//! Display an about box
	QMessageBox::about(this, tr("MapLink Pro Qt Editor Interaction modes Sample"),
		tr("<img src=\":/images/envitia.png\"/>"
			"<p>Copyright &copy; 1998-2017 Envitia Group PLC. All rights reserved.</p>"
#ifdef WIN32
			"<p align=center style=\"color:#909090\">Portions developed using LEADTOOLS<br>"
			"Copyright &copy; 1991-2009 LEAD Technologies, Inc. ALL RIGHTS RESERVED</p>"
#endif
		));
}

void MainWindow::exit()
{
	close();
}

//////////
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

//////////
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

void MainWindow::activateMagnifierMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the magnifier interaction mode
		maplinkWidget->activateMagnifierMode();
		maplinkWidget->setCursor(Qt::OpenHandCursor);
	}
}

void MainWindow::activateEditMode()
{
	if (maplinkWidget)
	{
		maplinkWidget->resetEditMode();
		//! Tell the widget to activate the edit interaction mode.
		maplinkWidget->activateEditMode();
		maplinkWidget->setCursor(Qt::ArrowCursor);
	}
}

//////////
void MainWindow::activateSelectAllMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateSelectAllMode();
		maplinkWidget->setCursor(Qt::CrossCursor);
	}
}

void MainWindow::activateSelectByRectangleMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateSelectByRectangleMode();
		maplinkWidget->setCursor(Qt::CrossCursor);
	}
}

void MainWindow::activateSelectByPolygonMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateSelectByPolygonMode();
		maplinkWidget->setCursor(Qt::CrossCursor);
	}
}

////////////
void MainWindow::activateTransformationUndo()
{
	if (maplinkWidget)
	{
		maplinkWidget->activateTransformationUndo();

		// enable/disable the undo/redo
		actionUndo->setEnabled(maplinkWidget->checkTransformationUndoPossible());
		actionRedo->setEnabled(maplinkWidget->checkTransformationRedoPossible());
	}
}

void MainWindow::activateTransformationRedo()
{
	if (maplinkWidget)
	{
		maplinkWidget->activateTransformationRedo();

		// enable/disable the undo/redo
		actionUndo->setEnabled(maplinkWidget->checkTransformationUndoPossible());
		actionRedo->setEnabled(maplinkWidget->checkTransformationRedoPossible());
	}
}

void MainWindow::activateTransformationClear()
{
	// ask the user to confirm the clearing
	QMessageBox::StandardButton reply = QMessageBox::question(this, "Clear confirmation", "Clear will remove all the entities. do you want to continue?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No)
		return;

	if (maplinkWidget)
	{
		maplinkWidget->activateTransformationClear();

		//! Tell the widget to activate the edit interaction mode.
		maplinkWidget->activateEditMode();
		resetInteractionModes();
	}
}
//////////
void MainWindow::activateCreatePolygonMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateCreatePolygonMode();
		maplinkWidget->setCursor(Qt::CrossCursor);
	}
}

void MainWindow::activateCreatePolylineMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateCreatePolylineMode();
		maplinkWidget->setCursor(Qt::CrossCursor);
	}
}

void MainWindow::activateCreateMarkerMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateCreateMarkerMode();
		maplinkWidget->setCursor(Qt::CrossCursor);
	}
}

void MainWindow::activateCreateCircleMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateCreateCircleMode();
		maplinkWidget->setCursor(Qt::CrossCursor);
	}
}

void MainWindow::activateCreateTextMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateCreateTextMode();
		maplinkWidget->setCursor(Qt::CrossCursor);
	}
}

///////////////////////////////////
void MainWindow::activateTransformationCopyMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationCopyMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateTransformationMoveMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationMoveMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}

void MainWindow::activateTransformationScaleMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationScaleMode();
		maplinkWidget->setCursor(Qt::SizeAllCursor);
	}
}
void MainWindow::activateTransformationRotateMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationRotateMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateTransformationChangeTextMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationChangeTextMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateTransformationAddPointMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationAddPointMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateTransformationDeletePointMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationDeletePointMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateTransformationMovePointMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationMovePointMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateTransformationFrontMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationFrontMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateTransformationBackMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationBackMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateTransformationGroupMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationGroupMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateTransformationUngroupMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationUngroupMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateTransformationDeleteMode()
{
	if (maplinkWidget)
	{
		//! Tell the widget to activate the edit interaction mode and activate the proper editor mode
		maplinkWidget->activateEditMode();

		maplinkWidget->activateTransformationDeleteMode();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}

//////////////////////////////////////////////////////////
void MainWindow::activateConstraintEqual()
{
	if (maplinkWidget)
	{
		maplinkWidget->activateConstraintEqual();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateConstraintUnequal()
{
	if (maplinkWidget)
	{
		maplinkWidget->activateConstraintUnequal();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateConstraintHorizontal()
{
	if (maplinkWidget)
	{
		maplinkWidget->activateConstraintHorizontal();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateConstraintVertical()
{
	if (maplinkWidget)
	{
		maplinkWidget->activateConstraintVertical();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateConstraintAngular()
{
	if (maplinkWidget)
	{
		maplinkWidget->activateConstraintAngular();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}
void MainWindow::activateConstraintDistance()
{
	if (maplinkWidget)
	{
		maplinkWidget->activateConstraintDistance();
		maplinkWidget->setCursor(Qt::DragMoveCursor);
	}
}