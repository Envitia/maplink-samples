/****************************************************************************
                Copyright (c) 2013-2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_simpleglsample.h"

class MainWindow : public QMainWindow, private Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadFile( const char *fileToLoad );
private slots:
    void loadFile();
    void resetView();
    void zoomInOnce();
    void zoomOutOnce();
    void activatePanMode();
    void activateGrabMode();
    void activateZoomMode();
    void enableBufferedLayerTiling(bool enable);
    void showAboutBox();
    void exit();

private:
    QActionGroup *m_interactionModesGroup;
};

#endif // MAINWINDOW_H
