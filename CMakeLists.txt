# ./CMakeLists.txt

project(QtLobby) # the name of your project

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )

set(QTLOBBY_VERSION 1.0alpha)

if("$ENV{SVN_REV}" GREATER 0)
  set(SVN_REV $ENV{SVN_REV})
  message(STATUS Overriding SVN_REV from environment variable)
else("$ENV{SVN_REV}" GREATER 0)
  execute_process(COMMAND svnversion -n WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE SVN_REV)
endif("$ENV{SVN_REV}" GREATER 0)


option(BUILDBOT "Use buildbot build configuration(used for cross compiling)" OFF)
mark_as_advanced(BUILDBOT)

option(TRANSLATIONS "Create .qm files)" ON)

set(QT_USE_QTCORE true)
set(QT_USE_QTGUI true)
set(QT_USE_QTSCRIPT true)
set(QT_USE_QTNETWORK true)
set(QT_USE_QTOPENGL true)
set(QT_USE_QTXML true)
set(QT_USE_QTSQL true)

if(WIN32 OR BUILDBOT)
  option(PURE_WINAPI_UNITSYNC_LOADER "Use pure Win API unitsync loader(helps against some loading bugs)" ON)
  mark_as_advanced(PURE_WINAPI_UNITSYNC_LOADER)
  find_package(Qt4 COMPONENTS QtCore QtGui QtScript QtNetwork QtOpenGL QtXml QtSql REQUIRED) # -QtDbus
else(WIN32 OR BUILDBOT)
  set(QT_USE_QTDBUS true)
  find_package(Qt4 COMPONENTS QtCore QtGui QtDbus QtScript QtNetwork QtOpenGL QtXml QtSql REQUIRED)
endif(WIN32 OR BUILDBOT)

find_package(QScintilla REQUIRED)
find_package(QtScriptTools REQUIRED)

file(GLOB jobjol "${CMAKE_SOURCE_DIR}/share/qtlobby/icons/jobjol/?*.*")
install(FILES ${jobjol} DESTINATION share/qtlobby/icons/jobjol)
file(GLOB trolltech "${CMAKE_SOURCE_DIR}/share/qtlobby/icons/trolltech/?*.*")
install(FILES ${trolltech} DESTINATION share/qtlobby/icons/trolltech)
file(GLOB icons "${CMAKE_SOURCE_DIR}/share/qtlobby/icons/?*.*")
install(FILES ${icons} DESTINATION share/qtlobby/icons)
file(GLOB flags "${CMAKE_SOURCE_DIR}/share/qtlobby/flags/?*.*")
install(FILES ${flags} DESTINATION share/qtlobby/flags)
file(GLOB shaders "${CMAKE_SOURCE_DIR}/share/qtlobby/shaders/?*.*")
install(FILES ${shaders} DESTINATION share/qtlobby/shaders)
file(GLOB javascript "${CMAKE_SOURCE_DIR}/share/qtlobby/javascript/?*.*")
install(FILES ${javascript} DESTINATION share/qtlobby/javascript)


add_subdirectory(src)

