cmake_minimum_required(VERSION 3.5)

# set project name
set( APP_NAME_qpixmapexample maplinkqpixmapexample )
project(${APP_NAME_qpixmapexample} LANGUAGES CXX)

# set parameters
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

############ Envitia ############
# set MapLink definitions
include ("../../maplinkqtdefs.cmake")
if(NOT WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()
# setup the default build type.
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE )
endif()

# set include directories and link libraries
if(WIN32)
	#message("Windows build")
	# include directories
	include_directories(${MAPLINK_INCLUDE_DIR})
	# link directories
	link_directories(${MAPLINK_LIB_DIR})
	# link libraries
	set(link_libs_qpixmapexample MapLink${MLS})
	# add definitions
	add_definitions(-DTTLDLL -DWINNT -D_CRT_SECURE_NO_WARNINGS)
elseif (UNIX)
	#message("Unix build")
	# include directories
	include_directories(${MAPLINK_INCLUDE_DIR})
	# link directories
	find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core X11Extras REQUIRED)
	link_directories(${MAPLINK_LIB_DIR})
	# link libraries
	set(link_libs_qpixmapexample MapLink Qt${QT_VERSION_MAJOR}::X11Extras)
endif()
	
# set source files
set(sources 
    main.cpp
	)
	
# add source files to the executable
if(ANDROID)
  add_library(${APP_NAME_qpixmapexample} SHARED
    ${sources}
  )
else()
  add_executable(${APP_NAME_qpixmapexample}
    ${sources}
  )
endif()

# link app to link libraries
target_link_libraries(${APP_NAME_qpixmapexample} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${link_libs_qpixmapexample})

# install
install(TARGETS ${APP_NAME_qpixmapexample} COMPONENT runtime DESTINATION ${MAPLINK_ROOT_DIR}/bin64)