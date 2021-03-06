
if ( UNIX )
    # To be able to load binary modules as shared libraries.
    add_definitions( -DLUA_DL_DLOPEN )
endif ( UNIX )

add_library( liblua-5.3.0  STATIC
                       src/lapi.c src/lcode.c src/ldebug.c src/ldo.c
                       src/ldump.c src/lfunc.c src/lgc.c src/llex.c
                       src/lmem.c src/lobject.c src/lopcodes.c
                       src/lparser.c src/lstate.c src/lstring.c
                       src/ltable.c src/ltm.c src/lundump.c src/lvm.c
                       src/lzio.c src/lauxlib.c src/lbaselib.c
                       src/ldblib.c src/liolib.c src/lmathlib.c
                       src/loslib.c src/ltablib.c src/lstrlib.c
                       src/loadlib.c src/linit.c src/lctype.c
                       src/lcorolib.c src/lutf8lib.c )
if ( UNIX )
    target_link_libraries( liblua-5.3.0 m dl )
endif ( UNIX )

add_executable( lua-5.3.0 src/lua.c )
target_link_libraries( lua-5.3.0 liblua-5.3.0 )

add_executable( luac-5.3.0 src/lapi.c src/lcode.c src/ldebug.c src/ldo.c
                    src/ldump.c src/lfunc.c src/lgc.c src/llex.c
                    src/lmem.c src/lobject.c src/lopcodes.c
                    src/lparser.c src/lstate.c src/lstring.c
                    src/ltable.c src/ltm.c src/lundump.c src/lvm.c
                    src/lzio.c src/lauxlib.c src/lbaselib.c
                    src/ldblib.c src/liolib.c src/lmathlib.c
                    src/loslib.c src/ltablib.c src/lstrlib.c
                    src/loadlib.c src/linit.c src/luac.c )
target_link_libraries( luac-5.3.0 liblua-5.3.0 )
