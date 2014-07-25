/*----------------------------------------------------------------------------
    LuaManager.H
    Lua Script Manager.
    contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include    <stdio.h>
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
    }

#include <orz/View_Lua/LuaScript.h>
#include     <orz/View_Lua/LuaManager.h>
#include     "LuaScriptLib.h"


bool LuaManager::init(void)
{

	_masterState = lua_open();

	if (_masterState)
	{
		
	// open any auxiliary libaraies we may need
	//   lua_baselibopen (_masterState); 
	//lua_iolibopen   (_masterState); 
	//lua_strlibopen  (_masterState); 
	//     lua_mathlibopen (_masterState); 
	// including our own!
		LuaOpenScriptLib(_masterState);
	}


	return true;
}
void LuaManager::shutdown(void)
{

	
	//LuaScript*  s;
	//LuaScript*  next;

	//// destroy all children scripts
	//s = _head;
	//while (s)
	//{
	//	// save the _next pointer in case the script is deleted
	//	next = s->_next;
	//	delete s;
	//	s = next;
	//}
	LuaScript::unlinkAll();
	lua_close(_masterState);
}
bool LuaManager::update(Orz::TimeType i)
{
	Update(i/1000.f);
	return true;
}
/*----------------------------------------------------------------------------
    CONSTRUCTOR
*/

LuaManager::LuaManager()
{
	_masterState = NULL;
	//_head = NULL;
}


/*----------------------------------------------------------------------------
    DESTRUCTOR
    Destroy all children scripts we are managing, and then close the Lua _state.
*/

LuaManager::~LuaManager()
{

}


/*----------------------------------------------------------------------------
    CREATE SCRIPT
    Creates a script object.
*/

LuaScriptPtr LuaManager::CreateScript()  
{
    LuaScriptPtr s(new LuaScript(this));
    // create a script object
    s->link();
    //// tell the script object who its _manager is
    //s->_manager = this;

    //// link the new script into the list
    //s->_next = _head;
    //_head = s.get();

    return(s);
}


/*----------------------------------------------------------------------------
    UNLINK SCRIPT
    Unhooks a script from the linked list of scripts.
*/

//void LuaManager::UnlinkScript(
//    LuaScript *  s)
//    {
//	LuaScript *  prev;
//
//    // if s is at the _head, simply unlink it
//    if (_head == s)
//        {
//        _head = s->_next;
//        return;
//        }
//
//    // find previous link
//    prev = _head;
//    while (prev)
//        {
//        if (prev->_next == s)
//            {
//            prev->_next = s->_next;
//            return;
//            }
//        prev = prev->_next;
//        }
//    }


/*----------------------------------------------------------------------------
    UPDATE
    Call every script's update routine.
    For convenience, scripts return their child when Update is called.  This
    helps when a script destroys itself during Update.
*/

void LuaManager::Update(
    float       elapsedSec)
{
	for(LuaScript::Iterator it = LuaScript::begin();
		it != LuaScript::end();
		++it)
	{
		(*it)->Update(elapsedSec);
	}

  /*  s = _head;
    while (s)
    {
		s = s->Update(elapsedSec);
    }*/
}