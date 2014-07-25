#ifndef __LockedZipArchive_H__
#define __LockedZipArchive_H__

#include "OgrePrerequisites.h"



#include "OgreArchive.h"
//#include "OgreNoMemoryMacros.h"
#include "ZipArchive.h"
//#include "OgreMemoryMacros.h"

namespace Ogre {

    class LockedZipArchive : public Archive 
	{
	protected:
		// cached file list
		FileInfoList mFileList;

    public:
		LockedZipArchive(const String& name, const String& archType );
		~LockedZipArchive();

		/// @copydoc Archive::isCaseSensitive
		bool isCaseSensitive(void) const { return false; }

		/// @copydoc Archive::load
		void load();
		/// @copydoc Archive::unload
		void unload();

		/// @copydoc Archive::open
		DataStreamPtr open(const String& filename) const;

		/// @copydoc Archive::list
		StringVectorPtr list(bool recursive = true , bool dirs = false);

		/// @copydoc Archive::listFileInfo
		FileInfoListPtr listFileInfo(bool recursive = true , bool dirs = false);

		/// @copydoc Archive::find
		StringVectorPtr find(const String& pattern, bool recursive = true, bool dirs = false);

		/// @copydoc Archive::findFileInfo
		FileInfoListPtr findFileInfo(const String& pattern, bool recursive = true, bool dirs = false);

		/// @copydoc Archive::exists
		bool exists(const String& filename);

		virtual time_t getModifiedTime(const String& filename)
		{
			return 0;
		}
	private:
		/// returns info on a specific file in ogre format
		bool getOgreFileInfo(int idx, FileInfo& info);
		/// super sofisticated algo to determine the archive password
		void calculatePassword();
		/// logs a message in ogre log file
		void logMessage(const String& msg) const;

    private:
		CZipArchive* m_zipArchive;
		char password[128];
    };

}

#endif

