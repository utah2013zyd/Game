/*----------------------------------------------------------------------------
    LUA SCRIPT
    Class to manage a single Lua thread/_state/script.

    contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#ifndef _LUASCRIPT_H
#define _LUASCRIPT_H

#include <orz/View_Lua/ViewLuaConfig.h>
#include <orz/Toolkit_Base/AutoList.h>
/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                            Defined Constants                             */
/*--------------------------------------------------------------------------*/

typedef enum luaScriptStateTag
    {
    LSS_WAITFRAME,
    LSS_WAITTIME,
    LSS_RUNNING,
    LSS_NOTLOADED,
    LSS_DONE
    } LUASCRIPTSTATE;


/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

class LuaManager;
typedef struct lua_State lua_State;
class LuaScript;
typedef boost::shared_ptr<LuaScript> LuaScriptPtr;
class _OrzViewLuaExport LuaScript:public Orz::AutoList<LuaScript>
    {
    public:                         // ---------------------------------------

    LuaManager*     _manager;            // LuaManager object that created this script
  //  LuaScript *     _next;
    LUASCRIPTSTATE  _state;              // are we waiting, not loaded yet, etc.
    float           _waitTimestamp;      // _time to wake up
    int             _waitFrame;          // number of frames to wait
    float           _time;               // current _time

                    LuaScript          (LuaManager* mgr);
                    ~LuaScript         (void);
    void            CreateThread       (void);
    bool    Update             (float       elapsedSec);
    void            RunFile            (char*       fileName);
    int             RunString          (char*       commandString);
    void            CallFn             (char*       fnName,
                                        int         iParam);
    void            AbortWait          (void);

    private:                        // ---------------------------------------

    lua_State*      threadState;
    char            lastErrorString[256];
    void            ResumeScript       (float   param);
    void            FormatError        (void);
    void            OutputError        (char*   strType);

    };


/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#endif // _LUASCRIPT_H

