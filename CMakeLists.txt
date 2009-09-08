# ./CMakeLists.txt

project(QtLobby) # the name of your project

cmake_minimum_required(VERSION 2.6)
set(QT_USE_QTCORE true)
set(QT_USE_QTGUI true)
set(QT_USE_QTDBUS true)
set(QT_USE_QTSCRIPT true)
set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)
set(QT_USE_QTXML true)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )
# TODO: abstract qtdbus
find_package(Qt4 COMPONENTS QtCore QtGui QtDbus QtScript QtNetwork QtOpenGL QtXml REQUIRED) # find and setup Qt4 for this project
find_package(QScintilla REQUIRED)
find_package(QtScriptTools REQUIRED)

set(QTLOBBY_VERSION 1.0alpha)

if($ENV{SVN_REV})
  set(SVN_REV $ENV{SVN_REV})
  message(STATUS Overriding SVN_REV from environment variable)
else($ENV{SVN_REV})
  execute_process(COMMAND svnversion -n OUTPUT_VARIABLE SVN_REV)
endif($ENV{SVN_REV})


option(BUILDBOT "Use buildbot build configuration(used for cross compiling)" OFF)
mark_as_advanced(BUILDBOT)
if(WIN32 OR BUILDBOT)
  option(PURE_WINAPI_UNITSYNC_LOADER "Use pure Win API unitsync loader(helps against some loading bugs)" ON)
  mark_as_advanced(PURE_WINAPI_UNITSYNC_LOADER)
endif(WIN32 OR BUILDBOT)

CONFIGURE_FILE( ${CMAKE_SOURCE_DIR}/src/config.h.cmake ${CMAKE_SOURCE_DIR}/src/config.h )

add_subdirectory(src)
