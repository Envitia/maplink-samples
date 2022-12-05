/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "maplinkwidget.h"
#include <string>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  void loadMap(const char *filename)
  {
    if (m_maplinkWidget)
      m_maplinkWidget->loadMap(filename);
  }
protected:
  void closeEvent(QCloseEvent *event);

private slots:
    void open();

private:
  MapLinkWidget *m_maplinkWidget;
  
  QAction *m_openAct;
  QMenu *m_fileMenu;
};

#endif
