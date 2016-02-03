

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Network)
FIND_PACKAGE(Qt5Help)
find_package(Qt5PrintSupport REQUIRED)
SET(CMAKE_AUTOMOC ON)

include_directories( "./src" )

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


include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
add_library( libhelp STATIC ${src} )
qt5_use_modules( libhelp Widgets Core Network Help PrintSupport )
