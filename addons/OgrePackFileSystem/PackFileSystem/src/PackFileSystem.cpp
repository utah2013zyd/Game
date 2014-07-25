#include "LockedZipArchiveFactory.h"
#include "OgreArchiveManager.h"

namespace Ogre {

	LockedZipArchiveFactory* pLZFactory = NULL;

	//-----------------------------------------------------------------------
	// entry points for ogre plugin
	//-----------------------------------------------------------------------

	//-----------------------------------------------------------------------
	extern "C" void dllStartPlugin(void)
	{
		pLZFactory = new LockedZipArchiveFactory();
		ArchiveManager::getSingleton().addArchiveFactory(pLZFactory);
	}

	//-----------------------------------------------------------------------
	extern "C" void dllStopPlugin(void)
	{
		delete pLZFactory;
	}

}