/****************************************************************************
				Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <QtGui>
#include <QMessageBox>

#include "application.h"
#include "textentrydialog.h"

//! Interaction mode IDs - these can be any numbers
#define ID_TOOLS_ZOOM                   1
#define ID_TOOLS_PAN                    2
#define ID_TOOLS_GRAB                   3
#define ID_TOOLS_EDIT                   4
#define ID_TOOLS_MAGNIFY                5

// user unit size used for distance constraint
#define USER_UNIT_SIZE 1000.0

// number of slots to undo/redo
#define UNDO_NUM_SLOTS 10

//! The name of our map layer. This is used when adding the data layer
//! to the drawing surface and used to reference the data layer from the 
//! drawing surface
const char * Application::m_mapLayerName = "map";
const char * Application::m_editLayerName = "edit";

//! Controls how far the drawing surface rotates in one key press - this value is in radians
static const double rotationIncrement = M_PI / 360.0;

Application::Application(QWidget *parent) :
	m_mapDataLayer(NULL),
	m_editDataLayer(NULL),
	m_drawingSurface(NULL),
#ifndef WINNT
	m_display(NULL),
	m_drawable(0),
	m_screen(NULL),
	m_colourmap(0),
	m_visual(NULL),
#else
	m_window(NULL),
#endif
	m_modeManager(NULL),
	m_resetInteractionModesCallBack(NULL),
	m_selectionChangedCallBack(NULL),
	m_widgetWidth(1),
	m_widgetHeight(1),
	m_surfaceRotation(0.0),
	m_tobeCreated(true),
	m_parentWidget(parent),

	m_editMode(NULL),
	m_initialiseRendering(true),
	m_numEntities(0),
	m_selDepth(0),
	m_angularValue(0.0),
	m_distanceValue(0.0)
#ifdef USE_RENDERING_ATTRIBUTES_PANEL
	, m_attributePanel(0)
#endif
{
	//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//! Clear the error stack so that we can get the errors that occurred here.
	//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	TSLErrorStack::clear();

	//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//! Initialise the drawing surface data files.
	//! This call uses the TSLUtilityFunctions::getMapLinkHome method to determine
	//! where MapLink is currently installed.  It then proceeds to load the
	//! following files : tsllinestyles.dat, tslfillstyles.dat, tslfonts.dat, tslsymbols.dat
	//! and tslcolours.dat
	//! When deploying your application, pass in a full path to the directory containing
	//! these files.
	//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	TSLDrawingSurface::loadStandardConfig();

	//! Check for any errors that have occurred, and display them
	const char * msg = TSLErrorStack::errorString();
	if (msg)
	{
		//! If we have any errors during initialisation, display the message
		//! and exit.
		QMessageBox::information(m_parentWidget, "Initialisation Error",
			msg, QMessageBox::Cancel);
	}
}

Application::~Application()
{
	//! Clean up by destroying the map and pathlist
	if (m_mapDataLayer)
	{
		m_mapDataLayer->destroy();
		m_mapDataLayer = 0;
	}
	if (m_editDataLayer)
	{
		//m_editDataLayer->removeData();
		m_editDataLayer->destroy();
		m_editDataLayer = 0;
	}
	if (m_modeManager)
	{
		delete m_modeManager;
		m_modeManager = NULL;
	}
	if (m_drawingSurface)
	{
		delete m_drawingSurface;
		m_drawingSurface = 0;
	}

#ifdef USE_RENDERING_ATTRIBUTES_PANEL
	if (m_attributePanel)
	{
		delete m_attributePanel;
		m_attributePanel = 0;
	}
#endif

	// Beyond this point MapLink will no longer be used - clear up all static data.
	// Once this is done no MapLink functions or classes can be used.
	TSLDrawingSurface::cleanup();
}

////////////////////////////////////////////////////////////////////////
void Application::create()
{
	if (!m_tobeCreated)
	{
		return;  //! allready created all that we need
	}

	//! The first time ever, there will be no drawing surface.
	//! If we have no drawing surface, create it, otherwise remove the old layer.
	if (!m_drawingSurface)
	{
		//! Create a double buffered drawing surface, and an interaction interface
		//! to control it.
		//! Set up the initial window extents.

#ifndef WINNT
		m_drawingSurface = new TSLMotifSurface(m_display, m_screen, m_colourmap, m_drawable, 0, m_visual);
#else
		m_drawingSurface = new TSLNTSurface((void *)m_window, false);
#endif

		m_drawingSurface->setOption(TSLOptionDoubleBuffered, true);
		m_drawingSurface->setOption(TSLOptionDynamicArcSupportEnabled, true); //! could do this based on map
		m_drawingSurface->wndResize(0, 0, m_widgetWidth, m_widgetHeight, false, TSLResizeActionMaintainTopLeft);
	}

	//! Create a map data layer
	if (m_mapDataLayer == NULL)
	{
		m_mapDataLayer = new TSLMapDataLayer();

		//! add the datalayer to the drawing surface
		//! Note: any number of datalayers can be added to a surface (each has it's own name)
		m_drawingSurface->addDataLayer(m_mapDataLayer, m_mapLayerName);
		m_drawingSurface->setDataLayerProps(m_mapLayerName, TSLPropertyVisible, true);
		m_drawingSurface->setDataLayerProps(m_mapLayerName, TSLPropertyDetect, true);
		m_drawingSurface->setDataLayerProps(m_mapLayerName, TSLPropertyBuffered, true);
	}

	//! Create the edit data layer
	if (m_editDataLayer == NULL)
	{
		m_editDataLayer = new TSLStandardDataLayer;

		//! add the datalayer to the drawing surface
		//! Note: any number of datalayers can be added to a surface (each has it's own name)
		m_drawingSurface->addDataLayer(m_editDataLayer, m_editLayerName);
		m_drawingSurface->setDataLayerProps(m_editLayerName, TSLPropertyVisible, true);
		m_drawingSurface->setDataLayerProps(m_editLayerName, TSLPropertyDetect, true);
		m_drawingSurface->setDataLayerProps(m_editLayerName, TSLPropertySelect, true);

	}

	// setup mode manager
	if (m_modeManager == NULL)
	{
		//! Now create and initialse the mode manager and modes
#ifdef WINNT
		m_modeManager = new TSLInteractionModeManagerNT(this, m_drawingSurface, HWND(m_window), 5, 5, 30, true);
#else
		m_modeManager = new TSLInteractionModeManagerX11(this, m_drawingSurface, m_display, m_screen, m_drawable, m_colourmap);
#endif

		//! Add the three interaction mode types to the manager - the zoom mode is the default
		m_modeManager->addMode(new TSLInteractionModeZoom(ID_TOOLS_ZOOM), true);
		m_modeManager->addMode(new TSLInteractionModePan(ID_TOOLS_PAN), false);
		m_modeManager->addMode(new TSLInteractionModeGrab(ID_TOOLS_GRAB), false);
		m_modeManager->addMode(new TSLInteractionModeMagnify(ID_TOOLS_MAGNIFY), false);

		m_modeManager->setCurrentMode(ID_TOOLS_ZOOM);
	}

	//! and reset the current view to display the entire map.
	m_drawingSurface->reset();

	//! Display any errors that have occurred
	const char * msg = TSLErrorStack::errorString("Cannot initialise view\n");
	if (msg)
	{
		QMessageBox::critical(m_parentWidget, "Cannot initialise view", QString::fromUtf8(msg));
	}

	m_tobeCreated = false;
}

void Application::redraw()
{
	if (m_drawingSurface)
	{
		//! Draw the map to the widget
		m_drawingSurface->drawDU(0, 0, m_widgetWidth, m_widgetHeight, true);

		//! Don't forget to draw any echo rectangle that may be active.
		if (m_modeManager)
		{
			m_modeManager->onDraw(0, 0, m_widgetWidth, m_widgetHeight);
		}
	}
}

void Application::resize(int width, int height)
{
	if (m_drawingSurface)
	{
		//! Inform the drawing surface of the new window size,
		//! attempting to keep the top left corner the same.
		//! Do not ask for an automatic redraw since we will get a call to redraw() to do so
		m_drawingSurface->wndResize(0, 0, width, height, false, TSLResizeActionMaintainTopLeft);
	}
	if (m_modeManager)
	{
		m_modeManager->onSize(width, height);
	}
	m_widgetWidth = width;
	m_widgetHeight = height;
}

///////////////////////////
bool Application::mouseMoveEvent(unsigned int buttonPressed, bool shiftPressed, bool controlPressed, int mx, int my)
{
	//! If the user is in the middle of an interaction, pass the event onto the handler
	if (m_modeManager)
	{
		//! Request a redraw if the interaction hander requires it
		return m_modeManager->onMouseMove((TSLButtonType)buttonPressed, mx, my, shiftPressed, controlPressed);
	}
	return false;
}

bool Application::OnLButtonDown(bool shiftPressed, bool controlPressed, int X, int Y)
{
	//! If the user is in the middle of an interaction, pass the event onto the handler
	if (m_modeManager)
	{
		//! Request a redraw if the interaction hander requires it
		return m_modeManager->onLButtonDown(X, Y, shiftPressed, controlPressed);
	}
	return false;
}

bool Application::OnMButtonDown(bool shiftPressed, bool controlPressed, int X, int Y)
{
	//! If the user is in the middle of an interaction, pass the event onto the handler
	if (m_modeManager)
	{
		//! Request a redraw if the interaction hander requires it
		return m_modeManager->onMButtonDown(X, Y, shiftPressed, controlPressed);
	}
	return false;
}

bool Application::OnRButtonDown(bool shiftPressed, bool controlPressed, int mx, int my)
{
	//! If the user is in the middle of an interaction, pass the event onto the handler
	if (m_modeManager)
	{
		//! Request a redraw if the interaction hander requires it
		return m_modeManager->onRButtonDown(mx, my, shiftPressed, controlPressed);
	}
	return false;
}

bool Application::OnLButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
	//! If the user is in the middle of an interaction, pass the event onto the handler
	if (m_modeManager)
	{
		//! Request a redraw if the interaction hander requires it
		return m_modeManager->onLButtonUp(mx, my, shiftPressed, controlPressed);
	}
	return false;
}

bool Application::OnMButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
	//! If the user is in the middle of an interaction, pass the event onto the handler
	if (m_modeManager)
	{
		//! Request a redraw if the interaction hander requires it
		return m_modeManager->onMButtonUp(mx, my, shiftPressed, controlPressed);
	}
	return true;
}

bool Application::OnRButtonUp(bool shiftPressed, bool controlPressed, int mx, int my)
{
	//! If the user is in the middle of an interaction, pass the event onto the handler
	if (m_modeManager)
	{
		//! Request a redraw if the interaction hander requires it
		return m_modeManager->onRButtonUp(mx, my, shiftPressed, controlPressed);
	}
	return true;
}

bool Application::OnKeyPress(bool, bool, int keySym)
{
	//! The left and right arrow keys allow the drawing surface to be rotated
	switch (keySym)
	{
	case Qt::Key_Left:
		m_surfaceRotation += rotationIncrement;
		m_drawingSurface->rotate(m_surfaceRotation);
		return true;

	case Qt::Key_Right:
		m_surfaceRotation -= rotationIncrement;
		m_drawingSurface->rotate(m_surfaceRotation);
		return true;

	default:
		break;
	}
	return false;
}

bool Application::OnMouseWheel(bool, bool, short zDelta, int X, int Y)
{
	if (m_modeManager)
	{
		//! Request a redraw if the interaction hander requires it
		return m_modeManager->onMouseWheel(zDelta, X, Y);
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////
//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Sets the Map background colour by querying the Map Datalayer
//! for the colour and either clearing the draw surface background
//! colour or setting it to the colour specified in the datalayer.
//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Application::setMapBackgroundColour()
{
	//! Set the Map background colour.
	//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//! Query the colour from tha Map datalayer
	int backgroundColour = m_mapDataLayer->getBackgroundColour();

	//! If there is no colour then clear the colour set in the
	//! drawing surface or we will keep the old colour (the
	//! default colour is white).
	//
	//! If there is a colour set it.
	//
	//! If we have multiple map data layers attached to the drawing
	//! surface we would need to decide at application level
	//! what colour to use.
	//
	//! When we originally attach a datalayer the drawing surface
	//! sets the background colour using the colour in the datalayer
	//! however on subsequent load's the background colour is not
	//! read, as there is a knock on effect depending on which
	//! drawing surfaces a layer is attached to and the order
	//! and number of other attached layers.
	//
	if (backgroundColour == -1)
	{
		m_drawingSurface->clearBackgroundColour();
	}
	else
	{
		m_drawingSurface->setBackgroundColour(backgroundColour);
	}
}

#ifdef WINNT
void Application::drawingInfo(WId window)
{
	m_window = window;
}

#else
void Application::drawingInfo(Drawable drawable, Display *display, Screen *screen, Colormap colourmap, Visual *visual)
{
	m_display = display;
	m_drawable = drawable;
	m_screen = screen;
	m_colourmap = colourmap;
	m_visual = visual;
}
#endif

//! set the call back to update the GUI for reseting interaction modes.
void Application::ResetInteractionModesCallBack(resetInteractionModesCallBack func)
{
	m_resetInteractionModesCallBack = func;
}

//! set the call back to update the GUI when the selected entity changes.
void Application::SelectionChangedCallBack(selectionChangedCallBack func)
{
	m_selectionChangedCallBack = func;
}

///////////////////////////////////////////////////////////////////////////
//! TSLInteractionModeRequest callback functions
///////////////////////////////////////////////////////////////////////////
void Application::resetMode(TSLInteractionMode *, TSLButtonType, TSLDeviceUnits, TSLDeviceUnits)
{
	// reset the editor
	resetEditMode();

	//! call back when reset interaction modes
	if (m_resetInteractionModesCallBack)
	{
		m_resetInteractionModesCallBack();
	}
}

void Application::viewChanged(TSLDrawingSurface*)
{
	//! Do nothing
}

///////////////EDITOR SDK//////////////////
void Application::initializeEditMode(const char *iniFile)
{
	m_iniFile = iniFile;

	// setup edit mode
	if (!m_editMode)
	{
		m_editMode = new TSLInteractionModeEdit(ID_TOOLS_EDIT, this, this, iniFile);
		m_editMode->allowEditing(false);
		m_modeManager->addMode(m_editMode, true);
	}

	//! reset the current view to display the entire map.
	m_drawingSurface->reset();
	if (m_modeManager)
	{
		m_modeManager->resetViews();
		m_modeManager->resetMode(TSLButtonNone, 0, 0);
	}
}

void Application::resetEditMode()
{
	// reset the editor
	if (m_editMode)
		m_editMode->editor()->reset();
}


//////////////////////////////////
void Application::attributePanelCallback(void * arg, TSLRenderingAttributes * attribs)
{
	Application * self = (Application *)arg;

	self->m_editMode->editor()->activate("renderingattributes", attribs);
}

void Application::OnOperationsAttributes()
{
#ifdef USE_RENDERING_ATTRIBUTES_PANEL
	// get the current editor attributes 
	TSLRenderingAttributes attributes;
	std::string title;
	if (!m_attributePanel || !m_attributePanel->isShown())
	{
		title = getRenderingAttributes(m_editMode->editor()->numSelected(), attributes);
	}

	// show in the attribute panel
	if (!m_attributePanel)
	{
		// We're not interested in bordered polygons here - that's only in the Spatial SDK really
		m_attributePanel = new TSLRenderingAttributePanel(attributePanelCallback, (HWND)m_window, false);
		m_attributePanel->setRenderingAttributes(&attributes, this);
		m_attributePanel->setDialogTitle(title.c_str());
		m_attributePanel->showDialog(true);
	}
	else
	{
		bool isCurrentlyShown = m_attributePanel->isShown();

		// We're not interested in bordered polygons here - that's only in the Spatial SDK really
		if (!isCurrentlyShown)
		{
			m_attributePanel->setRenderingAttributes(&attributes, this);
			m_attributePanel->setDialogTitle(title.c_str());
		}
		m_attributePanel->showDialog(!isCurrentlyShown);
	}
#endif
}

void Application::setDefaultRenderingAttributes(TSLRenderingAttributes& attributes)
{
	// Set the default attributes
	TSLProfileHelper::setDefaultFilename(m_iniFile.c_str());
	TSLProfileHelper::setDefaultSection("initialAttributes");

	// edge/fill colour
	TSLProfileHelper::lookupProfile("edgeColour", &attributes.m_edgeColour, 1);
	TSLProfileHelper::lookupProfile("edgeStyle", &attributes.m_edgeStyle, 1);
	TSLProfileHelper::lookupProfile("edgeWidth", &attributes.m_edgeThickness, 1.0);
	TSLProfileHelper::lookupProfile("fillColour", &attributes.m_fillColour, 211);
	TSLProfileHelper::lookupProfile("fillStyle", &attributes.m_fillStyle, 2);
	TSLProfileHelper::lookupProfile("borderWidth", &attributes.m_exteriorEdgeThickness, 1.0);
	TSLProfileHelper::lookupProfile("borderColour", &attributes.m_exteriorEdgeColour, 1);
	attributes.m_exteriorEdgeStyle = 1;
	attributes.m_edgeThicknessUnits = TSLDimensionUnitsPixels;
	attributes.m_exteriorEdgeThicknessUnits = TSLDimensionUnitsPixels;

	// text
	TSLProfileHelper::lookupProfile("textColour", &attributes.m_textColour, 6);
	TSLProfileHelper::lookupProfile("textFont", &attributes.m_textFont, 1);
	TSLProfileHelper::lookupProfile("textHeight", &attributes.m_textSizeFactor, 50);
	attributes.m_textSizeFactorUnits = TSLDimensionUnits::TSLDimensionUnitsPixels;
	int val = 0;
	TSLProfileHelper::lookupProfile("textHorizontalAlignment", &val, TSLHorizontalAlignmentCentre);
	attributes.m_textHorizontalAlignment = static_cast<TSLHorizontalAlignment>(val);
	TSLProfileHelper::lookupProfile("textVerticalAlignment", &val, TSLVerticalAlignmentMiddle);
	attributes.m_textVerticalAlignment = static_cast<TSLVerticalAlignment>(val);
	TSLProfileHelper::lookupProfile("textBackgroundMode", &val, TSLTextBackgroundModeNone);
	attributes.m_textBackgroundMode = static_cast<TSLTextBackgroundMode>(val);
	TSLProfileHelper::lookupProfile("textBackgroundStyle", &attributes.m_textBackgroundStyle, 1);
	TSLProfileHelper::lookupProfile("textBackgroundColour", &attributes.m_textBackgroundColour, 216);
	TSLProfileHelper::lookupProfile("textBackgroundEdgeColour", &attributes.m_textBackgroundEdgeColour, 1);

	// symbol
	TSLProfileHelper::lookupProfile("symbolColour", &attributes.m_symbolColour, 181);
	TSLProfileHelper::lookupProfile("symbolStyle", &attributes.m_symbolStyle, 99002);
	TSLProfileHelper::lookupProfile("symbolSize", &attributes.m_symbolSizeFactor, 50);
	attributes.m_symbolSizeFactorUnits = TSLDimensionUnits::TSLDimensionUnitsPixels;
}

std::string Application::getRenderingAttributes(int numEntities, TSLRenderingAttributes& attributes)
{
	std::string panelTitle = "";
	if (numEntities)
	{
		m_editMode->editor()->querySelectedAttributes(attributes);
		panelTitle = "Select list attributes";
	}
	else
	{
		TSLVariant value;
		if (m_editMode->editor()->query("renderingattributes", &value))
		{
			if (TSLVariantTypePtr == value.getType())
			{
				void *ptr = NULL;
				if (value.getVal(&ptr))
				{
					if (ptr != NULL)
					{
						TSLRenderingAttributes * defaultAttributes = reinterpret_cast<TSLRenderingAttributes *>(ptr);
						if (defaultAttributes)
						{
							panelTitle = "Default attributes";
							attributes = *defaultAttributes;
						}
					}
				}
			}
		}
	}

	return panelTitle;
}

//////////////////EDITOR CALLBACKS///////////////////
void Application::invokeContextMenu(TSLEditor* editor, TSLDeviceUnits xDU, TSLDeviceUnits yDU)
{
}

bool Application::onOperationDeactivated()
{
	//! call back when reset interaction modes
	if (m_resetInteractionModesCallBack)
	{
		m_resetInteractionModesCallBack();
	}

	return false;
}

bool Application::onSelectionChanged(int numEntities, int depth)
{
	m_numEntities = numEntities;
	m_selDepth = depth;

#ifdef USE_RENDERING_ATTRIBUTES_PANEL
	if (m_attributePanel)
	{
		TSLRenderingAttributes attributes;
		std::string panelTitle = getRenderingAttributes(numEntities, attributes);
		m_attributePanel->setDialogTitle(panelTitle.c_str());
		m_attributePanel->setRenderingAttributes(&attributes, this);
	}
#endif

	// show transformation toolbar only if there is selection
	if (m_selectionChangedCallBack)
	{
		m_selectionChangedCallBack(numEntities);
	}

	return true;
}

bool Application::requestDialog(const char* prompt, const char* label1, const char* label2, const char* label3, const char* label4)
{
	return false;
}

bool Application::requestText(const char* prompt, const char* initialValue)
{
	TextEntryDialog txtDlg;
	txtDlg.setInitialText(initialValue);
	int dialogCode = txtDlg.exec();
	if (dialogCode == QDialog::Accepted)
	{
		std::string txtEntered = txtDlg.getText();
		if (!txtEntered.empty())
		{
			m_editor->textEntered(txtEntered.c_str(), false);
			return true;
		}
	}
	m_editor->textEntered("", false);
	return false;
}

bool Application::displayError(const char* message)
{
	QMessageBox::information(m_parentWidget, "Display Error", message, QMessageBox::Cancel);
	return true;
}

int Application::querySelectList(int & depth)
{
	depth = m_selDepth;
	return m_numEntities;
}

const char * Application::queryPrompt()
{
	return m_modeManager ? m_modeManager->queryPrompt() : "";
}

////////////////////////////////////
bool Application::loadMap(const char *mapFilename)
{
	//! Clear the error stack, load the map then check for errors.
	TSLErrorStack::clear();

	if (mapFilename == NULL)
	{
		return false;
	}

	//! load the map
	if (!m_mapDataLayer->loadData(mapFilename))
	{
		QString messageBody("Could not load map " + QString::fromUtf8(mapFilename));
		QMessageBox::information(m_parentWidget, "Could not load map",
			messageBody, QMessageBox::Cancel);
		return false;
	}
	setMapBackgroundColour();

	// setup editmode
	if (m_editMode)
	{
		// reset the editor
		m_editMode->editor()->reset();
		m_editMode->allowEditing(true);
		m_editMode->editor()->dataChanged();

		// initialize the rendering attribute panel if not initialized.
		if (m_initialiseRendering)
		{
			// Set the default attributes
			TSLRenderingAttributes attributes;
			setDefaultRenderingAttributes(attributes);
			m_editMode->editor()->enableGlobalUndo(UNDO_NUM_SLOTS);
			m_editMode->editor()->activate("renderingattributes", &attributes);
			m_initialiseRendering = false;
		}

		// show rendering attributes panel at the start
		//OnOperationsAttributes();
	}

	// and reset the current view to display the entire map.
	m_drawingSurface->reset(false);
	if (m_modeManager)
	{
		//! Loading a map invalidates any stored views in mode manager  - this sample doesn't create any
		m_modeManager->resetViews();
		m_modeManager->resetMode(TSLButtonNone, 0, 0);
	}

	//! Display any errors that have occurred
	const char * msg = TSLErrorStack::errorString("Cannot load map\n");
	if (msg)
	{
		QMessageBox::information(m_parentWidget, "Could not load map",
			QString::fromUtf8(msg), QMessageBox::Cancel);
		return false;
	}

	return true;
}

bool Application::loadGeometryFile(const char *tmfFilename)
{
	if (m_editor && m_editDataLayer)
	{
		m_editor->reset();

		if (!m_editDataLayer->loadData(TSLUTF8Encoder(tmfFilename)))
			return false;

		m_editor->dataChanged();
	}

	return true;
}

bool Application::saveGeometryFile(const char *tmfFilename)
{
	if (m_editor && m_editDataLayer)
	{
		m_editor->reset();

		if (!m_editDataLayer->saveData(TSLUTF8Encoder(tmfFilename)))
			return false;
	}

	return true;
}

void Application::newGeometryFile()
{
	if (m_editor && m_editDataLayer)
	{
		m_editor->reset();
		m_editDataLayer->removeData();
		m_editor->dataChanged();
	}
}

///////////////////////////////////////////////////////////////////////////
//! Event handler functions - these are invoked from the widget
///////////////////////////////////////////////////////////////////////////

void Application::activatePanMode()
{
	//! Activate the pan interaction mode
	if (m_modeManager)
	{
		m_modeManager->setCurrentMode(ID_TOOLS_PAN);
	}
}

void Application::activateZoomMode()
{
	//! Activate the zoom interaction mode
	if (m_modeManager)
	{
		m_modeManager->setCurrentMode(ID_TOOLS_ZOOM);
	}
}

void Application::activateGrabMode()
{
	//! Activate the grab interaction mode
	if (m_modeManager)
	{
		m_modeManager->setCurrentMode(ID_TOOLS_GRAB);
	}
}

void Application::activateMagnifierMode()
{
	//! Activate the magnifier interaction mode
	if (m_modeManager)
	{
		m_modeManager->setCurrentMode(ID_TOOLS_MAGNIFY);
	}
}

void Application::activateEditMode()
{
	//! Activate the edit interaction mode
	if (m_modeManager)
	{
		m_modeManager->setCurrentMode(ID_TOOLS_EDIT);
	}
}
/////////////////////////////////////
bool Application::zoomIn()
{
	if (m_modeManager)
	{
		//! Request a redraw if the interaction hander requires it
		return m_modeManager->zoomIn(30);
	}
	return false;
}

bool Application::zoomOut()
{
	if (m_modeManager)
	{
		//! Request a redraw if the interaction hander requires it
		return m_modeManager->zoomOut(30);
	}
	return false;
}

void Application::resetView()
{
	//! Reset the view to the full extent of the map being loaded
	if (m_drawingSurface)
	{
		//! Reset the drawing surface rotation as well
		m_surfaceRotation = 0.0;
		m_drawingSurface->rotate(m_surfaceRotation);

		m_drawingSurface->reset(false);
	}
}

//////////////////EDITOR INTERACTION MODES///////////////////
void Application::activateSelectAllMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("selectall", 0);
	}
}
void Application::activateSelectByRectangleMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("selectbyextent", 0);
	}
}
void Application::activateSelectByPolygonMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("selectbypolygon", 0);
	}
}
///////////
void Application::activateTransformationUndo()
{
	if (m_editMode)
	{
		m_editMode->editor()->undo();
	}
}
void Application::activateTransformationRedo()
{
	if (m_editMode)
	{
		m_editMode->editor()->redo();
	}
}
void Application::activateTransformationClear()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("clear", 0);
	}
}

bool Application::checkTransformationUndoPossible()
{
	if (m_editMode)
	{
		return m_editMode->editor()->undoPossible();
	}
	return false;
}
bool Application::checkTransformationRedoPossible()
{
	if (m_editMode)
	{
		return m_editMode->editor()->redoPossible();
	}
	return false;
}
///////////
void Application::activateCreatePolygonMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("polygon", 0);
	}
}

void Application::activateCreatePolylineMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("polyline", 0);
	}
}

void Application::activateCreateMarkerMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("symbol", 0);
	}
}

void Application::activateCreateCircleMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("circle", 0);
	}
}

void Application::activateCreateTextMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("text", 0);
	}
}

///////////
void Application::activateTransformationCopyMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("copy", 0);
	}
}
void Application::activateTransformationMoveMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("move", 0);
	}
}
void Application::activateTransformationScaleMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("scale", 0);
	}
}
void Application::activateTransformationRotateMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("rotate", 0);
	}
}
void Application::activateTransformationChangeTextMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("settext", 0);
	}
}
void Application::activateTransformationAddPointMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("topologicaladdpoint", 0);
	}
}
void Application::activateTransformationDeletePointMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("topologicaldeletepoint", 0);
	}
}
void Application::activateTransformationMovePointMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("topologicalmovepoint", 0);
	}
}
void Application::activateTransformationFrontMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("front", 0);
	}
}
void Application::activateTransformationBackMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("back", 0);
	}
}
void Application::activateTransformationGroupMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("group", 0);
	}
}
void Application::activateTransformationUngroupMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("ungroup", 0);
	}
}
void Application::activateTransformationDeleteMode()
{
	if (m_editMode)
	{
		m_editMode->editor()->activate("delete", 0);
	}
}

///////////
void Application::activateConstraintEqual()
{
	if (m_editMode)
	{
		m_editMode->editor()->constraint(TSLEditorConstraintEqual);
	}
}
void Application::activateConstraintUnequal()
{
	if (m_editMode)
	{
		m_editMode->editor()->constraint(TSLEditorConstraintNone);
	}
}
void Application::activateConstraintHorizontal()
{
	if (m_editMode)
	{
		m_editMode->editor()->constraint(TSLEditorConstraintHorizontal);
	}
}
void Application::activateConstraintVertical()
{
	if (m_editMode)
	{
		m_editMode->editor()->constraint(TSLEditorConstraintVertical);
	}
}
void Application::activateConstraintAngular()
{
	if (m_editMode)
	{
		// read the angular value from the config file
		TSLProfileHelper::setDefaultSection("constraintParams");
		TSLProfileHelper::lookupProfile("angularValue", &m_angularValue, 0.0);

		// set constraint angular value
		m_editMode->editor()->constraint(TSLEditorConstraintAngle, m_angularValue);
	}
}
void Application::activateConstraintDistance()
{
	if (m_editMode)
	{
		// read the angular value from the config file
		TSLProfileHelper::setDefaultSection("constraintParams");
		TSLProfileHelper::lookupProfile("distanceValue", &m_distanceValue, 0.0);

		// set constraint angular value
		m_editMode->editor()->constraint(TSLEditorConstraintDistance, m_distanceValue * USER_UNIT_SIZE);
	}
}
