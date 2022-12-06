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

  // MapLink Widget
  m_maplinkWidget = new MapLinkWidget(); 
  m_maplinkWidget->setMinimumSize( 800, 600 );
  setCentralWidget(m_maplinkWidget);

  // Trigger an update of the MainWindow when the map is redrawn
  // This ensures that the MapLinkWidget is fully redrawn by Qt when needed
  // In more complex applications this would also ensure additional widgets are
  // updated if needed, based on any changes to the map display
  connect( m_maplinkWidget, SIGNAL(mapDrawn()), this, SLOT(update()) );
}  

void MainWindow::closeEvent(QCloseEvent *)
{
}

void MainWindow::open()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Map"), QString(), tr("All Map files (*.map *.mpc)"));

  if (!fileName.isEmpty())
  {
    m_maplinkWidget->loadMap( fileName.toUtf8() );
  }
}

