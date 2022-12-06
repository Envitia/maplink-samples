/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#include <QtGui>
#include <QMainWindow>
#include <QEvent>
#include <QWidget>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPaintEngine>
#include <QWidget>
#include <QInputEvent>
#include <QGLWidget>
#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // Set the Window Icon to the MapLink Pro Icon
  setWindowIcon(QIcon(":images/SimpleInteraction.png"));

  // Add an Open File button to the toolbar.
  //
  // The images are in the directory images.
  // They are built into the exe using the MapLini.qrc file.
  m_openFileAction = new QAction(QIcon(":/images/file_open.png"), tr("&Open"), this);
  m_openFileAction->setShortcut(tr("Open"));
  m_openFileAction->setStatusTip(tr("Open map"));
  connect(m_openFileAction, SIGNAL(triggered()), this, SLOT(open()));

  // Add a Reset Map button to the toolbar.
  m_resetMapAction = new QAction(QIcon(":/images/reset.png"), tr("&Reset"), this);
  m_resetMapAction->setShortcut(tr("Reset"));
  m_resetMapAction->setStatusTip(tr("Reset"));
  connect(m_resetMapAction, SIGNAL(triggered()), this, SLOT(resetMap()));

  // Add a zoom in once Map button to the toolbar.
  m_zoomInOnceAction = new QAction(QIcon(":/images/zoomin_once.png"), tr("Zoom &In"), this);
  m_zoomInOnceAction->setShortcut(tr("Zoom In"));
  m_zoomInOnceAction->setStatusTip(tr("Zoom In Once"));
  connect(m_zoomInOnceAction, SIGNAL(triggered()), this, SLOT(zoomInOnce()));

  // Add a zoom out once Map button to the toolbar.
  m_zoomOutOnceAction = new QAction(QIcon(":/images/zoomout_once.png"), tr("Zoom &Out"), this);
  m_zoomOutOnceAction->setShortcut(tr("Zoom Out"));
  m_zoomOutOnceAction->setStatusTip(tr("Zoom Out Once"));
  connect(m_zoomOutOnceAction, SIGNAL(triggered()), this, SLOT(zoomOutOnce()));

  // Add a pan Map button to the toolbar.
  m_panAction = new QAction(QIcon(":/images/pan.png"), tr("&Pan"), this);
  m_panAction->setShortcut(tr("Pan"));
  m_panAction->setStatusTip(tr("Pan map"));
  m_panAction->setCheckable(true);
  connect(m_panAction, SIGNAL(triggered()), this, SLOT(pan()));

  // Add a grap Map button to the toolbar.
  m_grabAction = new QAction(QIcon(":/images/grab.png"), tr("&Grab"), this);
  m_grabAction->setShortcut(tr("Grab"));
  m_grabAction->setStatusTip(tr("Grab Map"));
  m_grabAction->setCheckable(true);
  connect(m_grabAction, SIGNAL(triggered()), this, SLOT(grab()));

  // Add a zoom Map button to the toolbar.
  m_zoomAction = new QAction(QIcon(":/images/zoom.png"), tr("&Zoom"), this);
  m_zoomAction->setShortcut(tr("Zoom"));
  m_zoomAction->setStatusTip(tr("Zoom Map"));
  m_zoomAction->setCheckable(true);
  connect(m_zoomAction, SIGNAL(triggered()), this, SLOT(zoom()));

  // Create a group of actions... so that we can have a toggle group.
  m_actionGroup = new QActionGroup(ui->mainToolBar);
  m_actionGroup->addAction(m_panAction);
  m_actionGroup->addAction(m_grabAction);
  m_actionGroup->addAction(m_zoomAction);

  // Add a previous view button to the toolbar.
  m_previousViewAction = new QAction(QIcon(":/images/previous_view.png"), tr("&Previous View"), this);
  m_previousViewAction->setShortcut(tr("Previous View"));
  m_previousViewAction->setStatusTip(tr("Previous View"));
  connect(m_previousViewAction, SIGNAL(triggered()), this, SLOT(previousView()));

  // Add a next view button to the toolbar.
  m_nextViewAction = new QAction(QIcon(":/images/next_view.png"), tr("&Next View"), this);
  m_nextViewAction->setShortcut(tr("Next View"));
  m_nextViewAction->setStatusTip(tr("Next View"));
  connect(m_nextViewAction, SIGNAL(triggered()), this, SLOT(nextView()));

  // Add a view one button to the toolbar.
  m_viewOneAction = new QAction(QIcon(":/images/view1.png"), tr("&View One"), this);
  m_viewOneAction->setShortcut(tr("View One"));
  m_viewOneAction->setStatusTip(tr("View One"));
  connect(m_viewOneAction, SIGNAL(triggered()), this, SLOT(viewOne()));

  // Add a view two button to the toolbar.
  m_viewTwoAction = new QAction(QIcon(":/images/view2.png"), tr("&View Two"), this);
  m_viewTwoAction->setShortcut(tr("View Two"));
  m_viewTwoAction->setStatusTip(tr("View Two"));
  connect(m_viewTwoAction, SIGNAL(triggered()), this, SLOT(viewTwo()));

  // Add a view three button to the toolbar.
  m_viewThreeAction = new QAction(QIcon(":/images/view3.png"), tr("&View Three"), this);
  m_viewThreeAction->setShortcut(tr("View Three"));
  m_viewThreeAction->setStatusTip(tr("View Three"));
  connect(m_viewThreeAction, SIGNAL(triggered()), this, SLOT(viewThree()));

  // Create a group of actions... so we can enable/disable them as a group.
  m_viewActionGroup = new QActionGroup(ui->mainToolBar);
  m_viewActionGroup->addAction(m_previousViewAction);
  m_viewActionGroup->addAction(m_nextViewAction);
  m_viewActionGroup->addAction(m_viewOneAction);
  m_viewActionGroup->addAction(m_viewTwoAction);
  m_viewActionGroup->addAction(m_viewThreeAction);

  // disable the whole group.
  m_previousViewAction->setEnabled(false);
  m_nextViewAction->setEnabled(false);
  m_viewOneAction->setEnabled(false);
  m_viewTwoAction->setEnabled(false);
  m_viewThreeAction->setEnabled(false);

  // Add the actions to the toolbar.
  // Note: Icon's: Add alpha channel using GIMP - select fuzzy select tool and delete outer edges.
  ui->mainToolBar->addAction(m_openFileAction);
  ui->mainToolBar->addSeparator();
  ui->mainToolBar->addAction(m_resetMapAction);
  ui->mainToolBar->addSeparator();
  ui->mainToolBar->addAction(m_zoomInOnceAction);
  ui->mainToolBar->addAction(m_zoomOutOnceAction);
  ui->mainToolBar->addSeparator();
  ui->mainToolBar->addAction(m_panAction);
  ui->mainToolBar->addAction(m_grabAction);
  ui->mainToolBar->addAction(m_zoomAction);
  ui->mainToolBar->addSeparator();
  ui->mainToolBar->addAction(m_previousViewAction);
  ui->mainToolBar->addAction(m_nextViewAction);
  ui->mainToolBar->addSeparator();
  ui->mainToolBar->addAction(m_viewOneAction);
  ui->mainToolBar->addAction(m_viewTwoAction);
  ui->mainToolBar->addAction(m_viewThreeAction);
  ui->mainToolBar->addSeparator();

  // Limit size of tool bar icons
  QSize iconSize;
  iconSize.setHeight(24);
  iconSize.setWidth(24);
  ui->mainToolBar->setIconSize(iconSize);

  // Status Window
  m_label = new QLabel(this);
  m_label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_label->setText("Position: ");
  m_label->setAlignment(Qt::AlignBottom | Qt::AlignRight);

  ui->statusBar->addPermanentWidget(m_label);
  ui->mapLinkWidget->statusLabel(m_label);
}

