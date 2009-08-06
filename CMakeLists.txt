# ./CMakeLists.txt

project(QtLobby) # the name of your project

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules )
# TODO: abstract qtdbus
find_package(Qt4 COMPONENTS QtCore QtGui QTDBUS QTSCRIPT QTNETWORK QtOpenGL QtXml REQUIRED) # find and setup Qt4 for this project
find_package(QScintilla REQUIRED)
find_package(QtScriptTools REQUIRED)

add_definitions(-DSVN_REV)

add_subdirectory(./src)