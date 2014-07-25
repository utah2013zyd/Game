#ifndef _LockedZipArchiveFactory_H__
#define _LockedZipArchiveFactory_H__

#include "OgreArchiveFactory.h"
#include "LockedZipArchive.h"

namespace Ogre {
    /** Specialisation of ArchiveFactory for password protected Zip files. */

    class LockedZipArchiveFactory : public ArchiveFactory
    {
    public:
        virtual ~LockedZipArchiveFactory();

		/// @copydoc FactoryObj::getType
		const String& getType(void) const;

		/// @copydoc FactoryObj::createInstance
		Archive *createInstance( const String& name ) 
		{
			return new LockedZipArchive(name, "LockedZipArchive");
		}

		/// @copydoc FactoryObj::destroyInstance
		void destroyInstance( Archive* arch) { delete arch; }

    };

}

#endif

