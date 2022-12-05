#****************************************************************************
#                Copyright (c) 2008-2017 by Envitia Group PLC.
#****************************************************************************

# Only release configurations are generated - on Windows debug builds can only be used if the same version
# of Visual Studio that MapLink was built with is used.
# On windows only release or debug should be added to CONFIG. Using debug_and_release will generate 
# an invalid project due to the way conditions are evaluated by qmake.
CONFIG -=  debug_and_release release debug
CONFIG += qt thread debug

# The CONFIG variable must be setup before including maplinkqtdefs.pri
dev {
  include(../../maplinkqtdefs.pri)
} else {
  include(../maplinkqtdefs.pri)
}

TARGET = maplinkqtearthsample

QT += widgets

win32 {
  message("Windows build")
  CONFIG += windows
  INCLUDEPATH += $$quote($${MAPLINK_INCLUDE_DIR}) widgets widgets/surface widgets/controller 
  
  LIBS += $$quote($${MAPLINK_LIB_DIR}/MapLink$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLinkEarth$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/ttlterrain$${MLS})
    
  # Use the appropriate libraries depending on whether this is a 32 or 64bit build of Qt
  contains( QMAKE_HOST.arch, x86_64 ) {
    message( "64bit Qt build detected" )
    QMAKE_LFLAGS *= /MACHINE:X64
  } else {
    message( "32bit Qt build detected" )
    QMAKE_LFLAGS *= /MACHINE:X86
  }
  
  DEFINES += TTLDLL WINNT _CRT_SECURE_NO_WARNINGS
  CharacterSet = 1
   
  RC_FILE = qtearthsample.rc
   
  TEMPLATE = vcapp
  DESTDIR = $$PWD/$$QMAKE_HOST.arch
  OBJECTS_DIR = $$PWD/$$QMAKE_HOST.arch
}

unix {
  message("Unix build")
  greaterThan(QT_MAJOR_VERSION, 4): QT += x11extras
  INCLUDEPATH += $${MAPLINK_INCLUDE_DIR} widgets widgets/surface widgets/controller 
  
  LIBS += -L$$MAPLINK_LIB_DIR -Wl,-rpath,$$MAPLINK_LIB_DIR
  
  LIBS += -lMapLink -lMapLinkEarth -lMapLinkTerrain
  TEMPLATE = app
} 

# Common Input
FORMS = qtearthsample.ui widgets/surfacecontroller.ui widgets/controller/cameracontroller.ui widgets/screenshot/screenshotdialog.ui 
HEADERS = mainwindow.h widgets/surfacecontroller.h widgets/surface/maplinkwidget.h widgets/surface/application.h widgets/screenshot/screenshotdialog.h widgets/controller/cameracontroller.h managers/datalayers/datalayersmanager.h managers/geometry/geometrymanager.h managers/tracks/tracksmanager.h interactions/cameramanager.h interactions/createpolygoninteraction.h interactions/createpolylineinteraction.h interactions/createsymbolinteraction.h interactions/createtextinteraction.h interactions/deletegeometryinteraction.h interactions/interaction.h interactions/interactionmodemanager.h interactions/Interactionmoderequest.h interactions/MapLink3DIMode.h interactions/selectinteraction.h interactions/trackballviewinteraction.h
SOURCES = main.cpp mainwindow.cpp widgets/surfacecontroller.cpp widgets/surface/maplinkwidget.cpp widgets/surface/application.cpp widgets/screenshot/screenshotdialog.cpp widgets/controller/cameracontroller.cpp managers/datalayers/datalayersmanager.cpp managers/geometry/geometrymanager.cpp managers/tracks/tracksmanager.cpp interactions/cameramanager.cpp interactions/createpolygoninteraction.cpp interactions/createpolylineinteraction.cpp interactions/createsymbolinteraction.cpp interactions/createtextinteraction.cpp interactions/deletegeometryinteraction.cpp interactions/interaction.cpp interactions/interactionmodemanager.cpp interactions/Interactionmoderequest.cpp interactions/selectinteraction.cpp interactions/trackballviewinteraction.cpp
RESOURCES = MapLink.qrc
