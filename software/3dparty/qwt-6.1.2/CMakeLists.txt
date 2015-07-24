
#find_package( Qt4 REQUIRED )
#set( QT_USE_QTGUI true )
#set( QT_USE_QTSVG true )
#include( ${QT_USE_FILE} )

    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    FIND_PACKAGE(Qt5Widgets )
    FIND_PACKAGE(Qt5Core)
    #FIND_PACKAGE(Qt5Xml)
    FIND_PACKAGE(Qt5Svg)
    SET(CMAKE_AUTOMOC ON)

include_directories( "./src"
                     ${CMAKE_BINARY_DIR} )

file( GLOB src     "./src/*.cpp" )
file( GLOB headers "./src/*.h" )
message( STATUS "SRC = ${src}" )
message( STATUS "HEADERS = ${headers}" )

qt5_wrap_cpp( moc_src ${headers} )
set( src ${src} ${moc_src} )

add_library( qwt-6.1.2 STATIC ${src} )
qt5_use_modules( qwt-6.1.2 Widgets Core Concurrent OpenGL Svg PrintSupport )
target_link_libraries( qwt-6.1.2 ${QT_LIBRARIES} )









