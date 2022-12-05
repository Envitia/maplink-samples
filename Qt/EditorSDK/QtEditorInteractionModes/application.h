/****************************************************************************
				Copyright (c) 1998 to 2021 by Envitia Group PLC.
****************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#ifndef WINNT
# include <X11/Xlib.h>
#else
#endif

/////////////////////////////////////////////////////////////////////
//! Include MapLink Pro Headers...
//
//! Define some required Macros and include X11 and Win32 headers as
//! necessary.
//
//! Define: TTLDLL & WIN32 within the project make settings.
//
/////////////////////////////////////////////////////////////////////
#if defined(WIN32) || defined(_WIN64) || defined(Q_WS_WIN)
# ifndef WINVER
#  define WINVER 0x502
# endif
# ifndef VC_EXTRALEAN
#  define VC_EXTRALEAN		//! Exclude rarely-used stuff from Windows headers
# endif
# ifndef WIN32
#  define WIN32  1
# endif
# include <windows.h>
#endif

#include "MapLink.h"
#include "MapLinkIMode.h"
#include "MapLinkEDT.h"
#include "MapLinkEDTIMode.h"

#ifdef USE_RENDERING_ATTRIBUTES_PANEL
#include "tslrenderingattributepanel.h"
#endif

class TSLRenderingAttributePanel;//< forward declaration

//! call back method from the surface to update the GUI when reseting interaction modes.
typedef void(*resetInteractionModesCallBack)();

//! call back method from the surface to update the GUI when the selected entity changes.
typedef void(*selectionChangedCallBack)(int numEntities);


////////////////////////////////////////////////////////////////
//! Main Application class.
//
//! Contains the calls to MapLink and the simple application
//! code.
////////////////////////////////////////////////////////////////
class Application : public TSLInteractionModeRequest, TSLInteractionModeEditRequest, TSLWinContextHandler
{
public:
	Application(QWidget *parent);
	virtual ~Application();

	//! Creates the MapLink drawing surface and associated map data layer
	void create();

	//! Called when the size of the window has changed
	void resize(int width, int height);

	//! Called to redraw the map
	void redraw();

	//! Mouse and Keyboard events - if the method returns true it indicates that the widget needs to redraw
	bool mouseMoveEvent(unsigned int button, bool shiftPressed, bool controlPressed, int X, int Y);
	bool OnLButtonDown(bool shiftPressed, bool controlPressed, int X, int Y);
	bool OnMButtonDown(bool shiftPressed, bool controlPressed, int X, int Y);
	bool OnRButtonDown(bool shiftPressed, bool controlPressed, int X, int Y);
	bool OnLButtonUp(bool shiftPressed, bool controlPressed, int X, int Y);
	bool OnMButtonUp(bool shiftPressed, bool controlPressed, int X, int Y);
	bool OnRButtonUp(bool shiftPressed, bool controlPressed, int X, int Y);
	bool OnKeyPress(bool shiftPressed, bool controlPressed, int keySym);
	bool OnMouseWheel(bool shiftPressed, bool controlPressed, short zDelta, int X, int Y);

	//! set the map's background colour
	void setMapBackgroundColour();

	//! information to enable Drawing surface to draw.
#ifndef WINNT
	void drawingInfo(Drawable drawable, Display *display, Screen *screen, Colormap colourmap, Visual *visual);
#else
	void drawingInfo(WId window);
#endif

	//! set the call back method from the surface to update the GUI when the selected entity changes.
	//!
	//! @param call back method.
	//!
	void SelectionChangedCallBack(selectionChangedCallBack func);

	//! set the call back method from the surface to update the GUI when reseting interaction modes.
	//!
	//! @param call back method.
	//!
	void ResetInteractionModesCallBack(resetInteractionModesCallBack func);

	//! Interaction Mode request implementations.
	virtual void resetMode(TSLInteractionMode * mode, TSLButtonType button, TSLDeviceUnits xDU, TSLDeviceUnits yDU);
	virtual void viewChanged(TSLDrawingSurface* drawingSurface);

public://editor sdk
	//! 
	//! Initialize the editor mode with the editor configuration file and add it to the interaction mode manager.
	//!
	//! @param editor configuration file path.
	//!
	void initializeEditMode(const char *iniFile);

	//! reset edit mode from its primitives mode.
	void resetEditMode();

	//! check if undo operation is available.
	//!
	//! @return true if undo operation can be performed. False otherwise.
	bool checkTransformationUndoPossible();

	//! check if redo operation is available.
	//!
	//! @return true if redo operation can be performed. False otherwise.
	bool checkTransformationRedoPossible();

public://editor sdk call backs
	//!
	//! This method is triggered by the Windows-style selection mechanism when a
	//! context menu may be invoked.
	//!
	//!
	//! @param editor  Pointer to the current TSLEditor instance.
	//!
	//! @param xDU  X position, in Device Units where the invoking button press occurred
	//!
	//! @param yDU  Y position, in Device Units, where the invoking button press occurred.
	virtual void invokeContextMenu(TSLEditor* editor, TSLDeviceUnits xDU, TSLDeviceUnits yDU);


	//!
	//! Called to indicate a change in the selection list.
	//!
	//! This method is called by the editor to inform the user's application of the
	//! current select list status.
	//!
	//!
	//! @param numEntities The number of selected entities.
	//!
	//! @param depth The select depth.
	//!
	//! @return true if successful, otherwise false.
	bool onSelectionChanged(int numEntities, int depth);

	//!
	//! Request user input from a dialog box.
	//!
	//! This method requests that the user select one of the specified choices. An
	//! empty string indicates that the button should not be shown.
	//!
	//!
	//! @param prompt The prompt requesting a user selection.
	//!
	//! @param label1 The text label for button 1.
	//!
	//! @param label2 The text label for button 2.
	//!
	//! @param label3 The text label for button 3.
	//!
	//! @param label4 The text label for button 4.
	//!
	//! @return true on success, otherwise false.
	bool requestDialog(const char* prompt, const char* label1, const char* label2, const char* label3, const char* label4);

	//!
	//! Request text input from the user.
	//!
	//! This method is called to request that the client application ask the user to
	//! enter some text.
	//!
	//! The client application should called the textEntered method when the text
	//! has been entered by the user.
	//!
	//! prompt: A pointer to the string prompting the user for input.
	//!
	//! initialValue: A pointer to the initial value of the text field.
	//!
	//! Returns true on success, otherwise false.
	bool requestText(const char* prompt, const char* initialValue);

	//!
	//! Called to indicate that the current operation has been deactivated.
	//!
	//! This method is called by the editor to inform the user's application that the
	//! current operation has been deactivated and that the default select operation
	//! is now active.
	//!
	//! Returns true if successful, otherwise false.
	bool onOperationDeactivated();

	//	Display an error message.
	//
	//	This method is called by the editor to request that the users application
	//	display an error message.
	//
	//	Argument list description:
	//
	//	message: A string containing the error message.
	//
	//	Return true if successful, otherwise false.
	bool displayError(const char* message);

	//! handle the callback to be triggered when attributes change.
	static void attributePanelCallback(void * arg, TSLRenderingAttributes * attribs);

public://handle menu and toolbar clicks

	//! 
	//! Load the map file into the surface's map data layer
	//!
	//! @param map file path.
	//!
	bool loadMap(const char *mapFilename);

	//! 
	//! Load the geometry tmf file into the surface's editor data layer
	//!
	//! @param geometry tmf file path.
	//!
	bool loadGeometryFile(const char *tmfFilename);

	//! save the surface's editor data layer into the provided tmf geometry file path.
	//!
	//! @param geometry tmf file path.
	//!
	bool saveGeometryFile(const char *tmfFilename);

	//! clear and reset the surface's editor data layer into the provided tmf geometry file path.
	void newGeometryFile();

	//! show/hide the rendering attributes panel.
	void OnOperationsAttributes();

	//! Interaction Mode control - if the method returns true it indicates that the widget needs to redraw
	//! Zoom in once through the surface's map data layer.
	bool zoomIn();
	//! Zoom out once through the surface's map data layer.
	bool zoomOut();
	//! Reset the view of the surface's map data layer.
	void resetView();

	//! Activate the pan interaction mode.
	void activatePanMode();
	//! Activate the zoom interaction mode.
	void activateZoomMode();
	//! Activate the grab interaction mode.
	void activateGrabMode();
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
	//! initialize the initial default rendering attributes (read from ini file or use default values)
	void setDefaultRenderingAttributes(TSLRenderingAttributes& attributes);

	//! get the current rendering attributes from the editor
	std::string getRenderingAttributes(int numEntities, TSLRenderingAttributes& attributes);

	// query select list (if we need to show the selected geometry in the statusbar)
	int querySelectList(int & depth);

	// query mode manager's prompt (if we need to show the mode manager instructions in the statusbar)
	const char * queryPrompt();

private:
	//! The data layer containing the map
	TSLMapDataLayer * m_mapDataLayer;

	//! Name of my map layer
	static const char * m_mapLayerName;

#ifndef WINNT
	//! The MapLink drawing surface
	TSLMotifSurface *m_drawingSurface;

	//! The display connection and screen to use
	Display *m_display;
	Drawable m_drawable;
	Screen *m_screen;
	Colormap m_colourmap;
	Visual *m_visual;
#else

	//! The MapLink drawing surface
	TSLNTSurface *m_drawingSurface;

	//! The window to draw to
	WId m_window;
#endif

	//! Interaction manager - this handles panning and zooming around the map
	//! based on the active interaction mode
#ifdef WINNT
	TSLInteractionModeManagerNT *m_modeManager;
#else
	TSLInteractionModeManagerX11 *m_modeManager;
#endif

	//! call back to update the GUI for reseting interaction modes.
	resetInteractionModesCallBack m_resetInteractionModesCallBack;

	//! The size of the window the drawing surface is attached to
	int m_widgetWidth;
	int m_widgetHeight;

	//! Rotation of the drawing surface in radians
	double m_surfaceRotation;

	//! flag set if the window is not created
	bool m_tobeCreated;

	//! parent widget
	QWidget *m_parentWidget;

private://editor sdk
  // The standard data layer for editing
	TSLStandardDataLayer * m_editDataLayer;

	// Name of my rfiy layer
	static const char * m_editLayerName;

	// Edit Mode
	TSLInteractionModeEdit * m_editMode;

	// Editor configuration file path
	std::string m_iniFile;

	// Flag to say whether to initialise the editor rendering
	bool m_initialiseRendering;

	// Current selection
	int m_numEntities;
	int m_selDepth;

	// constraints params (should be editted through the gui)
	double	m_angularValue;
	double	m_distanceValue;

	//! call back to update the GUI when the selected entity changes.
	selectionChangedCallBack m_selectionChangedCallBack;

#ifdef USE_RENDERING_ATTRIBUTES_PANEL
	// Rendering Attribute Panel
	TSLRenderingAttributePanel * m_attributePanel;
#endif
};

#endif

