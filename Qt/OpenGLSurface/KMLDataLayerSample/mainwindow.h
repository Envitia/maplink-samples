/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QLabel>
#include "ui_kmldatalayersample.h"

class MainWindow : public QMainWindow, private Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadMap( const char *mapToLoad );
    void loadKML( const char *kmlToLoad );
private slots:
    void loadMap();
    void loadKML();
    void resetView();
    void zoomInOnce();
    void zoomOutOnce();
    void activatePanMode();
    void activateGrabMode();
    void activateZoomMode();
    void showAboutBox();
    void exit();

private:
    QActionGroup *m_interactionModesGroup;
};

#endif // MAINWINDOW_H
