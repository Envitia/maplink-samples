====================================================
Overview

This sample provides a basic application that loads 
a shapefile via direct import and displays using x11.

====================================================

Build Instructions

- Set the "MAPL_HOME" environment variable to point at
  the MapLink installation

- Update the following paths in the .pro file with the
  correct path to your MapLink installation

  INCLUDEPATH += "<MaplinkInstallLocation>/include"
  LIBS += -L"<MaplinkInstallLocation>/lib64"

- Open the .pro file within QT5

- Update the path to the repo location on line 271 of
  UnixDirectImportExample.cpp

- Build using QT


===================================================

Known Issues

- This sample should provide a CMake build file for
  portability rather than a QT project file

- SLD file handling is limited within MapLink, if you
  are loading a shapefile with this sample and find it
  isn't rendering correctly, disable the sld loading.

- For debugging in QT, if you get any errors with driver
  loading,  ensure that the "MAPL_HOME_OVERRIDE"
  environment variable is set. It is unclear as to why
  the MAPL_HOME variable does not always perform as
  expected

==================================================

