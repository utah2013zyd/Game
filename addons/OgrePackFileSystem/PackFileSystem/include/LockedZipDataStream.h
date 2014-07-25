#ifndef _LockedZipDataStream_H__
#define _LockedZipDataStream_H__

#include "OgreDataStream.h"
//#include "OgreNoMemoryMacros.h"
#include "ZipArchive.h"
//#include "OgreMemoryMacros.h"

// i have to do this otherwise i get conflicts with std::min
// probabily due to redefinition in ziparchive or something
#undef min
#undef max
#include <algorithm>
using std::min;
using std::max;


namespace Ogre {
	/** Specialisation of DataStream to handle streaming data from locked zip archives. */
	class LockedZipDataStream : public DataStream
	{
	protected:
		/// ZipArchive memory file
		CZipMemFile* m_zipFile;
		/// Temporary zip copy area
		char mZipTmpArea[OGRE_STREAM_TEMP_SIZE];

	public:
		///// Unnamed constructor
		//LockedZipDataStream(FILE* zzipFile, size_t uncompressedSize);
		
		/// Constructor for creating named streams
		LockedZipDataStream::LockedZipDataStream(const String& name, CZipMemFile* zipMemoryFile, size_t uncompressedSize);
		~LockedZipDataStream();

		/// @copydoc DataStream::read
		size_t read(void* buf, size_t count);
		/// @copydoc DataStream::read
		size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
		/// @copydoc DataStream::skipLine
		size_t skipLine(const String& delim = "\n");
		/// @copydoc DataStream::skip
		void skip(long count);
		/// @copydoc DataStream::seek
		void seek( size_t pos );
		/// @copydoc DataStream::tell
		size_t tell(void) const;
		/// @copydoc DataStream::eof
		bool eof(void) const;
		/// @copydoc DataStream::close
		void close(void);

	};

}

#endif