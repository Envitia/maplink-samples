#****************************************************************************
#                Copyright (c) 2017 by Envitia Group PLC.
#****************************************************************************

# Only release configurations are generated - on Windows debug builds can only be used if the same version
# of Visual Studio that MapLink was built with is used.
# On windows only release or debug should be added to CONFIG. Using debug_and_release will generate 
# an invalid project due to the way conditions are evaluated by qmake.
CONFIG -=  debug_and_release release debug
CONFIG += qt thread release

# The CONFIG variable must be setup before including maplinkqtdefs.pri
dev {
  include(../../maplinkqtdefs.pri)
} else {
  include(../maplinkqtdefs.pri)
}

TARGET = OGCServiceViewer
QT += widgets 

win32 {
  message("Windows build")
  CONFIG += windows

  INCLUDEPATH += $$quote($${MAPLINK_INCLUDE_DIR})
  
  LIBS += $$quote($${MAPLINK_LIB_DIR}/MapLink$${MLS}) \
		$$quote($${MAPLINK_LIB_DIR}/MapLinkIMode$${MLS}) \
    $$quote($${MAPLINK_LIB_DIR}/MapLinkWMTSDataLayer$${MLS})
    
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
   
  RC_FILE = ogcviewer.rc
  TEMPLATE = vcapp
  DESTDIR = $$PWD/$$QMAKE_HOST.arch
  OBJECTS_DIR = $$PWD/$$QMAKE_HOST.arch
}

unix {
  message("Unix build")
  QT += x11extras 
  INCLUDEPATH += $${MAPLINK_INCLUDE_DIR}
  
  LIBS += -L$$MAPLINK_LIB_DIR

  DEFINES += MAPLINK_NO_DRAWING_SURFACE
  
  LIBS += -lMapLink -lMapLinkIModes -lMapLinkWMSDL -lMapLinkWMTSDL -lMapLinkRemoteLoader -lX11
  TEMPLATE = app
} 

# Common Input
FORMS = ui/designerfiles/mainwindow.ui \
        ui/designerfiles/servicewizard.ui \
		ui/designerfiles/editdimensiondialog.ui \
		ui/designerfiles/layerproperties.ui \
		ui/designerfiles/generaloptionsdialog.ui \
        ui/designerfiles/credentialsdialog.ui \
		ui/designerfiles/wizardpages/actiontype.ui \
		ui/designerfiles/wizardpages/coordinatesystemselect.ui \
		ui/designerfiles/wizardpages/dimensions.ui \
		ui/designerfiles/wizardpages/selectlayers.ui \
		ui/designerfiles/wizardpages/wmsserviceaddress.ui \
		ui/designerfiles/wizardpages/wmtsserviceaddress.ui \
		ui/designerfiles/wizardpages/wmsserviceoptions.ui \
		ui/designerfiles/wizardpages/wmtsserviceoptions.ui
		
HEADERS = services/service.h \
		  services/servicelist.h \
		  services/servicelistmodel.h \
		  services/servicelayerpreview.h \
		  services/wms/wmsservice.h \
		  services/wms/wmsservicelayermodel.h \
		  services/wms/wmsservicelayerstylesmodel.h \
		  services/wms/wmsservicelayerinfomodel.h \
		  services/wms/wmsservicedimensionsmodel.h \
		  services/wms/wmsservicedimensioninfomodel.h \
		  services/wms/wmslayerpreview.h \
		  services/wmts/wmtsservice.h \
		  services/wmts/wmtsservicelayermodel.h \
		  services/wmts/wmtsservicelayerinfomodel.h \
		  services/wmts/wmtsservicelayerstylesmodel.h \
		  services/wmts/wmtsservicedimensionsmodel.h \
		  services/wmts/wmtsservicedimensioninfomodel.h \
		  services/wmts/wmtslayerpreview.h \
          ui/mainwindow.h \
		  ui/drawingsurfacewidget.h \
		  ui/drawingsurfaceinteractions.h \
		  ui/servicewizard.h \
		  ui/servicetreeview.h \
		  ui/datedelegate.h \
		  ui/prescribedvaluedelegate.h \
		  ui/editdimensiondialog.h \
		  ui/layerpropertiesdialog.h \
		  ui/generaloptionsdialog.h \
          ui/credentialsdialog.h \
		  ui/pages/servicewizardpageenum.h \
		  ui/pages/actiontypepage.h \
		  ui/pages/addwmsservicepage.h \
		  ui/pages/addwmtsservicepage.h \
		  ui/pages/dimensionspage.h \
		  ui/pages/selectcoordsyspage.h \
		  ui/pages/selectlayerspage.h \
		  ui/pages/wmsserviceoptionspage.h \
		  ui/pages/wmtsserviceoptionspage.h
		  
SOURCES = main.cpp \
          services/service.cpp \
		  services/servicelist.cpp \
		  services/servicelistmodel.cpp \
		  services/servicelayerpreview.cpp \
		  services/wms/wmsservice.cpp \
		  services/wms/wmsservicelayermodel.cpp \
		  services/wms/wmsservicelayerstylesmodel.cpp \
		  services/wms/wmsservicelayerinfomodel.cpp \
		  services/wms/wmsservicedimensionsmodel.cpp \
		  services/wms/wmsservicedimensioninfomodel.cpp \
		  services/wms/wmslayerpreview.cpp \
		  services/wmts/wmtsservice.cpp \
		  services/wmts/wmtsservicelayermodel.cpp \
		  services/wmts/wmtsservicelayerinfomodel.cpp \
		  services/wmts/wmtsservicelayerstylesmodel.cpp \
		  services/wmts/wmtsservicedimensionsmodel.cpp \
		  services/wmts/wmtsservicedimensioninfomodel.cpp \
		  services/wmts/wmtslayerpreview.cpp \
          ui/mainwindow.cpp \
		  ui/drawingsurfacewidget.cpp \
		  ui/drawingsurfaceinteractions.cpp \
		  ui/servicewizard.cpp \
		  ui/servicetreeview.cpp \
		  ui/datedelegate.cpp \
		  ui/prescribedvaluedelegate.cpp \
		  ui/editdimensiondialog.cpp \
		  ui/layerpropertiesdialog.cpp \
		  ui/generaloptionsdialog.cpp \
          ui/credentialsdialog.cpp \
		  ui/pages/actiontypepage.cpp \
		  ui/pages/addwmsservicepage.cpp \
		  ui/pages/addwmtsservicepage.cpp \
		  ui/pages/dimensionspage.cpp \
		  ui/pages/selectcoordsyspage.cpp \
		  ui/pages/selectlayerspage.cpp \
		  ui/pages/wmsserviceoptionspage.cpp \
		  ui/pages/wmtsserviceoptionspage.cpp
		 
#win32 {
#  QT += webkitwidgets
#  FORMS += ui/designerfiles/helpdialog.ui
#  HEADERS += ui/helpdialog.h
#  SOURCES += ui/helpdialog.cpp
#  DEFINES += HAVE_QWEBVIEW
#}
 
RESOURCES = ui/images.qrc

INCLUDEPATH += ui ui/pages
