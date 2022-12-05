#****************************************************************************
#                Copyright (c) 2017 by Envitia Group PLC.
#****************************************************************************

# Only release configurations are generated - on Windows debug builds can only be used if the same version
# of Visual Studio that MapLink was built with is used.
# On windows only release or debug should be added to CONFIG. Using debug_and_release will generate 
# an invalid project due to the way conditions are evaluated by qmake.
CONFIG -=  debug_and_release release debug
CONFIG += qt thread widgets release

# The CONFIG variable must be setup before including maplinkqtdefs.pri
include(../maplinkqtdefs.pri)

TARGET = maplinkosgearthsample

CONFIG += qt thread
QT += opengl gui widgets
DEFINES += MAPLINK_HAVE_KML

win32 {
  message("Windows build")
  CONFIG += windows
  
  INCLUDEPATH += $$quote($${MAPLINK_INCLUDE_DIR})
  INCLUDEPATH += $$quote($${MAPLINK_OSG_INCLUDE_DIR}) \
                 $$quote($${MAPLINK_OSG_GEN_INCLUDE_DIR}) \
                 $$quote($${MAPLINK_OSGEARTH_INCLUDE_DIR}) 
  
  LIBS += $$quote($${MAPLINK_LIB_DIR}/MapLinkOsgEarth$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLink$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/ttlterrain$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLink2DKML$${MLS})
          
  LIBS += $$quote($${MAPLINK_OSG_LIB_DIR}/osgEarth$${OSGLS}) \
          $$quote($${MAPLINK_OSG_LIB_DIR}/osgEarthUtil$${OSGLS}) \
          $$quote($${MAPLINK_OSG_LIB_DIR}/osgEarthQt$${OSGLS}) \
          $$quote($${MAPLINK_OSG_LIB_DIR}/osgEarthAnnotation$${OSGLS}) \
          $$quote($${MAPLINK_OSG_LIB_DIR}/osgEarthSymbology$${OSGLS}) \
          $$quote($${MAPLINK_OSG_LIB_DIR}/osg$${OSGLS}) \
          $$quote($${MAPLINK_OSG_LIB_DIR}/osgGA$${OSGLS}) \
          $$quote($${MAPLINK_OSG_LIB_DIR}/osgQt$${OSGLS}) \
          $$quote($${MAPLINK_OSG_LIB_DIR}/osgViewer$${OSGLS}) \
          $$quote($${MAPLINK_OSG_LIB_DIR}/OpenThreads$${OSGLS}) \
          $$quote($${MAPLINK_OSG_LIB_DIR}/osgEarthMapLink$${OSGLS})
    
  contains( QMAKE_HOST.arch, x86_64 ) {
    message( "64bit Qt build detected" )
    QMAKE_LFLAGS *= /MACHINE:X64
  } else {
    message( "32bit Qt build detected" )
    QMAKE_LFLAGS *= /MACHINE:X86
  }
    
  DEFINES += WINNT _CRT_SECURE_NO_WARNINGS
  CharacterSet = 1
  
  RC_FILE = osgearthsample.rc
   
  TEMPLATE = vcapp
  DESTDIR = $$PWD/$$QMAKE_HOST.arch
  OBJECTS_DIR = $$PWD/$$QMAKE_HOST.arch
}

unix {
  message("Unix build")
  INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include \
                 $$[QT_INSTALL_PREFIX]/include/QtGui/$$[QT_VERSION]/QtGui \
                 $(MAPL_HOME)/include \
                 $(MAPL_HOME)/thirdparty/include/osg \
                 $(MAPL_HOME)/thirdparty/include/osgearth
  LIBS += -L$$MAPLINK_LIB_DIR
  
  LIBS += -losgEarth \
          -losgEarthUtil  \
          -losgEarthQt  \
          -losgEarthAnnotation  \
          -losgEarthSymbology  \
          -losg  \
          -losgGA  \
          -losgQt  \
          -losgViewer \
          -lOpenThreads  \
          -losgDB  \
          -losgText  \
          -lMapLinkOsgEarth  \
          -lMapLink  \
          -lMapLinkTerrain  \
          -lMapLinkCADRGDL  \
          -lMapLink2DKML  \
          -losgEarthMapLink  \
          -lX11
  TEMPLATE = app

  # OsgEarth was built using GCC 4
  # This define is required when using GCC 5 as 
  # OsgEarth exposes std::string at the API.
  DEFINES += _GLIBCXX_USE_CXX11_ABI=0
} 

# Input files
FORMS = osgearthsample.ui simulationOptions.ui
HEADERS = mainwindow.h maplinktrackobject.h maplinktrackmanager.h simulationoptionsdialog.h osgearthsampleconfig.h viewereventfilter.h
SOURCES = main.cpp mainwindow.cpp maplinktrackobject.cpp maplinktrackmanager.cpp simulationoptionsdialog.cpp viewereventfilter.cpp
RESOURCES = MapLink.qrc