MainWindow::~MainWindow()
{
  delete ui;
  delete m_resetMapAction;
  delete m_openFileAction;
  delete m_zoomInOnceAction;
  delete m_zoomOutOnceAction;
  delete m_panAction;
  delete m_grabAction;
  delete m_zoomAction;
  delete m_previousViewAction;
  delete m_nextViewAction;
  delete m_viewOneAction;
  delete m_viewTwoAction;
  delete m_viewThreeAction;
  delete m_actionGroup;
  delete m_viewActionGroup;
  delete m_label;
}

void MainWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
  default:
      break;
  }
}

void MainWindow::open()
{
  // MapLink Pro maps either have a '.map' or '.mpc' file ending.
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), QString(), tr("All Map files (*.map *.mpc)"));

  if (!fileName.isEmpty())
  {
    ui->mapLinkWidget->loadMap(fileName);
  }
}

void MainWindow::about()
{
  QMessageBox::about(this, tr("MapLink Pro Sample"),
                          tr("<h2>MapLink Pro : OpenGL Accelerator Simple Interaction Sample</h2>"
                                          "<p>Copyright &copy;2013 Envitia PLC"));
}

void MainWindow::resetMap()
{
  ui->mapLinkWidget->reset();
}

void MainWindow::zoomInOnce()
{
  ui->mapLinkWidget->zoomInOnce();
}

void MainWindow::zoomOutOnce()
{
  ui->mapLinkWidget->zoomOutOnce();
}
void MainWindow::pan()
{
  ui->mapLinkWidget->pan();
}
void MainWindow::grab()
{
  ui->mapLinkWidget->grab();
}
void MainWindow::zoom()
{
  ui->mapLinkWidget->zoom();
}
void MainWindow::previousView()
{
  ui->mapLinkWidget->previousView();
}
void MainWindow::nextView()
{
  ui->mapLinkWidget->nextView();
}
void MainWindow::viewOne()
{
  ui->mapLinkWidget->viewOne();
}
void MainWindow::viewTwo()
{
  ui->mapLinkWidget->viewTwo();
}
void MainWindow::viewThree()
{
  ui->mapLinkWidget->viewThree();
}

void MainWindow::resetViews()
{
  ui->mapLinkWidget->resetViews();
  // disable the whole group.
  m_previousViewAction->setEnabled(false);
  m_nextViewAction->setEnabled(false);
  m_viewOneAction->setEnabled(false);
  m_viewTwoAction->setEnabled(false);
  m_viewThreeAction->setEnabled(false);
}

void MainWindow::setViewOne()
{
  if (ui->mapLinkWidget->setViewOne())
  {
    m_previousViewAction->setEnabled(true);
    m_nextViewAction->setEnabled(true);
    m_viewOneAction->setEnabled(true);
  }
}

void MainWindow::setViewTwo()
{
  if (ui->mapLinkWidget->setViewTwo())
  {
    m_previousViewAction->setEnabled(true);
    m_nextViewAction->setEnabled(true);
    m_viewTwoAction->setEnabled(true);
  }
}

void MainWindow::setViewThree()
{
  if (ui->mapLinkWidget->setViewThree())
  {
    m_previousViewAction->setEnabled(true);
    m_nextViewAction->setEnabled(true);
    m_viewThreeAction->setEnabled(true);
  }
}
