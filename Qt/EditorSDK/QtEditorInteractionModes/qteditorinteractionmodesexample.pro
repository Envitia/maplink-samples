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
  include(../../../maplinkqtdefs.pri)
} else {
  include(../../maplinkqtdefs.pri)
}

TARGET = maplinkqteditorinteractionmodesexample

QT += widgets

win32 {
  message("Windows build")
  CONFIG += windows
  INCLUDEPATH += $$quote($${MAPLINK_INCLUDE_DIR})
  
  LIBS += $$quote($${MAPLINK_LIB_DIR}/MapLink$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLinkIMode$${MLS}) \
		  $$quote($${MAPLINK_LIB_DIR}/MapLinkEDT$${MLS}) \
		  $$quote($${MAPLINK_LIB_DIR}/MapLinkEDTIMode$${MLS}) \
		  $$quote($${MAPLINK_LIB_DIR}/RenderingAttributePanel$${MLS})
    
  # Use the appropriate libraries depending on whether this is a 32 or 64bit build of Qt
  contains( QMAKE_HOST.arch, x86_64 ) {
    message( "64bit Qt build detected" )
    QMAKE_LFLAGS *= /MACHINE:X64
  } else {
    message( "32bit Qt build detected" )
    QMAKE_LFLAGS *= /MACHINE:X86
  }
  
  DEFINES += TTLDLL WINNT _CRT_SECURE_NO_WARNINGS USE_RENDERING_ATTRIBUTES_PANEL
  CharacterSet = 1
   
  RC_FILE = qteditorinteractionmodes.rc
   
  TEMPLATE = vcapp
  DESTDIR = $$PWD/$$QMAKE_HOST.arch
  OBJECTS_DIR = $$PWD/$$QMAKE_HOST.arch
}

unix {
  message("Unix build")
  greaterThan(QT_MAJOR_VERSION, 4): QT += x11extras
  INCLUDEPATH += $${MAPLINK_INCLUDE_DIR}
  
  LIBS += -L$$MAPLINK_LIB_DIR
  
  LIBS += -lMapLink -lMapLinkIModes -lMapLinkEDT -lMapLinkEDTIMode
  TEMPLATE = app
} 

# Common Input
FORMS = qteditorinteractionmodes.ui textentrydialog.ui
HEADERS = maplinkwidget.h mainwindow.h application.h textentrydialog.h 
SOURCES = main.cpp mainwindow.cpp maplinkwidget.cpp application.cpp textentrydialog.cpp
RESOURCES = MapLink.qrc
