/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/
#include <QtGui>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <string>
using namespace std;

#include "mainwindow.h"


MainWindow::MainWindow() : m_maplinkWidget(NULL)
{
  m_openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
  m_openAct->setShortcut(tr("Ctrl+O"));
  m_openAct->setStatusTip(tr("Open an existing file"));
  connect(m_openAct, SIGNAL(triggered()), this, SLOT(open()));

  m_fileMenu = menuBar()->addMenu(tr("&File"));
  m_fileMenu->addAction(m_openAct);

  // Set the desired OpenGL context attributes
  QGLFormat format;
  format.setRgba(true);
  format.setDoubleBuffer(true);
  format.setAccum(false);
  format.setSamples(false);
  format.setDirectRendering(true);
  format.setDepth(true);
  format.setAlpha(true);
  format.setAlphaBufferSize(8);
  format.setDepthBufferSize(24);
  format.setGreenBufferSize(8);
  format.setRedBufferSize(8);
  format.setBlueBufferSize(8);

  // MapLink Widget
  m_maplinkWidget = new MapLinkWidget( format ); 

  // Default size of area to draw
  m_maplinkWidget->setMinimumSize(900, 700); 

  //
  setCentralWidget(m_maplinkWidget);
}  

void MainWindow::closeEvent(QCloseEvent *)
{
}

void MainWindow::open()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), QString(), tr("All Map files (*.map *.mpc)"));

  if (!fileName.isEmpty())
  {
    m_maplinkWidget->loadMap(fileName);
  }
}


