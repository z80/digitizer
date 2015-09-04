
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



ADD_CUSTOM_COMMAND( 
    OUTPUT ${ROOT}/digitizer/src/icee/factory.cpp 
    COMMAND ${ICE_PATH}/bin/slice2cpp -I${ICE_PATH}/slice factory.ice 
    WORKING_DIRECTORY ${ROOT}/digitizer/src/icee )
list( APPEND src ${ROOT}/digitizer/src/icee/factory.cpp )

ADD_CUSTOM_COMMAND( 
    OUTPUT ${ROOT}/digitizer/src/icee/digitizer.cpp
    COMMAND ${ICE_PATH}/bin/slice2cpp -I${ICE_PATH}/slice digitizer.ice 
    WORKING_DIRECTORY ${ROOT}/digitizer/src/icee )
list( APPEND src ${ROOT}/digitizer/src/icee/digitizer.cpp )

include_directories( ${ROOT}/digitizer/src/icee )

find_library( ice      NAMES ice.lib      PATHS "${ICE_LIB_PATH}" )
find_library( iceutil  NAMES iceutil.lib  PATHS "${ICE_LIB_PATH}" )
find_library( iced     NAMES iced.lib     PATHS "${ICE_LIB_PATH}" )
find_library( iceutild NAMES iceutild.lib PATHS "${ICE_LIB_PATH}" )
set( libs debug ${iced} 
          optimized ${ice} 
          debug ${iceutild} 
          optimized ${iceutil} 
    )

add_library( libdigitremote STATIC ${src} )
qt5_use_modules( libdigitremote Widgets Core Network )
set( libs ${libs} ${QT_LIBRARIES} )
target_link_libraries( libdigitremote ${libs} )
#set_property( TARGET libdigitremote PROPERTY COMPILE_DEFINITIONS ICEE_STATIC_LIBS )
install( TARGETS libdigitremote DESTINATION ./ )



