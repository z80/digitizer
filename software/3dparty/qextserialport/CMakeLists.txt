#find_package( Qt4 )
#include( ${QT_USE_FILE} )

    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    FIND_PACKAGE(Qt5Widgets)
    FIND_PACKAGE(Qt5Core)
    #FIND_PACKAGE(Qt5Xml)
    FIND_PACKAGE(Qt5Svg)
    SET(CMAKE_AUTOMOC ON)

include_directories( "src"
                     "${CMAKE_CURRENT_BINARY_DIR}/src" )

# Includes.
list( APPEND headers "./src/qextserialport.h" )
list( APPEND headers "./src/qextserialenumerator.h" )
list( APPEND headers "./src/qextserialport_global.h" )

list( APPEND headers "./src/qextserialenumerator_p.h" )
list( APPEND headers "./src/qextserialport_p.h" )

list( APPEND com_src "./src/qextserialenumerator.cpp" )
list( APPEND com_src "./src/qextserialport.cpp" )
if( WIN32 )
    list( APPEND com_src "./src/qextserialenumerator_win.cpp" )
    list( APPEND com_src "./src/qextserialport_win.cpp" )
    set( libs setupapi.lib advapi32.lib user32.lib )
    set( defs _TTY_WIN_ Q_OS_WIN )
else( WIN32 )
    list( APPEND com_src "./src/qextserialenumerator_linux.cpp" )
    list( APPEND com_src "./src/qextserialport_unix.cpp" )
    set( defs _TTY_POSIX_ __linux__ Q_OS_UNIX QESP_NO_UDEV )
    set( libs "" )
endif( WIN32 )
message( STATUS "SERIAL DEFS: ${defs}" )

qt5_wrap_cpp( moc_src ${headers} )
#set( com_src ${com_src} ${moc_src} )

add_library( serial STATIC ${com_src} )
target_link_libraries( serial ${QT_LIBRARIES} ${libs} )
qt5_use_modules( serial Widgets Core )
set_property( TARGET serial PROPERTY DEFINE_SYMBOL ${defs} )
set_property( TARGET serial PROPERTY COMPILE_DEFINITIONS ${defs} )

message( STATUS "configuring serial" )
