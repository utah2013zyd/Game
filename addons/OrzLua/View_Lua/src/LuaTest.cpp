/*----------------------------------------------------------------------------
    LUA TEST.CPP
    Quick and dirty console application to test the Lua management system.

    contact: Matthew Harmon matt@matthewharmon.com
*/

/*--------------------------------------------------------------------------*/
/*                              Include Files                               */
/*--------------------------------------------------------------------------*/

#include    <windows.h>             // just for timeGetTime()
#include    <stdio.h>
#include    <conio.h>
#include    <assert.h>
#include <orz/View_Lua/LuaScript.h>
#include  <orz/View_Lua/LuaManager.h>



/*--------------------------------------------------------------------------*/
/*                            Public Prototypes                             */    
/*--------------------------------------------------------------------------*/

static void Initialize  (void);
static void Frame       (void);
static void Shutdown    (void);
static void ProcessInput(void);


/*--------------------------------------------------------------------------*/
/*                             Global Variables                             */
/*--------------------------------------------------------------------------*/

static LuaManager*  luaManager;      // object to manage all scripts
static LuaScriptPtr   script1;         // an individual script
static LuaScriptPtr   script2;
static LuaScriptPtr   script3;
static DWORD        oldTime;         // timekeepers
static DWORD        newTime;
static float        elapsedSec;
static int          done = FALSE;
static char*        strTestScript = 
    "print(\"Script from text string.\");"
    "Script.Beep(880,100);"
    "print(\"  Hello world!\");"
    "print(\"  Press [Esc] to exit the demo.\");"
    "Script.Beep(844,100);";


/*----------------------------------------------------------------------------
    MAIN
*/

int main()
    {
    Initialize();

    while(!done)
        Frame();

    Shutdown();

    return(0);
    }


/*----------------------------------------------------------------------------
    INITIALIZE
    Create the Lua _manager and 3 script objects.  Then, load/run the test scripts.
*/

static void Initialize()
    {
    LuaScriptPtr  scriptTemp;

    luaManager = new LuaManager;

    // create a thread and run a script held in a local string
    scriptTemp = luaManager->CreateScript();
    scriptTemp->RunString(strTestScript);
   // delete scriptTemp;

    // create three threads for use in our game loop
    script1 = luaManager->CreateScript();
    script2 = luaManager->CreateScript();
    script3 = luaManager->CreateScript();

    // now load and run 3 files
    script1->RunFile("Script1.lua");
    script2->RunFile("Script2.lua");
    script3->RunFile("Script3.lua");

    oldTime =  timeGetTime();      
    }


/*----------------------------------------------------------------------------
    SHUTDOWN
    Delete everything we created.
*/

void Shutdown()
    {
    script3.reset();
    script2.reset();
    script1.reset();
    delete luaManager;
    }


/*----------------------------------------------------------------------------
    FRAME
    Once through the game loop!
*/

void Frame()
    {
    ProcessInput();

    // get elapsed seconds since last frame
    newTime    = timeGetTime();
    elapsedSec = (float)(newTime-oldTime) / 1000.0f;
    oldTime    = newTime;

    // update the scripts
    luaManager->update(elapsedSec);
    }


/*----------------------------------------------------------------------------
    PROCESS INPUT
*/

static void ProcessInput()
    {
    int     ch;

    if (kbhit())
        {
        ch = getch();
        switch(ch)
            {
            case 27 : done=TRUE;                   break;
            case '2': script2->AbortWait();        break;
            case '3': script3->CallFn("Beeps", 4); break;
            }
        }
    }