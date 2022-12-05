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
win32 {
  include(../maplinkqtdefs.pri)
}
unix {
  dev {
    include(../maplinkqtdefs.pri)
  } else {
    include(maplinkqtdefs.pri)
  }
}

TARGET = maplinkqteventmanagerexample

QT += widgets

WebSocketEventManager_Include = "$${MAPLINK_INCLUDE_DIR}/../SDK/EventManagerSDK\src/api"
contains( QMAKE_HOST.arch, x86_64 ) {
  WebSocketEventManager_LibPath = "$${MAPLINK_INCLUDE_DIR}/../SDK/EventManagerSDK/build64/Release"
} else {
  WebSocketEventManager_LibPath = "$${MAPLINK_INCLUDE_DIR}/../SDK/EventManagerSDK/build32/Release"
}

win32 {
  message("Windows build")
  CONFIG += windows
  INCLUDEPATH += $$quote($${MAPLINK_INCLUDE_DIR})\
                 $$quote($${MAPLINK_INCLUDE_DIR}/trackmanager)\
                 $$quote($${WebSocketEventManager_Include})
  
  LIBS += $$quote($${MAPLINK_LIB_DIR}/MapLink$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLinkIMode$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLinkTrackManager$${MLS}) \
		  $$quote($${WebSocketEventManager_LibPath}/clientWebSocket.lib)
    
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
   
  RC_FILE = qteventmanager.rc
   
  TEMPLATE = vcapp
  DESTDIR = $$PWD/$$QMAKE_HOST.arch
  OBJECTS_DIR = $$PWD/$$QMAKE_HOST.arch
}

unix {
  message("Unix build")
  greaterThan(QT_MAJOR_VERSION, 4): QT += x11extras
  INCLUDEPATH += $${MAPLINK_INCLUDE_DIR} $${MAPLINK_INCLUDE_DIR}/trackmanager $${WebSocketEventManager_Include}
  
  LIBS += -L$$MAPLINK_LIB_DIR -L$${WebSocketEventManager_LibPath}
  
  LIBS += -lMapLink -lMapLinkIModes -lMapLinkTrackManager -lclientWebSocket
  TEMPLATE = app
} 

# Common Input
FORMS = qteventmanager.ui
HEADERS = maplinkwidget.h mainwindow.h application.h \
    interactionmodetracks.h \
    clientmanager.h \
    clientconnectionthread.h
SOURCES = main.cpp mainwindow.cpp maplinkwidget.cpp application.cpp \
    interactionmodetracks.cpp \
    clientmanager.cpp \
    clientconnectionthread.cpp
RESOURCES = MapLink.qrc
