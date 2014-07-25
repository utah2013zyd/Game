/*----------------------------------------------------------------------------
    LUA SCRIPT.CPP
    Object to manage a single Lua thread/coroutine script.

    contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include    <stdio.h>
#include    <string.h>
#include    <assert.h>
#include <orz/View_Lua/LuaScript.h>
#include  <orz/View_Lua/LuaManager.h>
extern "C" {
  #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
    }


/*----------------------------------------------------------------------------
    CONSTRUCTOR
*/

	
Orz::AutoList<LuaScript>::List Orz::AutoList<LuaScript>::_list;
LuaScript::LuaScript(
    LuaManager*     mgr)
    {
    _manager             = mgr;
    _state               = LSS_NOTLOADED;
    _time             = 0;
    strcpy(lastErrorString, "No error.\n");

    // create a thread/_state for this object
    threadState = lua_newthread(_manager->_masterState);
    
    // save a pointer to the thread _manager object in the global table
    // using the new thread's vm pointer as a key
    lua_pushlightuserdata(_manager->_masterState, threadState);
    lua_pushlightuserdata(_manager->_masterState, this );
    lua_settable(_manager->_masterState, LUA_GLOBALSINDEX );
    }


/*----------------------------------------------------------------------------
    DESTRUCTOR
*/

LuaScript::~LuaScript()
{
	// Unhook from the script _manager
	//_manager->UnlinkScript(this);
	unlink();
}


/*----------------------------------------------------------------------------
    RUN FILE
*/

void LuaScript::RunFile(
    char*       fileName)
    {
    int         status;

    assert(_manager->_masterState);
    assert(threadState);

    status = luaL_loadfile(threadState, fileName);
    if (status == 0)
        {
        ResumeScript(0.0f);
        }
    else 
        {
        FormatError();
        OutputError("Syntax Error");
        }
    }


/*----------------------------------------------------------------------------
    RUN STRING
*/

int LuaScript::RunString(
    char*       commandString)
    {
    int         status;

    assert(_manager->_masterState);
    assert(threadState);

    status = luaL_loadbuffer(threadState, commandString, strlen(commandString), "Console");
    if (status == 0)
        status = lua_pcall(threadState, lua_gettop(threadState)-1, 0, 0);

    if (status)
        {
        FormatError();
        return(-1);
        }

    return(0);
    }


/*----------------------------------------------------------------------------
    UPDATE
    The purpose of update is basically to check if any suspended scripts need
    to be turned back on again!

    Returns the _next script in the list.  This assists in linked list traversal
    where this script may delete it's _next sibling.

*/

bool LuaScript::Update(
    float       elapsedSec)
    {
    _time += elapsedSec;

    switch(_state)
        {
        case LSS_WAITTIME:
            if (_time >= _waitTimestamp)
                ResumeScript(0.0f);
            break;
        case LSS_WAITFRAME:
            _waitFrame--;
            if (_waitFrame <= 0)
                ResumeScript(0.0f);
            break;
        case LSS_NOTLOADED:
            break;
//        case LSS_DONE:
        default :
            // if we aren't waiting and we get here, the script must be done
//            if (autoDelete)
//                {
//                LuaScript*  sNext;
//
//                sNext = _next;
//                delete(this);
//                return(sNext);
//                }
            break;
        }
	return true;
    //return(_next);
    }


/*----------------------------------------------------------------------------
    ABORT WAIT
    Resumes esecution of the script, pushing 1 as a return value from the
    call that suspended the script.
*/

void LuaScript::AbortWait()
    {
    ResumeScript(1.0f);
    }


/*----------------------------------------------------------------------------
    RESUME SCRIPT
    Runs or resumes a LUA script.  Executes until a yeild is encountered or
    the script terminates.

    The parameter to this function is pushed on the stack as the return value
    of the previous yield.
*/

void LuaScript::ResumeScript(
    float       param)
    {
    int         status;

    // we're about to run/resume the thread, so set the global
    _state  = LSS_RUNNING;

    // param is treated as a return value from the function that yielded
    lua_pushnumber(threadState, param);

    status = lua_resume(threadState, 1);

    if (status)
        {
        FormatError();
        OutputError("Runtime Error:");
        }
    }


/*----------------------------------------------------------------------------
    CALL FN
    Calls a Lua function.  Shows how to pass a single parameter, but not how
    to retrieve return values.
*/

void LuaScript::CallFn(
    char*       fnName,
    int         iParam)
    {
    int         status;

    // find the lua function and push it on the stack
    lua_getglobal (threadState, fnName);

    // push our single argument
    lua_pushnumber(threadState, iParam);

    // now, call into Lua
    status = lua_pcall(threadState, 1, 0, 0);

    if (status)
        {
        FormatError();
        OutputError("Runtime Error:");
        }
    }


/*----------------------------------------------------------------------------
    FORMAT ERROR
    Grabs the last error message and stuffs it in lastErrorString.
*/

void LuaScript::FormatError()
    {
    const char* msg;

    msg = lua_tostring(threadState, -1);
    if (msg == NULL)
        msg = "(error with no message)";
    lua_pop(threadState, 1);

    strcpy(lastErrorString, msg);
    }


/*----------------------------------------------------------------------------
    OUTPUT ERROR
*/

void LuaScript::OutputError(
    char*   strType)
    {
    printf("%s %s \n", strType, lastErrorString);
    }