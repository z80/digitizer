
project( digitizer )
cmake_minimum_required(VERSION 2.8)

set( ROOT ${CMAKE_CURRENT_SOURCE_DIR} )
#set( CMAKE_INSTALL_PREFIX ${ROOT}/.build.app )
set( CMAKE_INSTALL_PREFIX ${ROOT}/app )
set( _3DPARTY ${CMAKE_CURRENT_SOURCE_DIR}/3dparty )

if ( WIN32 )
    include( "./settings.win32.cmake" )
elseif ( UNIX )
    include( "./settings.unix.cmake" )
endif ( WIN32 )

message( STATUS $ENV{PATH} )

add_subdirectory( "./3dparty" )
add_subdirectory( "./libio" )
#add_subdirectory( "./lib_lua_machine" )
add_subdirectory( "./test" )
add_subdirectory( "./digitizer" )
add_subdirectory( "./libextctrl" )
add_subdirectory( "./libluafactory" )
