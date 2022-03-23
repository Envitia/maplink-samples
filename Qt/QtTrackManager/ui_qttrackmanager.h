/********************************************************************************
** Form generated from reading UI file 'qttrackmanager.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTTRACKMANAGER_H
#define UI_QTTRACKMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "maplinkwidget.h"

QT_BEGIN_NAMESPACE

class Ui_QtTrackManagerClass
{
public:
    QAction *action_Open;
    QAction *actionReset;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionZoom_Mode;
    QAction *actionPan_Mode;
    QAction *actionGrab_Mode;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionStart_Tracks;
    QAction *actionStop_Tracks;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    MapLinkWidget *maplinkWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTools;
    QMenu *menuTracks;
    QMenu *menuHelp;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *QtTrackManagerClass)
    {
        if (QtTrackManagerClass->objectName().isEmpty())
            QtTrackManagerClass->setObjectName(QString::fromUtf8("QtTrackManagerClass"));
        QtTrackManagerClass->resize(875, 653);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QtTrackManagerClass->sizePolicy().hasHeightForWidth());
        QtTrackManagerClass->setSizePolicy(sizePolicy);
        QtTrackManagerClass->setMinimumSize(QSize(800, 600));
        QtTrackManagerClass->setBaseSize(QSize(800, 600));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/MapLink.png"), QSize(), QIcon::Normal, QIcon::Off);
        QtTrackManagerClass->setWindowIcon(icon);
        action_Open = new QAction(QtTrackManagerClass);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/file_open.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Open->setIcon(icon1);
        action_Open->setShortcutContext(Qt::WidgetShortcut);
        actionReset = new QAction(QtTrackManagerClass);
        actionReset->setObjectName(QString::fromUtf8("actionReset"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReset->setIcon(icon2);
        actionZoom_In = new QAction(QtTrackManagerClass);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/zoomin_once.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_In->setIcon(icon3);
        actionZoom_Out = new QAction(QtTrackManagerClass);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/zoomout_once.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Out->setIcon(icon4);
        actionZoom_Mode = new QAction(QtTrackManagerClass);
        actionZoom_Mode->setObjectName(QString::fromUtf8("actionZoom_Mode"));
        actionZoom_Mode->setCheckable(true);
        actionZoom_Mode->setChecked(true);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/zoom.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Mode->setIcon(icon5);
        actionPan_Mode = new QAction(QtTrackManagerClass);
        actionPan_Mode->setObjectName(QString::fromUtf8("actionPan_Mode"));
        actionPan_Mode->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/pan.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPan_Mode->setIcon(icon6);
        actionGrab_Mode = new QAction(QtTrackManagerClass);
        actionGrab_Mode->setObjectName(QString::fromUtf8("actionGrab_Mode"));
        actionGrab_Mode->setCheckable(true);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/grab.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGrab_Mode->setIcon(icon7);
        actionExit = new QAction(QtTrackManagerClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(QtTrackManagerClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionStart_Tracks = new QAction(QtTrackManagerClass);
        actionStart_Tracks->setObjectName(QString::fromUtf8("actionStart_Tracks"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart_Tracks->setIcon(icon8);
        actionStop_Tracks = new QAction(QtTrackManagerClass);
        actionStop_Tracks->setObjectName(QString::fromUtf8("actionStop_Tracks"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop_Tracks->setIcon(icon9);
        centralWidget = new QWidget(QtTrackManagerClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(4, 4, 4, 4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        maplinkWidget = new MapLinkWidget(centralWidget);
        maplinkWidget->setObjectName(QString::fromUtf8("maplinkWidget"));
        sizePolicy.setHeightForWidth(maplinkWidget->sizePolicy().hasHeightForWidth());
        maplinkWidget->setSizePolicy(sizePolicy);
        maplinkWidget->setMinimumSize(QSize(800, 600));
        maplinkWidget->setBaseSize(QSize(800, 600));
        maplinkWidget->setLayoutDirection(Qt::LeftToRight);

        gridLayout->addWidget(maplinkWidget, 0, 0, 1, 1);

        QtTrackManagerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtTrackManagerClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 875, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuTracks = new QMenu(menuBar);
        menuTracks->setObjectName(QString::fromUtf8("menuTracks"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        QtTrackManagerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtTrackManagerClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setOrientation(Qt::Horizontal);
        QtTrackManagerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuTracks->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(action_Open);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuTools->addAction(actionZoom_In);
        menuTools->addAction(actionZoom_Out);
        menuTools->addAction(actionReset);
        menuTools->addSeparator();
        menuTools->addAction(actionZoom_Mode);
        menuTools->addAction(actionPan_Mode);
        menuTools->addAction(actionGrab_Mode);
        menuTracks->addAction(actionStart_Tracks);
        menuTracks->addAction(actionStop_Tracks);
        menuHelp->addAction(actionAbout);
        mainToolBar->addAction(action_Open);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionZoom_In);
        mainToolBar->addAction(actionZoom_Out);
        mainToolBar->addAction(actionReset);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionZoom_Mode);
        mainToolBar->addAction(actionPan_Mode);
        mainToolBar->addAction(actionGrab_Mode);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionStart_Tracks);
        mainToolBar->addAction(actionStop_Tracks);

        retranslateUi(QtTrackManagerClass);

        QMetaObject::connectSlotsByName(QtTrackManagerClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtTrackManagerClass)
    {
        QtTrackManagerClass->setWindowTitle(QApplication::translate("QtTrackManagerClass", "Qt Track Manager Example", nullptr));
        action_Open->setText(QApplication::translate("QtTrackManagerClass", "&Open", nullptr));
#ifndef QT_NO_SHORTCUT
        action_Open->setShortcut(QApplication::translate("QtTrackManagerClass", "Alt+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionReset->setText(QApplication::translate("QtTrackManagerClass", "Reset", nullptr));
#ifndef QT_NO_TOOLTIP
        actionReset->setToolTip(QApplication::translate("QtTrackManagerClass", "Reset map", nullptr));
#endif // QT_NO_TOOLTIP
        actionZoom_In->setText(QApplication::translate("QtTrackManagerClass", "Zoom In", nullptr));
#ifndef QT_NO_TOOLTIP
        actionZoom_In->setToolTip(QApplication::translate("QtTrackManagerClass", "Zoom In", nullptr));
#endif // QT_NO_TOOLTIP
        actionZoom_Out->setText(QApplication::translate("QtTrackManagerClass", "Zoom Out", nullptr));
#ifndef QT_NO_TOOLTIP
        actionZoom_Out->setToolTip(QApplication::translate("QtTrackManagerClass", "Zoom Out", nullptr));
#endif // QT_NO_TOOLTIP
        actionZoom_Mode->setText(QApplication::translate("QtTrackManagerClass", "Zoom Mode", nullptr));
#ifndef QT_NO_TOOLTIP
        actionZoom_Mode->setToolTip(QApplication::translate("QtTrackManagerClass", "Zoom Mode", nullptr));
#endif // QT_NO_TOOLTIP
        actionPan_Mode->setText(QApplication::translate("QtTrackManagerClass", "Pan Mode", nullptr));
#ifndef QT_NO_TOOLTIP
        actionPan_Mode->setToolTip(QApplication::translate("QtTrackManagerClass", "Pan to point", nullptr));
#endif // QT_NO_TOOLTIP
        actionGrab_Mode->setText(QApplication::translate("QtTrackManagerClass", "Grab Mode", nullptr));
#ifndef QT_NO_TOOLTIP
        actionGrab_Mode->setToolTip(QApplication::translate("QtTrackManagerClass", "Grab and drag the map", nullptr));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("QtTrackManagerClass", "Exit", nullptr));
        actionAbout->setText(QApplication::translate("QtTrackManagerClass", "About", nullptr));
        actionStart_Tracks->setText(QApplication::translate("QtTrackManagerClass", "Start Tracks", nullptr));
        actionStop_Tracks->setText(QApplication::translate("QtTrackManagerClass", "Stop Tracks", nullptr));
        menuFile->setTitle(QApplication::translate("QtTrackManagerClass", "&File", nullptr));
        menuTools->setTitle(QApplication::translate("QtTrackManagerClass", "Tools", nullptr));
        menuTracks->setTitle(QApplication::translate("QtTrackManagerClass", "Tracks", nullptr));
        menuHelp->setTitle(QApplication::translate("QtTrackManagerClass", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtTrackManagerClass: public Ui_QtTrackManagerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTTRACKMANAGER_H
