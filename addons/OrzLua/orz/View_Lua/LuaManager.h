/*----------------------------------------------------------------------------
    LUA MANAGER.H
    Manages the single, master Lua _state.

    contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

// signal header has been included
#ifndef _LUAMANAGER_H
#define _LUAMANAGER_H


#include <orz/View_Lua/ViewLuaConfig.h>

#include <orz/Framework_Base/System/SystemInterface.h>
/*--------------------------------------------------------------------------*/
/*                              Data Structures                             */
/*--------------------------------------------------------------------------*/

class LuaScript;
typedef struct lua_State lua_State;
typedef boost::shared_ptr<LuaScript> LuaScriptPtr;
class _OrzViewLuaExport LuaManager
{

public: //orz interface
	bool init(void);
	void shutdown(void);
	bool update(Orz::TimeType i);
public:                         // ---------------------------------------
    lua_State*  _masterState;

                LuaManager         (void);
                ~LuaManager        (void);

    LuaScriptPtr  CreateScript       (void);
    //void        UnlinkScript       (LuaScript*  s);
   
    int         NumScripts         (void);

private:                        // ---------------------------------------
	void        Update             (float       elapsedSec);
   // LuaScript *  _head;
    

};
namespace Orz
{

SYSTEMUNIT_BEGIN(LuaManager)
SYSTEMUNIT_INIT
SYSTEMUNIT_UPDATE
SYSTEMUNIT_SHUTDOWN
SYSTEMUNIT_END(LuaManager)
}
/*--------------------------------------------------------------------------*/
/*                           Compilation Control                            */
/*--------------------------------------------------------------------------*/

#endif // _LUAMANAGER_H
