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

TARGET = openglc2sample
QT += opengl gui 

win32 {
  message("Windows build")
  CONFIG += windows
  INCLUDEPATH += ui \
				 $$quote($${MAPLINK_INCLUDE_DIR})
  
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
  
  DEFINES += TTLDLL WINNT _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES MAPLINK_DONT_INCLUDE_GL_HEADERS WIN32_LEAN_AND_MEAN NOMINMAX
  CharacterSet = 1
  
  RC_FILE = openglc2sample.rc
  TEMPLATE = vcapp
  DESTDIR = $$PWD/$$QMAKE_HOST.arch
  OBJECTS_DIR = $$PWD/$$QMAKE_HOST.arch
}

unix {
  message("Unix build")
  QT += x11extras
  INCLUDEPATH += $${MAPLINK_INCLUDE_DIR} ui $$[QT_INSTALL_PREFIX]/include/QtGui/$$[QT_VERSION]/QtGui
  
  LIBS += -L$$MAPLINK_LIB_DIR
  
  LIBS += -lMapLink -lMapLinkIModes -lMapLinkOpenGLSurface -lX11
  DEFINES += X11_BUILD
  TEMPLATE = app
} 

# Common Input
FORMS = ui/designerfiles/mainwindow.ui ui/designerfiles/toolbarspeedcontrol.ui ui/designerfiles/tracknumbers.ui
HEADERS = ui/maplinkglsurfacewidget.h ui/mainwindow.h ui/toolbarspeedcontrol.h ui/fractionspinbox.h ui/trackselectionmode.h ui/trackhostilitydelegate.h ui/tracknumbers.h layers/decluttermodel.h layers/layermanager.h layers/frameratelayer.h layers/tracklayer.h layers/textureatlas.h layers/glhelpers.h layers/shaders.h tracks/trackmanager.h tracks/track.h tracks/trackupdater.h tracks/trackinfomodel.h tracks/pinnedtrackmodel.h tracks/trackannotationenum.h
SOURCES = main.cpp ui/mainwindow.cpp ui/maplinkglsurfacewidget.cpp ui/toolbarspeedcontrol.cpp ui/fractionspinbox.cpp layers/decluttermodel.cpp ui/trackselectionmode.cpp ui/trackhostilitydelegate.cpp ui/tracknumbers.cpp layers/layermanager.cpp layers/frameratelayer.cpp layers/tracklayer.cpp layers/textureatlas.cpp layers/glhelpers.cpp tracks/trackmanager.cpp tracks/track.cpp tracks/trackupdater.cpp tracks/trackinfomodel.cpp tracks/pinnedtrackmodel.cpp
RESOURCES = ui/images.qrc
