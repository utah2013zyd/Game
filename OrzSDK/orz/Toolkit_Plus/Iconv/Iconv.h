#ifndef __Orz_ToolkitPlus_Iconv_h__
#define __Orz_ToolkitPlus_Iconv_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
namespace Orz
{
#define CONV_STATIC_MAX_SIZE 128
#define UTF_STATIC_MAX_SIZE 256

class _OrzToolkitPlusExport Iconv
{
	Iconv();
	Iconv(const Iconv& _other);
	Iconv& operator=(const Iconv& _other);
public:
	~Iconv();

public:
	static Iconv& instance();

public:
#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
	int32			code_convert		(char* from_charset, char* to_charset, const char* inbuf, int inlen, char* outbuf, int outlen);
#else
	int32			code_convert		(char* from_charset, char* to_charset, char* inbuf, int inlen, char* outbuf, int outlen);
#endif
};
}
#define sIconv	Orz::Iconv::instance()
#endif
