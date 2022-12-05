/****************************************************************************
                Copyright (c) 2008-2017 by Envitia Group PLC.
****************************************************************************/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QLabel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:
    void open();
    void about();
    void resetMap();
    void zoomInOnce();
    void zoomOutOnce();
    void pan();
    void grab();
    void zoom();
    void previousView();
    void nextView();
    void viewOne();
    void viewTwo();
    void viewThree();

    void resetViews();
    void setViewOne();
    void setViewTwo();
    void setViewThree();

private:
    QAction *m_openFileAction;
    QAction *m_resetMapAction;
    QAction *m_zoomInOnceAction;
    QAction *m_zoomOutOnceAction;
    QAction *m_panAction;
    QAction *m_grabAction;
    QAction *m_zoomAction;
    QAction *m_previousViewAction;
    QAction *m_nextViewAction;
    QAction *m_viewOneAction;
    QAction *m_viewTwoAction;
    QAction *m_viewThreeAction;

    QActionGroup *m_actionGroup;
    QActionGroup *m_viewActionGroup;

    QLabel *m_label;

};

#endif // MAINWINDOW_H
