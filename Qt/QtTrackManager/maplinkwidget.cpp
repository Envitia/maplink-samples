/****************************************************************************
Copyright (c) 2008-2022 by Envitia Group PLC.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

****************************************************************************/

#include "maplinkwidget.h"
#include "application.h"
#ifndef WIN32
# include <QX11Info>
#endif

#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
using namespace std;

MapLinkWidget::MapLinkWidget(QWidget *parent)//, Qt::WFlags flags)
  : QWidget(parent)
  , m_application(new Application(parent))
  , m_initialized(false)
{
  //! This is required for Qt4 to stop the back ground being drawn and Qt
  //! Double buffering. You also need to override paintEngine().
  //
  //! Ref:
  //! http://lists.trolltech.com/qt-interest/2006-02/thread00004-0.html
  //
  setAttribute(Qt::WA_NoBackground, true);
  setAttribute(Qt::WA_NoSystemBackground, true);
  //! Possible issue with this for Qt4.1.0 and newer versions.
  //
  //! See:
  //!   http://www.trolltech.com/developer/task-tracker/index_html?id=106922&method=entry
  //!   http://lists.trolltech.com/qt-interest/2006-05/thread00316-0.html
  //
  //! Talk to Trolltech support about getting a fix if this proves to be a problem
  //
  //! NOTE: I am not seeing this problem, probably because I'm doing things slightly
  //!       differently from the example.
  setAttribute(Qt::WA_PaintOnScreen);
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_NativeWindow);

  //! set the focus policy so we can get keyboard events
  setFocusPolicy(Qt::WheelFocus);

  //! Set the mouse tracking in the designer.
  setMouseTracking(true);

  create();

  //! Initialize tracks simulator thread and connect its signal to this class's slot.
  tracksSimulatorThread = new TracksSimulator(this);
  connect
  (
    tracksSimulatorThread, SIGNAL(tracksUpdated()),
    this, SLOT(onTracksUpdated())
  );
}


MapLinkWidget::~MapLinkWidget()
{
  //! stop the thread.
  activateStopTracks();
  tracksSimulatorThread->wait();

  //! Clean up
  delete m_application;
}

//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! Map to load.
//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::loadMap(const char *filename)
{
  if (m_application)
  {
    m_application->loadMap(filename); //! Load the map/file
    m_application->resetView();//! Reset the view to show the entire extent of the new map/file
    update(); //! Cause a redraw so the new map/file can be seen
  }
}

bool MapLinkWidget::parseConfigurationFile(const QString &configFilePath, QString &msgError)
{
  if (!tracksSimulatorThread)
  {
    return false;
  }
  return tracksSimulatorThread->parseConfigurationFile(configFilePath, msgError);
}

//! get configuration settings.
const ConfigurationSettings& MapLinkWidget::getConfigurationSettings()
{
  return tracksSimulatorThread->getConfigurationSettings();
}

bool MapLinkWidget::close()
{
  bool result = QWidget::close();
  delete m_application;
  m_application = NULL;
  return result;
}

void MapLinkWidget::create()
{
  if (!m_application)
  {
    m_application = new Application(parentWidget());
  }
#ifndef WIN32
  //! The method to access the widget's native X11 resources differs depending on the version of Qt being used.
# if QT_VERSION < 0x50000
  QX11Info x11info = this->x11Info();
  Display *display = x11info.display();
  int screenNum = x11info.screen();
  Visual *visual = (Visual *)x11info.visual();
  Qt::HANDLE colourmap = x11info.colormap();
  Qt::HANDLE drawable = handle();
  Screen *screen = ScreenOfDisplay(display, screenNum);
# elif QT_VERSION >= 0x50100
  //! Qt 5.1 or later
  Display *display = QX11Info::display();
  QDesktopWidget *desktop = QApplication::desktop();
  int screenNum = desktop->screenNumber(this);
  Screen *screen = ScreenOfDisplay(display, screenNum);
  Visual *visual = DefaultVisual(display, screenNum);
  Colormap colourmap = DefaultColormap(display, screenNum);
  WId drawable = winId();
# else
#  error "This sample does not currently support building with Qt 5.0.
# endif

  //! pass to the application as we will need for the Drawing Surface
  m_application->drawingInfo(drawable, display, screen, colourmap, visual);
#else
  //! Attaching to the window is much more efficent.
  WId hWnd = winId();
  m_application->drawingInfo(hWnd);
#endif
  m_application->resize(width(), height());
  m_application->create();
}

