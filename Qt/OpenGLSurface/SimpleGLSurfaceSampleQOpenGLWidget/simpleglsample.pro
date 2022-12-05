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
win32 {
  include(../../maplinkqtdefs.pri)
}
unix {
  dev {
    include(../../maplinkqtdefs.pri)
  } else {
    include(../maplinkqtdefs.pri)
  }
}

TARGET = simpleglsampleqopenglwidget

QT += opengl

win32 {
  message("Windows build")
  CONFIG += windows
  INCLUDEPATH += $$quote($${MAPLINK_INCLUDE_DIR}) $$[QT_INSTALL_PREFIX]/include/QtGui/$$[QT_VERSION]/QtGui
  
  LIBS += $$quote($${MAPLINK_LIB_DIR}/MapLink$${MLS}) \
	  	  $$quote($${MAPLINK_LIB_DIR}/MapLinkIMode$${MLS}) \
		  $$quote($${MAPLINK_LIB_DIR}/MapLinkOpenGLSurface$${MLS}) \
		  opengl32.lib
		
  contains( QMAKE_HOST.arch, x86_64 ) {
    message( "64bit Qt build detected" )
	QMAKE_LFLAGS *= /MACHINE:X64
  } else {
    message( "32bit Qt build detected" )
	QMAKE_LFLAGS *= /MACHINE:X86
  }
  
  DEFINES += TTLDLL WINNT _CRT_SECURE_NO_WARNINGS
  CharacterSet = 1
  
  RC_FILE = simpleglsample.rc
   
  TEMPLATE = vcapp
  DESTDIR = $$PWD/$$QMAKE_HOST.arch
  OBJECTS_DIR = $$PWD/$$QMAKE_HOST.arch
}

unix {
  message("Unix build")
  QT += x11extras
  INCLUDEPATH += $${MAPLINK_INCLUDE_DIR} $$[QT_INSTALL_PREFIX]/include/QtGui/$$[QT_VERSION]/QtGui
  QMAKE_RPATHLINKDIR += :$${MAPLINK_LIB_DIR}
  LIBS += -L$$MAPLINK_LIB_DIR
  
  LIBS += -lMapLink -lMapLinkIModes -lMapLinkOpenGLSurface -lX11
  DEFINES += X11_BUILD
  TEMPLATE = app
} 

# Common Input
FORMS = simpleglsample.ui
HEADERS = maplinkwidget.h mainwindow.h application.h
SOURCES = main.cpp mainwindow.cpp maplinkwidget.cpp application.cpp
RESOURCES = MapLink.qrc
