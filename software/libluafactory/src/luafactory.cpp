
#include "luafactory.h"
#include "luadigitizer.h"
#include "factory_client.h"

static const char lmMeta[]    = "LUAFACTORY_META";
static const char lmLibName[] = "luafactory";

static int version( lua_State * L )
{
    lua_pushstring( L, FactoryClient::version().data() );
    return 1;
}

static int create( lua_State * L )
{
    FactoryClient * f;
    if ( lua_gettop( L ) > 1 )
    {
        std::string host = lua_tostring( L, 1 );
        int         port = static_cast<int>( lua_tonumber( L, 2 ) );
        f = FactoryClient::factory( host, port );
    }
    else
        f = FactoryClient::factory();
    if ( !f )
    {
        lua_pushnil( L );
        return 1;
    }
    
    FactoryClient * * p = reinterpret_cast< FactoryClient * * >( lua_newuserdata( L, sizeof( FactoryClient * ) ) );
    *p = f;
    luaL_getmetatable( L, lmMeta ); // Она сама выталкивает из стека lmMeta
    lua_setmetatable( L, -2 );

    return 1;
}

static int gc( lua_State * L )
{
    FactoryClient * f = *reinterpret_cast<FactoryClient * *>( lua_touserdata( L, 1 ) );
    if ( f )
        delete f;
    return 0;
}

static int status( lua_State * L )
{
    FactoryClient * f = *reinterpret_cast<FactoryClient * *>( lua_touserdata( L, 1 ) );
    if ( f )
    {
        std::string res = f->status();
        lua_pushstring( L, res.data() );
    }
    else
        lua_pushnil( L );
    return 1;
}

static int digitizer( lua_State * L )
{
    FactoryClient * f = *reinterpret_cast<FactoryClient * *>( lua_touserdata( L, 1 ) );
    if ( f )
    {
        DigitizerPrx * d = f->digitizer();
        DigiEnvelope * de = new DigiEnvelope();
        de->d = d;
        DigiEnvelope * * ppde = reinterpret_cast< DigiEnvelope * * >( lua_newuserdata( L, sizeof( DigiEnvelope * ) ) );
        *ppde = de;
        applyDigitizerMeta( L );
        return 1;
    }
    else
        lua_pushnil( L );
    return 1;
}

static const struct luaL_reg lmMetaFunctions[] = {
    { "status",    status }, 
    { "digitizer", digitizer }, 
    { 0,           0 }, 
};

static void createMeta( lua_State * L )
{
    luaL_newmetatable( L, lmMeta );  // create new metatable for file handles
    // file methods
    lua_pushliteral( L, "__index" );
    lua_pushvalue( L, -2 );  // push metatable
    lua_rawset( L, -3 );     // metatable.__index = metatable

    lua_pushliteral( L, "__gc" );
    lua_pushcfunction( L, gc );
    lua_rawset( L, -3 );

    luaL_register( L, NULL, lmMetaFunctions );
    // Очищаем стек.
    lua_pop( L, lua_gettop( L ) );
}

static const struct luaL_reg lmFunctions[] = {
    { "version",  version }, 
    { "create",   create }, 
    { 0,          0 },
};

static void registerFunctions( lua_State * L )
{
    luaL_register( L, lmLibName, lmFunctions );
}




extern "C" int EXT_FACTORY luaopen_luafactory( lua_State * L )
{
    createDigitizerMeta( L );

    createMeta( L );
    registerFunctions( L );
    lua_pop( L, lua_gettop( L ) );
    return 0;
}

