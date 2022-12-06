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
  , m_recordTracksHistory(true)
  , m_recordMaximum(500)
{
  m_MainWindow = this;
  //! Construct the window
  setupUi(this);

  if (m_recordTracksHistory)
  {
    //! initiliaze tracks history slider and add it to the tool bar.
    m_tracksHistorySlider = new QSlider(this);
    m_tracksHistorySlider->setOrientation(Qt::Horizontal);
    m_tracksHistorySlider->setFixedWidth(200);

    m_tracksHistoryTitle = new QLabel(this);
    m_tracksHistoryTitle->setText("  Tracks history: ");

    m_tracksHistoryProgress = new QLabel(this);

    m_tracksHistoryTitleAction = mainToolBar->addWidget(m_tracksHistoryTitle);
    m_tracksHistorySliderAction = mainToolBar->addWidget(m_tracksHistorySlider);
    m_tracksHistoryProgressAction = mainToolBar->addWidget(m_tracksHistoryProgress);

    //! hide tracks history slider and labels.
    showHistorySlider(false);

    connect(m_tracksHistorySlider, SIGNAL(valueChanged(int)), this, SLOT(historySliderValueChanged(int)));
  }


  //! Connect the actions for the toolbars and menus to the slots that deal with them
  connect(action_Open, SIGNAL(triggered()), this, SLOT(loadMap()));
  connect(actionReset, SIGNAL(triggered()), this, SLOT(resetView()));
  connect(actionZoom_In, SIGNAL(triggered()), this, SLOT(zoomInOnce()));
  connect(actionZoom_Out, SIGNAL(triggered()), this, SLOT(zoomOutOnce()));
  connect(actionZoom_Mode, SIGNAL(triggered()), this, SLOT(activateZoomMode()));
  connect(actionPan_Mode, SIGNAL(triggered()), this, SLOT(activatePanMode()));
  connect(actionGrab_Mode, SIGNAL(triggered()), this, SLOT(activateGrabMode()));
  connect(actionTracks_Mode, SIGNAL(triggered()), this, SLOT(activateTracksMode()));

  connect(actionStart_Client, SIGNAL(triggered()), this, SLOT(activateStartClient()));
  connect(actionStop_Client, SIGNAL(triggered()), this, SLOT(activateStopClient()));

  connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutBox()));
  connect(actionExit, SIGNAL(triggered()), this, SLOT(exit()));

  //! Create a group of actions for the interaction mode buttons and menus so that
  //! the active interaction mode is reflected in the toolbar and menu display
  m_interactionModesGroup = new QActionGroup(mainToolBar);
  m_interactionModesGroup->addAction(actionZoom_Mode);
  m_interactionModesGroup->addAction(actionPan_Mode);
  m_interactionModesGroup->addAction(actionGrab_Mode);
  m_interactionModesGroup->addAction(actionTracks_Mode);

  if (maplinkWidget)
  {
    maplinkWidget->setCursor(Qt::CrossCursor);
    maplinkWidget->ResetInteractionModesCallBack(resetInteractionModes_CallBack);
    maplinkWidget->setRecordTracks(m_recordTracksHistory, m_recordMaximum);
  }
  actionStop_Client->setVisible(false);

  //! initialize metadata table
  metadataDockWidget->setGeometry(QRect(12, 107, 204, 424));
  metadataDockWidget->close();

  //! Initialize Client Connection thread.
  m_clientConnectionThread = new ClientConnectionThread(this);
  if (maplinkWidget)
  {
    maplinkWidget->setClientConnectionThread(m_clientConnectionThread);
  }

  string errorMsg;
  std::string settingsFilePath = "C:/Users/Ahmed.Ibrahim/Desktop/eventmanagersettings.ini";
  if (!m_clientConnectionThread->initializeWebSocket(settingsFilePath, errorMsg))
  {
    QMessageBox::critical(this, tr("Web socket initialization error!"), tr(errorMsg.c_str()));
    return;
  }

  //! Connect Client Connection thread's signal to this class's slot.
  connect
  (
    m_clientConnectionThread, SIGNAL(tracksUpdated()),
    this, SLOT(onTracksUpdated())
  );

  connect
  (
    m_clientConnectionThread, SIGNAL(trackedItemUpdated()),
    this, SLOT(onTrackedItemUpdated())
  );

  connect
  (
    m_clientConnectionThread, SIGNAL(errorsUpdated()),
    this, SLOT(onErrorsUpdated())
  );

  //! start the thread.
  m_clientConnectionThread->start();
}

MainWindow::~MainWindow()
{
  //! exit the thread.
  activateExitClient();
  m_clientConnectionThread->wait();

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
  QString initDir = "C:/Users/Ahmed.Ibrahim/Desktop/DirectImportData/NaturalEarthBasic";
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), initDir, tr("All Map files (*.map *.mpc)"));

  if (!fileName.isEmpty() && maplinkWidget)
  {
    maplinkWidget->loadMap(fileName.toUtf8());
  }
}