void MapLinkWidget::paintEvent(QPaintEvent *rect)
{
  if (m_application == NULL)
    create();

  //! Redraw the MapLink drawing surface
  m_application->redraw();

  m_initialized = true;
  
  //! When using the X11/GDI drawing surfaces with Qt5
  //! the application can reach a state where the MapLinkWidget
  //! has received an update event but the MainWindow has not.
  //! If the MapLinkWidget has been double buffered by Qt
  //! this means the map update will not take effect until the
  //! next event is received by the widget (key/mouse press).
  //! Emit a signal to notify any classes that the map display
  //! has been redrawn.
  //
  //! This must not be done if the map is being rendered continuously
  //! using Qt or on a regular interval with a timer.
  emit mapDrawn();
}

//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! resize - informs the drawing surface of any change in size of the window
//! Relys on the ResizeAction to maintain the view of the map sensibly
//! ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::resizeEvent(QResizeEvent *event)
{
  QWidget::resizeEvent(event);
  if (m_application)
  {
    m_application->resize(width(), height());
    if (!m_initialized)
	{
		m_application->resetView();
	}
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Mouse & Keyboard handling
//
// We only update the display if MapLink tells us too.
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void MapLinkWidget::mouseMoveEvent(QMouseEvent *event)
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = (modifiers & Qt::ShiftModifier);
  bool controlPressed = (modifiers & Qt::ControlModifier);

  //! The MapLink application class doesn't use Qt - convert the mouse button types
  //! to the MapLink types
  TSLButtonType button = TSLButtonNone;
  Qt::MouseButton buttonQt = event->button();
  switch (buttonQt)
  {
  case Qt::LeftButton:
    button = TSLButtonLeft;
    break;

  case Qt::MidButton:
    button = TSLButtonCentre;
    break;

  case Qt::RightButton:
    button = TSLButtonRight;
    break;

  default:
    break;
  }

  //! Forward the event onto the application
  if (m_application->mouseMoveEvent(button, shiftPressed, controlPressed, event->x(), event->y()))
  {
    update(); //! We were asked to redraw the display
  }
}

void MapLinkWidget::mousePressEvent(QMouseEvent *event)
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = (modifiers & Qt::ShiftModifier);
  bool controlPressed = (modifiers & Qt::ControlModifier);
  int x = event->x();
  int y = event->y();
  Qt::MouseButton button = event->button();

  //! Forward the event onto the application
  bool redraw = false;
  switch (button)
  {
  case Qt::LeftButton:
    redraw = m_application->OnLButtonDown(shiftPressed, controlPressed, x, y);
    break;

  case Qt::MidButton:
    redraw = m_application->OnMButtonDown(shiftPressed, controlPressed, x, y);
    break;

  case Qt::RightButton:
    redraw = m_application->OnRButtonDown(shiftPressed, controlPressed, x, y);
    break;

  default:
    break;
  }

  if (redraw)
  {
    update(); //! We were asked to redraw the display
  }
}

void MapLinkWidget::mouseReleaseEvent(QMouseEvent *event)
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = (modifiers & Qt::ShiftModifier);
  bool controlPressed = (modifiers & Qt::ControlModifier);
  int x = event->x();
  int y = event->y();
  Qt::MouseButton button = event->button();

  //! Forward the event onto the application
  bool redraw = false;
  switch (button)
  {
  case Qt::LeftButton:
    redraw = m_application->OnLButtonUp(shiftPressed, controlPressed, x, y);
    break;

  case Qt::MidButton:
    redraw = m_application->OnMButtonUp(shiftPressed, controlPressed, x, y);
    break;

  case Qt::RightButton:
    redraw = m_application->OnRButtonUp(shiftPressed, controlPressed, x, y);
    break;

  default:
    break;
  }
  if (redraw)
  {
    update(); //! We were asked to redraw the display
  }
}

