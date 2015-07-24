
#find_package( Qt4 REQUIRED )
#set( QT_USE_QTGUI true )
#set( QT_USE_QTSVG true )
#include( ${QT_USE_FILE} )

    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    FIND_PACKAGE(Qt5Widgets)
    FIND_PACKAGE(Qt5Core)
    #FIND_PACKAGE(Qt5Xml)
    #FIND_PACKAGE(Qt5Svg)
    SET(CMAKE_AUTOMOC ON)


include_directories( "./src"
                     ${ROOT}/libio/src
                     ${ROOT}/3dparty/lua-5.1.5/src
                     ${ROOT}/3dparty/lua-5.1.5/etc
                     ${ROOT}/3dparty/libqtlua-2.0/src
                     ${ROOT}/3dparty/qwt-6.1.2/src
                     ${CMAKE_CURRENT_BINARY_DIR} )
if ( WIN32 )
    include_directories( ${BOOST_PATH} )
endif ( WIN32 )

file( GLOB src     "./src/*.cpp" )
file( GLOB headers "./src/*.h" )
file( GLOB ui      "./src/*.ui" )
file( GLOB qrc     "./src/*.qrc" )
message( STATUS "SRC = ${src}" )
message( STATUS "HEADERS = ${headers}" )
message( STATUS "ui = ${ui}" )

#qt5_wrap_cpp( moc_src ${headers} )
#set( src ${src} ${moc_src} )

qt5_wrap_ui( moc_src ${ui} )
set( src ${src} ${moc_src} )

qt5_add_resources( qrc_src ${qrc} )
set( src ${src} ${qrc_src} )

if ( WIN32 )
    list( APPEND src "src/icon.rc" )
    set( BLD_TYPE WIN32 )
else ( WIN32 )
    set( BLD_TYPE "" )
endif ( WIN32 )

add_executable( digitizer ${BLD_TYPE} ${src} )
qt5_use_modules( digitizer Widgets Core )
target_link_libraries( digitizer libio qwt-6.1.2 qtlua ${QT_LIBRARIES} Qt5::WinMain )
install( TARGETS   digitizer DESTINATION ./ )
install( DIRECTORY ./lua/ DESTINATION ./ )
