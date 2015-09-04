
find_package( Qt4 )
set( QT_USE_QTCORE    True )
set( QT_USE_QTGUI     True )
set( QT_USE_QTNETWORK True )
if ( WIN32 )
    set( QT_USE_QTMAIN True )
endif ( WIN32 )
include( ${QT_USE_FILE} )

include_directories( "./src"
                     "./src/devices"
                     "../src"
                     ${ICE_PATH}/cppe/include 
                     ${BOOST_PATH}
                     "../factory_ice" 
                     "${ROOT_PATH}/humidity/libhumidity/src" 
                     "${ROOT_PATH}/magnet/libmagnet/src" 
                     "${ROOT_PATH}/spectrum/libandor/src" 
                     ${CMAKE_CURRENT_BINARY_DIR} )
                     
aux_source_directory( "./src" src )
# Запуск slice2cpp для построения файлов, необходимых для компиляции проекта.
ADD_CUSTOM_COMMAND( 
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../factory_ice/factory.cpp 
    COMMAND ${ICE_PATH}/cpp/bin/slice2cppe -I${ICE_PATH}/slice factory.ice 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../factory_ice )
list( APPEND src ${CMAKE_CURRENT_SOURCE_DIR}/../factory_ice/factory.cpp )

set( libs "" )
set( defs ICEE_STATIC_LIBS )
foreach( device ${FACTORY_DEVICES} )
    string( TOLOWER ${device} filePattern )
    set( libs ${libs} lib${filePattern} )
    ADD_CUSTOM_COMMAND( 
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../factory_ice/${filePattern}.cpp
        COMMAND ${ICE_PATH}/cpp/bin/slice2cppe -I${ICE_PATH}/slice ${filePattern}.ice 
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../factory_ice )
    list( APPEND src ${CMAKE_CURRENT_SOURCE_DIR}/../factory_ice/${filePattern}.cpp )
    list( APPEND src ${CMAKE_CURRENT_SOURCE_DIR}/src/devices/${filePattern}_ice.cpp )
    string( TOUPPER ${device} definitionPattern )
    set( defs ${defs} ${definitionPattern} )
endforeach( device )

QT4_WRAP_CPP( moc_src "./src/setup_dlg.h" "./src/host_tray.h" )
list( APPEND src ${moc_src} )

QT4_WRAP_UI( ui_src "./src/setup_dlg.ui" )
list( APPEND src ${ui_src} )

QT4_ADD_RESOURCES( qrc_src "./src/images.qrc" )
list( APPEND src ${qrc_src} )

if ( WIN32 )
    set( BLD_TYPE WIN32 )
    list( APPEND src "./src/icon.rc" )
elseif ( UNIX )
    set( BLD_TYPE "" )
endif (WIN32)

add_executable( factory_host ${BLD_TYPE} ${src} )

find_library( icee      NAMES icee_static.lib   PATHS "${ICE_PATH}/cppe/lib" )
find_library( iceec     NAMES iceec_static.lib  PATHS "${ICE_PATH}/cppe/lib" )
find_library( iceed     NAMES icee_staticd.lib  PATHS "${ICE_PATH}/cppe/lib" )
find_library( iceecd    NAMES iceec_staticd.lib PATHS "${ICE_PATH}/cppe/lib" )

set_property( TARGET factory_host PROPERTY COMPILE_DEFINITIONS ${defs} )
set( libs ${libs} debug ${iceed} optimized ${icee} ${QT_LIBRARIES} )
if ( WIN32 )
    find_library( rpcrt4    NAMES rpcrt4.lib   PATHS  "${PL_SDK}/lib" )
    find_library( ws2_32    NAMES ws2_32.lib   PATHS  "${PL_SDK}/lib" )
    list( APPEND libs ${rpcrt4} ${ws2_32} ) 
endif ( WIN32 )
target_link_libraries( factory_host ${libs} )

install( TARGETS factory_host DESTINATION "factory" )


