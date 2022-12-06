/****************************************************************************
                Copyright (c) 2017 by Envitia Group PLC.
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Care must be taken when including Qt, osgEarth and MapLink together on
// non-windows platforms.
// There are several typedefs in the X11 headers (Bool, Status, Type) 
// which collide with Qt and osgEarth definitions.
#include "ui_osgearthsample.h"
#include "osgearthsampleconfig.h"

#include <QMainWindow>
#include <QActionGroup>
#include <QLabel>

// Collision between X11 'None' and QEvent::Type::None
#ifdef None
# undef None
#endif
#include <QtCore/QEvent>
#include <osgEarthQt/ViewerWidget>
#include <osgEarthUtil/Sky>


#include <tsldatalayertypeenum.h>

class MaplinkTrackManager;

class MainWindow : public QMainWindow, private Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


    bool initMapLink();
    bool initOsgEarth(osg::ArgumentParser& args);


private slots:
    void openMapLinkData();
    void exit();
    void showAboutBox();
    void setSkyBoxMoon( bool enabled );
    void setSkyBoxAnimated( bool enabled );
    void setLighting( bool enabled );
    void setDecluttering(bool enabled);
    void showSimulationOptions();

private:
  bool addMapLinkData( const char* fileName, TSLDataLayerTypeEnum layerType, bool limitZoomDisplay = true ); 
  bool addMapLinkTerrainData( const char* fileName );

  osg::ref_ptr<osgEarth::Map> m_osgEarthMap;
  osg::ref_ptr<osgViewer::Viewer> m_osgViewer;
  osgEarth::QtGui::ViewerWidget* m_earthViewer;
  osg::ref_ptr<osgEarth::MapNode> m_mapNode;
  osg::ref_ptr<osg::Group> m_rootNode;

  MaplinkTrackManager* m_trackManager;
  osg::ref_ptr<osgEarth::Util::SkyNode> m_skyNode;
  bool m_skyBoxMoonEnabled;
  bool m_skyBoxAnimationEnabled;
};

struct AnimateSunCallback : public osg::NodeCallback
{
  void operator()( osg::Node* node, osg::NodeVisitor* nv )
  {
    osgEarth::Util::SkyNode* skyNode = static_cast<osgEarth::Util::SkyNode*>(node);
    double hours = fmod( osg::Timer::instance()->time_s()/2.0, 24.0 );
    skyNode->setDateTime( osgEarth::DateTime(SUN_YEAR, SUN_MONTH, SUN_DAY, hours) );
  }
};

#endif // MAINWINDOW_H
