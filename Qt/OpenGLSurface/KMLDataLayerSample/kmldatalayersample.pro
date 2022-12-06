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

TARGET = kmldatalayersample
QT += opengl gui

win32 {
  message("Windows build")
  CONFIG += windows
  INCLUDEPATH += $$quote($${MAPLINK_INCLUDE_DIR}) $$[QT_INSTALL_PREFIX]/include/QtGui/$$[QT_VERSION]/QtGui
  
  LIBS += $$quote($${MAPLINK_LIB_DIR}/MapLink$${MLS}) \
		$$quote($${MAPLINK_LIB_DIR}/MapLinkIMode$${MLS}) \
    $$quote($${MAPLINK_LIB_DIR}/MapLinkOpenGLSurface$${MLS}) \
    $$quote($${MAPLINK_LIB_DIR}/MapLink2DKML$${MLS}) \
    opengl32.lib
    
  # Use the appropriate libraries depending on whether this is a 32 or 64bit build of Qt
  contains( QMAKE_HOST.arch, x86_64 ) {
    message( "64bit Qt build detected" )
	  QMAKE_LFLAGS *= /MACHINE:X64
  } else {
    message( "32bit Qt build detected" )
	  QMAKE_LFLAGS *= /MACHINE:X86
  }
  
  DEFINES += TTLDLL WINNT _CRT_SECURE_NO_WARNINGS MAPLINK_DONT_INCLUDE_GL_HEADERS
  
  CharacterSet = 1
   
  RC_FILE = kmldatalayersample.rc
  TEMPLATE = vcapp
  DESTDIR = $$PWD/$$QMAKE_HOST.arch
  OBJECTS_DIR = $$PWD/$$QMAKE_HOST.arch
}

unix {
  message("Unix build")
  CONFIG += x11extras
  INCLUDEPATH += $${MAPLINK_INCLUDE_DIR} $$[QT_INSTALL_PREFIX]/include/QtGui/$$[QT_VERSION]/QtGui
  
  LIBS += -L$$MAPLINK_LIB_DIR
  
  LIBS += -lMapLink -lMapLinkIModes -lMapLinkOpenGLSurface -lMapLink2DKML -lKMLDrawingLibrary -lkmlbase -lkmldom -lkmlengine -lkmlconvenience
  DEFINES += X11_BUILD
  TEMPLATE = app
} 


# Common Input
FORMS = kmldatalayersample.ui
HEADERS = maplinkwidget.h mainwindow.h application.h attributetreewidget.h
SOURCES = main.cpp mainwindow.cpp maplinkwidget.cpp application.cpp attributetreewidget.cpp
RESOURCES = MapLink.qrc
