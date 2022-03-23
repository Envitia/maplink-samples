#****************************************************************************
#Copyright (c) 2008-2022 by Envitia Group PLC.
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.

****************************************************************************/

# Common definitions for MapLink Qt samples
# MAPLINK_LIB_DIR - Path to MapLink libraries
# MAPLINK_INCLUDE_DIR - Path to MapLink headers
# MAPLINK_ROOT_DIR - The root directory of the MapLink installation
#
# MAPLINK_OSG_INCLUDE_DIR - Include directory for Open Scene Graph
# MAPLINK_OSG_GEN_INCLUDE_DIR - Include directory for Open Scene Graph (Generated version/config headers)
# MAPLINK_OSGEARTH_INCLUDE_DIR - Include directory for OsgEarth
# MAPLINK_OSG_LIB_DIR - Path to MapLink OSG/OSGEarth libraries
#
# MLS - The MapLink lib suffix, used by all MapLink libraries
#    64-bit release - "64.lib"
#    64-bit debug   - "64d.lib"
# OSGLS - The OSG/OsgEarth lib suffix, used by OpenSceneGraph, OSGEarth, and some other third party libraries
#    64-bit release - ".lib"
#    64-bit debug   - "d.lib"

CONFIG += warn_off

win32 {
  # Build against installed MapLink:
  MAPLINK_ROOT_DIR="C:/Program Files/Envitia/MapLink Pro/11.0"
  # Build in source tree:
  #MAPLINK_ROOT_DIR="$$PWD/../.."

  contains( QMAKE_HOST.arch, x86_64 ) {
    MAPLINK_LIB_DIR = "$${MAPLINK_ROOT_DIR}/lib64"
    # Note: This conditional will not be evaluated correctly
    # for debug_and_release.
    CONFIG(debug,debug|release) {
      MLS="64d.lib"
      OSGLS = "d.lib"
    } else {
      MLS="64.lib"
      OSGLS=".lib"
    }
    MAPLINK_OSG_LIB_DIR = "$${MAPLINK_ROOT_DIR}/thirdparty/x86_64/lib/"
  } else {
    MAPLINK_LIB_DIR = "$${MAPLINK_ROOT_DIR}/lib"
    # Note: This conditional will not be evaluated correctly
    # for debug_and_release.
    CONFIG(debug,debug|release) {
      MLS="d.lib"
      OSGLS="d.lib"
    } else {
      MLS=".lib"
      OSGLS=".lib"
    }
    MAPLINK_OSG_LIB_DIR = "$${MAPLINK_ROOT_DIR}/thirdparty/x86/lib/"
  }

  MAPLINK_INCLUDE_DIR="$${MAPLINK_ROOT_DIR}/include"
  
  MAPLINK_OSG_INCLUDE_DIR="$${MAPLINK_ROOT_DIR}/thirdparty/include/osg"
  MAPLINK_OSG_GEN_INCLUDE_DIR="$${MAPLINK_ROOT_DIR}/thirdparty/include/osg"
  MAPLINK_OSGEARTH_INCLUDE_DIR="$${MAPLINK_ROOT_DIR}/thirdparty/include/osgearth"
}

unix {
  MAPLINK_ROOT_DIR="$(MAPL_HOME)"
  contains( QMAKE_HOST.arch, x86_64 ) {
    MAPLINK_LIB_DIR="$(MAPL_HOME)/lib64"
  } else {
    MAPLINK_LIB_DIR="$(MAPL_HOME)/lib"
  }
  MAPLINK_OSG_LIB_DIR = $$MAPLINK_LIB_DIR
  MAPLINK_INCLUDE_DIR="$(MAPL_HOME)/include"
}
