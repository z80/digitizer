
#find_package( Qt4 REQUIRED )
#set( QT_USE_QTGUI true )
#set( QT_USE_QTSVG true )
#include( ${QT_USE_FILE} )

    SET(CMAKE_INCLUDE_CURRENT_DIR ON)
    FIND_PACKAGE(Qt5Widgets)
    FIND_PACKAGE(Qt5Core)
    FIND_PACKAGE(Qt5Network)
    #FIND_PACKAGE(Qt5Xml)
    #FIND_PACKAGE(Qt5Svg)
    SET(CMAKE_AUTOMOC ON)


include_directories( "./src"
                     ${ROOT}/libio/src
                     ${ROOT}/3dparty/qwt-6.1.2/src
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${ICE_PATH}/include )
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




ADD_CUSTOM_COMMAND( 
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/icee/factory.cpp 
    COMMAND ${ICE_PATH}/bin/slice2cpp -I${ICE_PATH}/slice factory.ice 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/icee )
list( APPEND src ${CMAKE_CURRENT_SOURCE_DIR}/src/icee/factory.cpp )

ADD_CUSTOM_COMMAND( 
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/icee/digitizer.cpp
    COMMAND ${ICE_PATH}/bin/slice2cpp -I${ICE_PATH}/slice digitizer.ice 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/icee )
list( APPEND src ${CMAKE_CURRENT_SOURCE_DIR}/src/icee/digitizer.cpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src/icee )

find_library( ice      NAMES ice.lib      PATHS "${ICE_LIB_PATH}" )
find_library( iceutil  NAMES iceutil.lib  PATHS "${ICE_LIB_PATH}" )
find_library( iced     NAMES iced.lib     PATHS "${ICE_LIB_PATH}" )
find_library( iceutild NAMES iceutild.lib PATHS "${ICE_LIB_PATH}" )
set( libs debug ${iced} 
          optimized ${ice} 
          debug ${iceutild} 
          optimized ${iceutil} 
    )

add_executable( digitizer ${BLD_TYPE} ${src} )
qt5_use_modules( digitizer Widgets Core Network )
set( libs ${libs} libio qwt-6.1.2 ${QT_LIBRARIES} )
if ( WIN32 )
    set( libs ${libs} Qt5::WinMain )
endif ( WIN32 )
target_link_libraries( digitizer ${libs} )
#set_property( TARGET digitizer PROPERTY COMPILE_DEFINITIONS ICEE_STATIC_LIBS )
install( TARGETS   digitizer DESTINATION ./ )



