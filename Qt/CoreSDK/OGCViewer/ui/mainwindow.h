/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include "ui_mainwindow.h"

#include "tslloaderstatus.h"
#include "tslloadercallbackreturn.h"

namespace Services
{
  class ServiceList;
};
class QMovie;
class QLabel;
class QActionGroup;
class HelpDialog;

class TSLEnvelope;

// The main application window

class MainWindow : public QMainWindow, private Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    static TSLLoaderCallbackReturn loadCallback( void* arg, const char* filename, TSLEnvelope extent, TSLLoaderStatus status, int percentDone );
    static void allLoadedCallback( void *arg );

    // Loads and saves application state such as the window size on startup/shutdown.
    void readSettings();
    void writeSettings();

    // Used by dialogs created elsewhere in the application that need to set the main window
    // as their parent.
    static QMainWindow* mainWindowInstance();

signals:
    void signalSetLoadingAnimationState( bool running );

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
  void showServiceWizard();
  void setLoadingAnimationState( bool running );
  void zoomViewToLayer();
  void showAboutBox();
  void showGeneralOptions();
  void showHelp();

private:
    QActionGroup *m_interactionModesGroup;

    // Status bar widgets
    QLabel *m_dataLoadingLabel;
    QMovie *m_dataLoadingAnimation;
    QLabel *m_mapUnitCursorPosition;
    QLabel *m_latLonCursorPosition;

    // This holds all the loaded services in the viewer and acts as the centre of the application
    Services::ServiceList *m_services;
#ifdef HAVE_QWEBVIEW
    HelpDialog *m_helpDialog;
#endif
};

#endif // MAINWINDOW_H
