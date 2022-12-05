#****************************************************************************
#Copyright (c) 2008-2022 by Envitia Group PLC.
#
#This program is free software: you can redistribute it and/or modify it 
#under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your 
#option) any later version.

#This program is distributed in the hope that it will be useful, but WITHOUT 
#ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
#FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
#for more details.

#You should have received a copy of the GNU Lesser General Public License 
#along with this program. If not, see <https://www.gnu.org/licenses/>.

# Only release configurations are generated - on Windows debug builds can only be used if the same version
# of Visual Studio that MapLink was built with is used.
# On windows only release or debug should be added to CONFIG. Using debug_and_release will generate 
# an invalid project due to the way conditions are evaluated by qmake.
CONFIG -=  debug_and_release release debug
CONFIG += qt thread release

# The CONFIG variable must be setup before including maplinkqtdefs.pri
include(../maplinkqtdefs.pri)

TARGET = maplinkqttrackmanagerexample

QT += widgets xml

win32 {
  message("Windows build")
  CONFIG += windows
  INCLUDEPATH += $$quote($${MAPLINK_INCLUDE_DIR})\
                 $$quote($${MAPLINK_INCLUDE_DIR}/trackmanager)
  
  LIBS += $$quote($${MAPLINK_LIB_DIR}/MapLink$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLinkIMode$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLinkTrackManager$${MLS})
    
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
   
  RC_FILE = qttrackmanager.rc
   
  TEMPLATE = vcapp
  DESTDIR = $$PWD/$$QMAKE_HOST.arch
  OBJECTS_DIR = $$PWD/$$QMAKE_HOST.arch
}

unix {
  message("Unix build")
  greaterThan(QT_MAJOR_VERSION, 4): QT += x11extras
  INCLUDEPATH += $${MAPLINK_INCLUDE_DIR} $${MAPLINK_INCLUDE_DIR}/trackmanager
  
  LIBS += -L$$MAPLINK_LIB_DIR
  
  LIBS += -lMapLink -lMapLinkIModes -lMapLinkTrackManager
  TEMPLATE = app
} 

# Common Input
FORMS = qttrackmanager.ui
HEADERS = maplinkwidget.h mainwindow.h application.h \
    configurationsettings.h \
    trackinformation.h \
    displaytrack.h \
    trackssimulator.h \
    symbolset.h
SOURCES = main.cpp mainwindow.cpp maplinkwidget.cpp application.cpp \
	configurationsettings.cpp \
    displaytrack.cpp \
    trackssimulator.cpp
RESOURCES = MapLink.qrc
