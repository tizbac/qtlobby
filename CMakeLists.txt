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

add_definitions(-DSVN_REV)

add_subdirectory(./src)