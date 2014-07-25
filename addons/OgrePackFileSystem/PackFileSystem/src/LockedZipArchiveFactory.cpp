#include "LockedZipArchiveFactory.h"

namespace Ogre {

    LockedZipArchiveFactory::~LockedZipArchiveFactory()
    {
    }

	//-----------------------------------------------------------------------
	const String& LockedZipArchiveFactory::getType(void) const
	{
		static String name = "LockedZipArchive";
		return name;
	}

}

