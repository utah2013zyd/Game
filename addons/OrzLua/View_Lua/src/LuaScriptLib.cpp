/*----------------------------------------------------------------------------
    LUA SCRIPT LIB.C
    Small library of Lua glue routines to support script management.

    contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include    <windows.h>     // just for Beep()
#include    <stdio.h>
#include    <string.h>
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
    }
#include    "LuaScriptLib.h"
#include <orz/View_Lua/LuaScript.h>
#include  <orz/View_Lua/LuaManager.h>


/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                            Public Prototypes                             */    
/*--------------------------------------------------------------------------*/

static LuaScript*   GetScriptObject(lua_State* l);
static int          LuaBeep        (lua_State* l);
static int          LuaWaitFrame   (lua_State* l);
static int          LuaWaitTime    (lua_State* l);
static int          LuaWaitSec     (lua_State* l);


/*--------------------------------------------------------------------------*/
/*                             Global Variables                             */
/*--------------------------------------------------------------------------*/

// table of function names and entry points that will be registered with Lua
static const luaL_reg scriptLib[] =
    {
    {"Beep",         LuaBeep},
    {"WaitFrame",    LuaWaitFrame},
    {"WaitTime",     LuaWaitTime},
    {"WaitSec",      LuaWaitSec},
    {NULL, NULL}
    };


/*----------------------------------------------------------------------------
    LUA OPEN SCRIPT LIB
    Registers all the basic script support functions with Lua.
*/

void LuaOpenScriptLib(
    lua_State*  l)
    {
    // register our library of routines under the table name Script
    luaL_openlib(l, "Script", scriptLib, 0);
    }


/*----------------------------------------------------------------------------
    LUA BEEP
    Expose the windows Beep() function.  Grab frequency and duration from
    the stack.
*/

static int LuaBeep(lua_State* l)
    {
    Beep((int)luaL_checknumber(l, 1), (int)luaL_checknumber(l, 2));
    return(0);
    }


/*----------------------------------------------------------------------------
    LUA WAIT FRAME
    Causes a script to wait for the specified number of frames.
*/

static int LuaWaitFrame(lua_State* l)
    {
    LuaScript*  s;

    s = GetScriptObject(l);

    s->_waitFrame = (int)luaL_checknumber(l, 1);
    s->_state     = LSS_WAITFRAME;

    return(lua_yield(l, 1));
    }


/*----------------------------------------------------------------------------
    LUA WAIT TIME
    Causes a script to wait until the given timestamp.
*/

static int LuaWaitTime(lua_State* l)
    {
    LuaScript*  s;

    s = GetScriptObject(l);

    s->_waitTimestamp = (float)luaL_checknumber(l, 1);
    s->_state         = LSS_WAITTIME;

    return(lua_yield(l, 1));
    }


/*----------------------------------------------------------------------------
    LUA WAIT SEC
    Causes a script to wait for the given number of seconds.
*/

static int LuaWaitSec(lua_State* l)
    {
    LuaScript*  s;

    s = GetScriptObject(l);

    s->_waitTimestamp = s->_time + (float)luaL_checknumber(l, 1);
    s->_state         = LSS_WAITTIME;

    return(lua_yield(l, 1));
    }


/*----------------------------------------------------------------------------
    GET SCRIPT OBJECT
    Retrieves the address of the C++ object managing the given lua_State.
*/

static LuaScript* GetScriptObject(lua_State* l)
    {
    lua_pushlightuserdata(l, l);
    lua_gettable(l, LUA_GLOBALSINDEX );
    return((LuaScript*)lua_touserdata(l, -1));
    }
