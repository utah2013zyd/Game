#include "LockedZipArchive.h"
#include "LockedZipDataStream.h"
#include "OgreLogManager.h"
#include "OgreException.h"

namespace Ogre {


	//-----------------------------------------------------------------------
	// LockedZipArchive implementation
	//-----------------------------------------------------------------------

	//-----------------------------------------------------------------------
	LockedZipArchive::LockedZipArchive(const String& name, const String& archType )
		: Archive(name, archType)
	{
		calculatePassword();
	}

	//-----------------------------------------------------------------------
	LockedZipArchive::~LockedZipArchive()
	{
		unload();
	}

	//-----------------------------------------------------------------------
	bool LockedZipArchive::getOgreFileInfo(int idx, FileInfo& info)
	{
		CZipFileHeader infoFromZip; //file info from ZipArchive

		if(m_zipArchive->GetFileInfo(infoFromZip,idx))
		{
			info.archive = this;

			// get file name from zip archive
			CZipString s = infoFromZip.GetFileName();
			String fileName((LPCTSTR)s);

			// Get basename / path
			StringUtil::splitFilename(fileName, info.basename, info.path);

			info.filename = fileName;
			info.compressedSize = infoFromZip.m_uComprSize;
			info.uncompressedSize = infoFromZip.m_uUncomprSize;

			return true;
		}
		else
			return false;
	}

	//-----------------------------------------------------------------------
	void LockedZipArchive::load() 
	{
		struct stat tagStat;
		stat( mName.c_str(), &tagStat );

		if (( tagStat.st_mode & S_IFDIR)) 
		{
			OGRE_EXCEPT( Exception::ERR_FILE_NOT_FOUND, "LockedZipArchive archive " + mName + " not found.", "LockedZipArchive::load" );
		}
		else
		{
			m_zipArchive = new CZipArchive();
			m_zipArchive->Open(mName.c_str(),CZipArchive::zipOpenReadOnly);
			m_zipArchive->EnableFindFast();	
			m_zipArchive->SetPassword(password);
		}

		// enumerates all files in the archive and caches file infos in a list
		for(WORD i = 0;i < m_zipArchive->GetCount(true);i++)
		{
			FileInfo info;

			if (getOgreFileInfo(i, info)) 
			{
				mFileList.push_back(info);
			}
		}

	};

	//-----------------------------------------------------------------------
	void LockedZipArchive::unload() 
	{
		if (m_zipArchive)
		{
			m_zipArchive->Close();
			OGRE_DELETE(m_zipArchive);
			mFileList.clear();
			m_zipArchive = NULL;
		}

		//ekt: non sono sicuro che qua ci vada un delete. monster ce l'aveva messo per?..
		//delete this;
	};


	//-----------------------------------------------------------------------
	DataStreamPtr LockedZipArchive::open(const String& filename) const
	{
		int idx = m_zipArchive->FindFile(filename.c_str());

		if( idx != (-1) )
		{
			CZipFileHeader info;

			if(m_zipArchive->GetFileInfo(info,(WORD)idx))
			{
				CZipMemFile* pmf = new CZipMemFile;

				try
				{
					if (!m_zipArchive->ExtractFile((WORD)idx, *pmf))
						logMessage("::open ExtractFile failed");
				}
				catch (CZipException e) 
				{
					throw Ogre::Exception(e.m_iCause,"CZipException thrown, probably a bad password",(LPCTSTR)e.m_szFileName);
				}

				// Construct & return stream
				return DataStreamPtr(new LockedZipDataStream(filename, pmf, info.m_uUncomprSize));
			}
		}
		
		// in all other cases, return a null ptr
		return DataStreamPtr();
	}


	/// @copydoc Archive::list
	StringVectorPtr LockedZipArchive::list(bool recursive, bool dirs)
	{
		StringVectorPtr ret = StringVectorPtr(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);

        FileInfoList::iterator i, iend;
        iend = mFileList.end();
        for (i = mFileList.begin(); i != iend; ++i)
            if ((dirs == (i->compressedSize == size_t (-1))) &&
                (recursive || i->path.empty()))
                ret->push_back(i->filename);

        return ret;	
	}

	/// @copydoc Archive::listFileInfo
	FileInfoListPtr LockedZipArchive::listFileInfo(bool recursive, bool dirs)
	{
		FileInfoList* fil = OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)();
        FileInfoList::const_iterator i, iend;
        iend = mFileList.end();
        for (i = mFileList.begin(); i != iend; ++i)
            if ((dirs == (i->compressedSize == size_t (-1))) &&
                (recursive || i->path.empty()))
                fil->push_back(*i);

        return FileInfoListPtr(fil, SPFM_DELETE_T);
	}

	/// @copydoc Archive::find
	StringVectorPtr LockedZipArchive::find(const String& pattern, bool recursive, bool dirs)
	{
	StringVectorPtr ret = StringVectorPtr(OGRE_NEW_T(StringVector, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
        // If pattern contains a directory name, do a full match
        bool full_match = (pattern.find ('/') != String::npos) ||
                          (pattern.find ('\\') != String::npos);

        FileInfoList::iterator i, iend;
        iend = mFileList.end();
        for (i = mFileList.begin(); i != iend; ++i)
            if ((dirs == (i->compressedSize == size_t (-1))) &&
                (recursive || full_match || i->path.empty()))
                // Check basename matches pattern (zip is case insensitive)
                if (StringUtil::match(full_match ? i->filename : i->basename, pattern, false))
                    ret->push_back(i->filename);

        return ret;
	}

	/// @copydoc Archive::findFileInfo
	FileInfoListPtr LockedZipArchive::findFileInfo(const String& pattern, bool recursive, bool dirs)
	{


		FileInfoListPtr ret = FileInfoListPtr(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
        // If pattern contains a directory name, do a full match
        bool full_match = (pattern.find ('/') != String::npos) ||
                          (pattern.find ('\\') != String::npos);

        FileInfoList::iterator i, iend;
        iend = mFileList.end();
        for (i = mFileList.begin(); i != iend; ++i)
            if ((dirs == (i->compressedSize == size_t (-1))) &&
                (recursive || full_match || i->path.empty()))
                // Check name matches pattern (zip is case insensitive)
                if (StringUtil::match(full_match ? i->filename : i->basename, pattern, false))
                    ret->push_back(*i);

        return ret;


		//logMessage("::findFileInfo NOT IMPLEMENTED!");
		//return FileInfoListPtr();
	}

	/// @copydoc Archive::exists
	bool LockedZipArchive::exists(const String& filename)
	{
		return ((m_zipArchive->FindFile(filename.c_str()))!=-1);
	}


	void LockedZipArchive::calculatePassword()
	{
		strcpy(password,"babu");
	}

	void LockedZipArchive::logMessage(const String& msg) const
	{
		LogManager::getSingleton().logMessage("LockedZipArchive"+msg);
	}

}
