
#ifndef __LUAFACTORY_H_
#define __LUAFACTORY_H_

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

#define EXT_FACTORY __declspec(dllexport)

extern "C" int EXT_FACTORY luaopen_luafactory( lua_State * L );



#endif



