/****************************************************************************
				Copyright (c) 1998 to 2021 by Envitia Group PLC.
****************************************************************************/

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ui_qteditorinteractionmodes.h"

class MainWindow : public QMainWindow, private Ui_QtEditorInteractionModesClass
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	//! Initialize the editor mode with the editor configuration file and add it to the interaction mode manager.
	//!
	//! @param editor configuration file path.
	//!
	void initializeEditMode(const char *iniFile);

	//! Load the map file into the surface's map data layer
	//!
	//! @param map file path.
	//!
	void loadMap(const char *filename);

	//! Load the geometry tmf file into the surface's editor data layer
	//!
	//! @param geometry tmf file path.
	//!
	void loadGeometryFile(const char *filename);

	//! handle the call back method from the surface to update the GUI when reseting interaction modes.
	void resetInteractionModes();

	//! handle the call back method from the surface to update the GUI when the selected entity changes.
	//!
	//! @param number of selected entities.
	//!
	void selectionChanged(int numEntities);

private slots://handle menu and toolbar clicks
	//! Prompt the user to provide a map file path and load it into the surface's map data layer.
	void loadMap();

	//! Prompt the user to provide a tmf geometry file path and load it into the surface's editor data layer.
	void loadGeometryFile();
	//! Prompt the user to provide a tmf geometry file path to save the surface's editor data layer.
	void saveGeometryFile();
	//! clear and reset the surface's editor data layer into the provided tmf geometry file path.
	void newGeometryFile();

	//! show/hide the rendering attributes panel.
	void showRenderingAttributes();
	//! Show help dialog.
	void showAboutBox();
	//! Exit the application.
	void exit();

	//! Reset the view of the surface's map data layer.
	void resetView();
	//! Zoom in once through the surface's map data layer.
	void zoomInOnce();
	//! Zoom out once through the surface's map data layer.
	void zoomOutOnce();

	//! Activate the pan interaction mode.
	void activatePanMode();
	//! Activate the grab interaction mode.
	void activateGrabMode();
	//! Activate the zoom interaction mode.
	void activateZoomMode();
	//! Activate the magnifier interaction mode.
	void activateMagnifierMode();
	//! Activate the edit interaction mode.
	void activateEditMode();

	//! Activate the editor's selectall interaction mode.
	void activateSelectAllMode();
	//! Activate the editor's selectbyextent interaction mode.
	void activateSelectByRectangleMode();
	//! Activate the editor's selectbypolygon interaction mode.
	void activateSelectByPolygonMode();

	//! Activate the editor's undo interaction mode.
	void activateTransformationUndo();
	//! Activate the editor's redo interaction mode.
	void activateTransformationRedo();
	//! Activate the editor's clear interaction mode.
	void activateTransformationClear();

	//! Activate the editor's polygon interaction mode.
	void activateCreatePolygonMode();
	//! Activate the editor's polyline interaction mode.
	void activateCreatePolylineMode();
	//! Activate the editor's symbol interaction mode.
	void activateCreateMarkerMode();
	//! Activate the editor's circle interaction mode.
	void activateCreateCircleMode();
	//! Activate the editor's text interaction mode.
	void activateCreateTextMode();

	//! Activate the editor's transformation copy interaction mode.
	void activateTransformationCopyMode();
	//! Activate the editor's transformation move interaction mode.
	void activateTransformationMoveMode();
	//! Activate the editor's transformation scale interaction mode.
	void activateTransformationScaleMode();
	//! Activate the editor's transformation rotate interaction mode.
	void activateTransformationRotateMode();
	//! Activate the editor's transformation changetext interaction mode.
	void activateTransformationChangeTextMode();
	//! Activate the editor's transformation addpoint interaction mode.
	void activateTransformationAddPointMode();
	//! Activate the editor's transformation deletepoint interaction mode.
	void activateTransformationDeletePointMode();
	//! Activate the editor's transformation movepoint interaction mode.
	void activateTransformationMovePointMode();
	//! Activate the editor's transformation front interaction mode.
	void activateTransformationFrontMode();
	//! Activate the editor's transformation back interaction mode.
	void activateTransformationBackMode();
	//! Activate the editor's transformation group interaction mode.
	void activateTransformationGroupMode();
	//! Activate the editor's transformation ungroup interaction mode.
	void activateTransformationUngroupMode();
	//! Activate the editor's transformation delete interaction mode.
	void activateTransformationDeleteMode();

	//! Activate the editor's constraint equal interaction mode.
	void activateConstraintEqual();
	//! Activate the editor's constraint unequal interaction mode.
	void activateConstraintUnequal();
	//! Activate the editor's constraint horizontal interaction mode.
	void activateConstraintHorizontal();
	//! Activate the editor's constraint vertical interaction mode.
	void activateConstraintVertical();
	//! Activate the editor's constraint angular interaction mode.
	void activateConstraintAngular();
	//! Activate the editor's constraint distance interaction mode.
	void activateConstraintDistance();

private:
	//! interaction modes toolbar group
	QActionGroup *m_interactionModesGroup;
	//! editor transformation modes toolbar group
	QActionGroup *m_transformationModesGroup;
	//! editor constraint modes toolbar group
	QActionGroup *m_constraintModesGroup;
};

#endif
