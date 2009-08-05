# - Try to find the QtScriptTools includes and library
# which defines
# 
# QTSCRIPTTOOLS_FOUND - system has QtScriptTools
# QTSCRIPTTOOLS_INCLUDE_DIR - where to find qscriptenginedebugger.h
# QTSCRIPTTOOLS_LIBRARIES - the libraries to link against to use QtScriptTools
# QTSCRIPTTOOLS_LIBRARY - where to find the QtScriptTools library (not for general use)

# copyright (c) 2009 Marcel Hauf <marcel.hauf@ŋooglemail.com>
#
# Redistribution and use is allowed according to the terms of the FreeBSD license.
if(NOT QT4_FOUND)
  include(FindQt4)
endif(NOT QT4_FOUND)

set(QTSCRIPTTOOLS_FOUND FALSE)

if(QT4_FOUND)
  find_path(QTSCRIPTTOOLS_INCLUDE_DIR qscriptenginedebugger.h 
  "${QT_INCLUDE_DIR}/QtScriptTools" /usr/include /usr/include/QtScriptTools
  )
  find_library(QTSCRIPTTOOLS_LIBRARY
    NAMES QtScriptTools libQtScriptTools
    PATHS /usr/lib /usr/lib64
  )
  
  if(QTSCRIPTTOOLS_LIBRARY AND QTSCRIPTTOOLS_INCLUDE_DIR)
    set(QTSCRIPTTOOLS_LIBRARIES ${QTSCRIPTTOOLS_LIBRARY})
    set(QTSCRIPTTOOLS_FOUND TRUE)
    
  endif(QTSCRIPTTOOLS_LIBRARY AND QTSCRIPTTOOLS_INCLUDE_DIR)
endif(QT4_FOUND)

if(QTSCRIPTTOOLS_FOUND)
  if (NOT QTSCRIPTTOOLS_FIND_QUIETLY)
    message(STATUS "Found QTSCRIPTTOOLS : ${QTSCRIPTTOOLS_LIBRARY}")
  endif (NOT QTSCRIPTTOOLS_FIND_QUIETLY)
else(QTSCRIPTTOOLS_FOUND)
  if(QTSCRIPTTOOLS_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find QtScriptTools")
  endif(QTSCRIPTTOOLS_FIND_REQUIRED)
endif(QTSCRIPTTOOLS_FOUND)

mark_as_advanced(QTSCRIPTTOOLS_INCLUDE_DIR QTSCRIPTTOOLS_LIBRARY)