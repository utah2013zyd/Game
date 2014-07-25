#include "LockedZipDataStream.h"
#include "OgreLogManager.h"

namespace Ogre {

	LockedZipDataStream::LockedZipDataStream(const String& name, CZipMemFile* zipMemoryFile, size_t uncompressedSize)
		:DataStream(name)
	{
		m_zipFile=zipMemoryFile;
		mSize=uncompressedSize;
	}
	

	LockedZipDataStream::~LockedZipDataStream()
	{
		close();
	}

	size_t LockedZipDataStream::read(void* buf, size_t count)
	{
		return m_zipFile->Read(buf, (UINT)count);
	}
	
	size_t LockedZipDataStream::readLine(char* buf, size_t maxCount, const String& delim)
	{
		// read in chunks
		size_t chunkSize = std::min(maxCount, (size_t)OGRE_STREAM_TEMP_SIZE-1);
		size_t totalCount = 0;
		size_t readCount; 
		// Deal with both Unix & Windows LFs
		bool trimCR = false;
		if (delim.find_first_of('\n') != String::npos)
		{
			trimCR = true;
		}
		while (chunkSize && (readCount = m_zipFile->Read(mZipTmpArea, (UINT)chunkSize)))
		{
			// Terminate
			mZipTmpArea[readCount] = '\0';
			// Find first delimiter
			size_t pos = strcspn(mZipTmpArea, delim.c_str());

			if (pos < readCount)
			{
				// found terminator
				// reposition backwards
				LONGLONG offset = static_cast<LONGLONG>(pos) - static_cast<LONGLONG>(readCount) + 1;
				m_zipFile->Seek(offset, CZipMemFile::current);
			}

			if (pos > 0)
			{
				if (trimCR && mZipTmpArea[pos-1] == '\r')
				{
					// strip off CR
					--pos;
				}
				// Are we genuinely copying?
				if (buf)
				{
					memcpy(buf, (const void*)mZipTmpArea, pos);
					buf[pos] = '\0';
				}
				totalCount += pos;
			}

			if (pos < readCount)
			{
				// Found delimiter, break out
				break;
			}
			// Adjust chunkSize for next time
			chunkSize = std::min((maxCount-totalCount), (size_t)OGRE_STREAM_TEMP_SIZE-1);

		}

		return totalCount;		
	}
	
	size_t LockedZipDataStream::skipLine(const String& delim)
	{
		// Re-use readLine, but don't copy data
		char* nullBuf = 0;
		return readLine(nullBuf, 1024, delim);
	}

	void LockedZipDataStream::skip(long count)
	{
		m_zipFile->Seek(static_cast<LONGLONG>(count), CZipMemFile::current);
	}

	void LockedZipDataStream::seek( size_t pos )
	{
		m_zipFile->Seek(static_cast<LONGLONG>(pos), CZipMemFile::begin);
	}

	size_t LockedZipDataStream::tell(void) const
	{
		return m_zipFile->GetPosition();
	}

	bool LockedZipDataStream::eof(void) const
	{
		return (m_zipFile->GetPosition() >= m_zipFile->GetLength());
	}

	void LockedZipDataStream::close(void)
	{
		if (m_zipFile) {
			m_zipFile->Close();
			OGRE_DELETE(m_zipFile);
		}
	}


}