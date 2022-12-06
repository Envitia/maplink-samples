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

TARGET = advancedsample

QT += opengl gui 

# Some features of this sample are optional
CONFIG += MapLinkRealtimeReprojection MapLinkDirectImportSDK

# Required for qplatformnativeinterface.h
# This will become a part of Qt-Platform-Abstraction
INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/QtGui/$$[QT_VERSION]/QtGui

win32 {
  message("Windows build")

  CONFIG += windows warn_off
  INCLUDEPATH += $$quote($${MAPLINK_INCLUDE_DIR})
  CONFIG -= flat
  
  LIBS += $$quote($${MAPLINK_LIB_DIR}/MapLink$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLinkIMode$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLinkOpenGLSurface$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLinkOpenGLTrackHelper$${MLS}) \
          $$quote($${MAPLINK_LIB_DIR}/MapLink2DKML$${MLS}) \
          opengl32.lib
  
  contains( QMAKE_HOST.arch, x86_64 ) {
    message( "64bit Qt build detected" )
    QMAKE_LFLAGS *= /MACHINE:X64
  } else {
    message( "32bit Qt build detected" )
    QMAKE_LFLAGS *= /MACHINE:X86
  }
  
  DEFINES += TTLDLL WINNT _CRT_SECURE_NO_WARNINGS MAPLINK_DONT_INCLUDE_GL_HEADERS
  CharacterSet = 1
   
  RC_FILE = advancedsample.rc
   
  TEMPLATE = vcapp
  DESTDIR = $$PWD/$$QMAKE_HOST.arch
  OBJECTS_DIR = $$PWD/$$QMAKE_HOST.arch
}

unix {
  message("Unix build")

  CONFIG += warn_off
  QT += x11extras
  INCLUDEPATH += $${MAPLINK_INCLUDE_DIR}
  LIBS += -L$$MAPLINK_LIB_DIR
  
  LIBS += -lMapLink -lMapLinkIModes -lMapLinkOpenGLSurface -lMapLinkOpenGLTrackHelper -lMapLink2DKML -lKMLDrawingLibrary -lkmlbase -lkmldom -lkmlengine -lkmlconvenience
  DEFINES += X11_BUILD
  TEMPLATE = app
} 

MapLinkRealtimeReprojection {
  message("Building with Realtime Reprojection enabled")
  DEFINES += ENABLE_PROJECTION
}

RESOURCES = MapLink.qrc

FORMS = ui/designerfiles/advancedsample.ui \
        ui/designerfiles/cachesizedialog.ui \
        ui/designerfiles/datalayerdialog.ui \
        ui/designerfiles/tmflayerdialog.ui \
        ui/designerfiles/toolbarspeedcontrol.ui \
        ui/designerfiles/urldialog.ui \
        ui/tracks/designerfiles/tracknumbers.ui

HEADERS = AddWaypointInteractionMode.h \
          application.h \
          layermanager.h \
          mainwindow.h \
          maplinkwidget.h \
          Util.h \
          layers/decluttermodel.h \
          layers/frameratelayer.h \
          layers/glhelpers.h \
          layers/shaders.h \
          layers/textureatlas.h \
          layers/TrackCustomDataLayer.h \
          layers/tracklayer.h \
          tracks/pinnedtrackmodel.h \
          tracks/trackannotationenum.h \
          tracks/track.h \
          tracks/trackinfomodel.h \
          tracks/trackmanager.h \
          tracks/trackupdater.h \
          ui/cachesizedialog.h \
          ui/datalayerdialog.h \
          ui/tmflayerdialog.h \
          ui/urldialog.h \
          ui/toolbarspeedcontrol.h \
          ui/attributetreewidget/attributetreewidget.h \
          ui/layertreeview/layertreeview.h \
          ui/layertreeview/treeitem.h \
          ui/layertreeview/treemodel.h \
          ui/tracks/trackhostilitydelegate.h \
          ui/tracks/tracknumbers.h \
          ui/tracks/trackselectionmode.h

SOURCES = main.cpp \
          AddWaypointInteractionMode.cpp \
          application.cpp \
          layermanager.cpp \
          mainwindow.cpp \
          maplinkwidget.cpp \
          layers/decluttermodel.cpp \
          layers/frameratelayer.cpp \
          layers/glhelpers.cpp \
          layers/textureatlas.cpp \
          layers/TrackCustomDataLayer.cpp \
          layers/tracklayer.cpp \
          tracks/pinnedtrackmodel.cpp \
          tracks/track.cpp \
          tracks/trackinfomodel.cpp \
          tracks/trackmanager.cpp \
          tracks/trackupdater.cpp \
          ui/cachesizedialog.cpp \
          ui/datalayerdialog.cpp \
          ui/tmflayerdialog.cpp \
          ui/toolbarspeedcontrol.cpp \
          ui/urldialog.cpp \
          ui/attributetreewidget/attributetreewidget.cpp \
          ui/layertreeview/layertreeview.cpp \
          ui/layertreeview/treeitem.cpp \
          ui/layertreeview/treemodel.cpp \
          ui/tracks/trackhostilitydelegate.cpp \
          ui/tracks/tracknumbers.cpp \
          ui/tracks/trackselectionmode.cpp

MapLinkDirectImportSDK {
  message("Building with support for MapLink DirectImport SDK")
  
  win32 {
	LIBS += $$quote($${MAPLINK_LIB_DIR}/MapLinkDirectImport$${MLS})
  }
  unix {
    LIBS += -lMapLinkDirectImport
  }
  DEFINES += HAVE_DIRECT_IMPORT_SDK

  FORMS += ui/directimportwizard/designerfiles/directimportwizard.ui \
          ui/directimportwizard/designerfiles/newlayerpage.ui \
          ui/directimportwizard/designerfiles/scalebandspage.ui \
          ui/directimportwizard/designerfiles/datasetinformationpage.ui \
          ui/directimportwizard/designerfiles/datasetcreationoptionspage.ui \
          ui/directimportwizard/designerfiles/datasetcreationfailedpage.ui

  HEADERS += \
            ui/directimportwizard/directimportwizard.h \
            ui/directimportwizard/newlayerpage.h \
            ui/directimportwizard/scalebandspage.h \
            ui/directimportwizard/scalebandstable.h \
            ui/directimportwizard/datasetinformationpage.h \
            ui/directimportwizard/datasetcreationoptionspage.h \
            ui/directimportwizard/datasetcreationfailedpage.h \
            ui/directimportwizard/wizardpageenum.h \
            ui/directimportwizard/scalebandstablemodel.h \
            ui/layertreeview/directimportscalebandtreeitem.h \
            ui/layertreeview/directimportdatasettreeitem.h

  SOURCES += \
            ui/directimportwizard/directimportwizard.cpp \
            ui/directimportwizard/newlayerpage.cpp \
            ui/directimportwizard/scalebandspage.cpp \
            ui/directimportwizard/scalebandstable.cpp \
            ui/directimportwizard/datasetinformationpage.cpp \
            ui/directimportwizard/datasetcreationoptionspage.cpp \
            ui/directimportwizard/datasetcreationfailedpage.cpp \
            ui/directimportwizard/scalebandstablemodel.cpp \
            ui/layertreeview/directimportscalebandtreeitem.cpp \
            ui/layertreeview/directimportdatasettreeitem.cpp
}
