
#include "luadigitizer.h"
#include "digitizer_prx.h"

static const char lmMeta[]    = "LUADIGITIZERPRX_META";


static int gc( lua_State * L )
{
    DigiEnvelope * de = *reinterpret_cast<DigiEnvelope * *>( lua_touserdata( L, 1 ) );
    if ( de )
        delete de;
    return 0;
}

static int pointer( lua_State * L )
{
    DigiEnvelope * h = *reinterpret_cast<DigiEnvelope * *>( lua_touserdata( L, 1 ) );
    void * res = reinterpret_cast<void *>( h );
    lua_pushlightuserdata( L, res );
    return 1;
}

static int instantValues( lua_State * L )
{
    DigiEnvelope * de = *reinterpret_cast<DigiEnvelope * *>( lua_touserdata( L, 1 ) );
    qreal workV, workI, probeV, probeI;
    bool res = de->d->instantValues( workV, workI, probeV, probeI );
    if ( res )
    {
        lua_pushnumber( L, static_cast<lua_Number>( workV ) );
        lua_pushnumber( L, static_cast<lua_Number>( workI ) );
        lua_pushnumber( L, static_cast<lua_Number>( probeV ) );
        lua_pushnumber( L, static_cast<lua_Number>( probeI ) );
        return 4;
    }
    else
        lua_pushnil( L );
    return 1;
}

static int setTrigEn( lua_State * L )
{
    DigiEnvelope * de = *reinterpret_cast<DigiEnvelope * *>( lua_touserdata( L, 1 ) );
    bool en = (lua_toboolean( L, 1 ) > 0);
    bool res = de->d->setTrigEn( en );
    lua_pushboolean( L, res ? 1 : 0 );
    return 1;
}

static int values( lua_State * L )
{
    DigiEnvelope * de = *reinterpret_cast<DigiEnvelope * *>( lua_touserdata( L, 1 ) );
    bool res = de->d->values( de->workV, de->workI, de->probeV, de->probeI );
    if ( res )
    {
        int sz = static_cast<int>( de->workV.size() );
        lua_createtable( L, sz, 0 );
        for ( int i=0; i<sz; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, de->workV[i] );
            lua_settable( L, -3 );
        }

        sz = static_cast<int>( de->workI.size() );
        lua_createtable( L, sz, 0 );
        for ( int i=0; i<sz; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, de->workI[i] );
            lua_settable( L, -3 );
        }

        sz = static_cast<int>( de->probeV.size() );
        lua_createtable( L, sz, 0 );
        for ( int i=0; i<sz; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, de->probeV[i] );
            lua_settable( L, -3 );
        }

        sz = static_cast<int>( de->probeI.size() );
        lua_createtable( L, sz, 0 );
        for ( int i=0; i<sz; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, de->probeI[i] );
            lua_settable( L, -3 );
        }
        return 4;
    }
    else
        lua_pushnil( L );
    return 1;
}



static const struct luaL_reg lmMetaFunctions[] = {
    { "pointer",            pointer }, 
    { "instantValues",      instantValues }, 
    { "setTrigEn",          setTrigEn }, 
    { "values",               values }, 
    { NULL,                 NULL }, 
};

void createDigitizerMeta( lua_State * L )
{
    int n = lua_gettop( L );
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
    lua_settop( L, n );
}

void applyDigitizerMeta( lua_State * L )
{
    luaL_getmetatable( L, lmMeta );
    lua_setmetatable( L, -2 );
}

