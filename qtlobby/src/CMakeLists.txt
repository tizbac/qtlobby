include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(qtlobby_SRCS qtlobby.cpp main.cpp)
qt4_automoc(${qtlobby_SRCS})
add_executable(qtlobby ${qtlobby_SRCS})
target_link_libraries(qtlobby ${QT_QTCORE_LIBRARY})