void MainWindow::showAboutBox()
{
  //! Display an about box
  QMessageBox::about(this, tr("MapLink Pro Qt Event Manager Sample"),
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

  //! hide metadata table
  metadataDockWidget->close();
}

void MainWindow::activatePanMode()
{
  if (maplinkWidget)
  {
    //! Tell the widget to activate the pan interaction mode
    maplinkWidget->activatePanMode();
    maplinkWidget->setCursor(Qt::SizeAllCursor);
  }

  //! hide metadata table
  metadataDockWidget->close();
}

void MainWindow::activateGrabMode()
{
  if (maplinkWidget)
  {
    //! Tell the widget to activate the grab interaction mode
    maplinkWidget->activateGrabMode();
    maplinkWidget->setCursor(Qt::OpenHandCursor);
  }

  //! hide metadata table
  metadataDockWidget->close();
}

void MainWindow::activateTracksMode()
{
  if (maplinkWidget)
  {
    //! Tell the widget to activate the zoom interaction mode
    maplinkWidget->activateTracksMode();
    maplinkWidget->setCursor(Qt::WhatsThisCursor);
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

///////////////////////////////////////////////////////////////////////////
//! Tracks
///////////////////////////////////////////////////////////////////////////

//! handles start thread button click
void MainWindow::activateStartClient()
{
  //! subscribe channels to receive data.
  string errorMsg;
  if (!m_clientConnectionThread->subscribe(errorMsg))
  {
    QMessageBox::critical(this, tr("Web socket subscription error!"), tr(errorMsg.c_str()));
    return;
  }
  actionStart_Client->setVisible(false);
  actionStop_Client->setVisible(true);

  if (m_recordTracksHistory)
  {
    maplinkWidget->displayTime(0);
    //! hide tracks history slider and labels.
    showHistorySlider(false);
  }
}

//! handles stop thread button click
void MainWindow::activateStopClient()
{
  //! unsubscribe channels to stop receiving data.
  string errorMsg;
  if (!m_clientConnectionThread->unsubscribe(errorMsg))
  {
    QMessageBox::critical(this, tr("Web socket subscription error!"), tr(errorMsg.c_str()));
    return;
  }

  actionStop_Client->setVisible(false);
  actionStart_Client->setVisible(true);
  if (m_recordTracksHistory)
  {
    m_current_RecordMaximum = (maplinkWidget) ? maplinkWidget->getCurrentTime() : 0;
    if (m_current_RecordMaximum > 0)
    {
      if (m_current_RecordMaximum - m_recordMaximum > 1)
      {
        m_tracksHistorySlider->setMinimum(m_current_RecordMaximum - m_recordMaximum);
      }
      else
      {
        m_tracksHistorySlider->setMinimum(1);
      }
      m_tracksHistorySlider->setMaximum(m_current_RecordMaximum);
      m_tracksHistorySlider->setValue(m_current_RecordMaximum);

      //! show tracks history slider and labels.
      showHistorySlider(true);
    }
  }
}

//! handles exit client thread
bool MainWindow::activateExitClient()
{
  if (!m_clientConnectionThread->exit())
  {
    QMessageBox::critical(this, tr("Web socket exit error!"), tr("Failed to exit web socket"));
    return false;
  }
  return true;
}


//! handles tracks updated slot sent by the thread
void MainWindow::onTracksUpdated()
{
  if (maplinkWidget)
  {
    std::vector<std::pair<string, string>> metadatPairs;
    maplinkWidget->onTracksUpdated(metadatPairs);
    if (metadatPairs.size() > 0)
    {
      showMetadataTableWidget(metadatPairs);
    }
  }
}

//! handles tracks updated slot sent by the thread
void MainWindow::onTrackedItemUpdated()
{
  if (maplinkWidget)
  {
    std::vector<std::pair<string, string>> metadatPairs;
    if (maplinkWidget->onTrackedItemUpdated(metadatPairs))
    {
      //! show metadata table widget
      showMetadataTableWidget(metadatPairs);
    }
  }
}

//! handles errors updated slot sent by the thread
void MainWindow::onErrorsUpdated()
{
  if (maplinkWidget)
  {
    maplinkWidget->onErrorsUpdated();
  }
}

//! show metadata table widget
void MainWindow::showMetadataTableWidget(std::vector<std::pair<string, string>> &metadatPairs)
{
  metadataTableWidget->clearContents();
  int size = metadatPairs.size();
  if (size < 13)
  {
    metadataTableWidget->setRowCount(13);
  }
  else
  {
    metadataTableWidget->setRowCount(metadatPairs.size());
  }
  metadataTableWidget->setColumnCount(2);
  QStringList m_TableHeader;
  m_TableHeader << "Key" << "Value";
  metadataTableWidget->setHorizontalHeaderLabels(m_TableHeader);
  metadataTableWidget->verticalHeader()->setVisible(false);
  metadataTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
  metadataTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  metadataTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  metadataTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  int idx = 0;
  for (const auto& keyVal : metadatPairs)
  {
    metadataTableWidget->setItem(idx, 0, new QTableWidgetItem(keyVal.first.c_str()));
    metadataTableWidget->setItem(idx, 1, new QTableWidgetItem(keyVal.second.c_str()));
    ++idx;
  }
  //! show metadata table
  metadataDockWidget->show();
}

//! handles changing the tracks history slider.
void MainWindow::historySliderValueChanged(int value)
{
  //! set the slider progress text
  QString maxTime = QString::number(value);
  QString currentRecordMaximum = QString::number(m_current_RecordMaximum);
  QString my_formatted_string = QString("(%1:%2)").arg(maxTime, currentRecordMaximum);
  m_tracksHistoryProgress->setText(my_formatted_string);

  //! display the history time
  if (maplinkWidget)
  {
    maplinkWidget->displayTime(value);
    maplinkWidget->redrawSurface();
  }
}

//! Show/hide tracks history slider and labels.
void MainWindow::showHistorySlider(bool visible)
{
  m_tracksHistoryTitleAction->setVisible(visible);
  m_tracksHistorySliderAction->setVisible(visible);
  m_tracksHistoryProgressAction->setVisible(visible);
}