void MapLinkWidget::wheelEvent(QWheelEvent *event)
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = (modifiers & Qt::ShiftModifier);
  bool controlPressed = (modifiers & Qt::ControlModifier);
  int x = event->x();
  int y = event->y();

  //! Forward the event onto the application
  if (m_application->OnMouseWheel(shiftPressed, controlPressed, event->delta(), x, y))
  {
    update(); //! We were asked to redraw the display
  }
}

void MapLinkWidget::keyPressEvent(QKeyEvent *event)
{
  Qt::KeyboardModifiers modifiers = event->modifiers();
  bool shiftPressed = (modifiers & Qt::ShiftModifier);
  bool controlPressed = (modifiers & Qt::ControlModifier);

  //! Forward the event onto the application
  if (m_application->OnKeyPress(shiftPressed, controlPressed, event->key()))
  {
    update(); //! We were asked to redraw the display
  }
}

void MapLinkWidget::keyReleaseEvent(QKeyEvent *)
{
  //! ignore
}



///////////////////////////////////////////
//! Events forwarded from the main window. These are all forwarded on to
//! the appliction, and we will issue a redraw request if the application asks
//! us to.
void MapLinkWidget::resetView()
{
  m_application->resetView();
  update(); //! The viewing extent has changed, so a redraw is always required
}

void MapLinkWidget::zoomInOnce()
{
  if (m_application->zoomIn())
  {
    //! We were asked to redraw the display
    update();
  }
}

void MapLinkWidget::zoomOutOnce()
{
  if (m_application->zoomOut())
  {
    //! We were asked to redraw the display
    update();
  }
}

void MapLinkWidget::activatePanMode()
{
  //! Tell the application to activate the pan interaction mode
  m_application->activatePanMode();
}

void MapLinkWidget::activateGrabMode()
{
  //! Tell the application to activate the grab interaction mode
  m_application->activateGrabMode();
}

void MapLinkWidget::activateZoomMode()
{
  //! Tell the application to activate the zoom interaction mode
  m_application->activateZoomMode();
}

//! set the call back to update the GUI for reseting interaction modes.
void MapLinkWidget::ResetInteractionModesCallBack(resetInteractionModesCallBack func)
{
  m_application->ResetInteractionModesCallBack(func);
}

///////////////////////////////////////////////////////////////////////////
//! Tracks
///////////////////////////////////////////////////////////////////////////

//! handles start thread button click
bool MapLinkWidget::activateUse_symbolSetsChanged(const QString &symbolSet, QString& msgError)
{
  tracksSimulatorThread->m_mutexTracks.lock();

  //! change tracks information.
  bool isChanged = tracksSimulatorThread->changeTracksSymbol(symbolSet, msgError);

  tracksSimulatorThread->m_mutexTracks.unlock();

  //! update the tracks
  onTracksUpdated();

  return isChanged;
}

//! handles stop thread button click
void MapLinkWidget::activateStopTracks()
{
  //! enable the abort flag to quit the thread's while loop.
  tracksSimulatorThread->m_mutexAbort.lock();
  tracksSimulatorThread->m_abort = true;
  tracksSimulatorThread->m_mutexAbort.unlock();
}

//! handles tracks updated slot sent by the thread
void MapLinkWidget::onTracksUpdated()
{
  tracksSimulatorThread->m_mutexTracks.lock();

  //! Process and update the track manager with the updated tracks.
  m_application->processUpdatedTracks(tracksSimulatorThread->m_tracksInformation);

  //! redraw the drawing surface.
  m_application->redrawSurface();

  tracksSimulatorThread->m_mutexTracks.unlock();
}


//! handles changing the tracks symbols [config xml/ default]
void MapLinkWidget::activateStartTracks()
{
  //! check if thread is already running.
  if (tracksSimulatorThread->isRunning())
  {
    return;
  }

  //! start the thread.
  tracksSimulatorThread->start();
}