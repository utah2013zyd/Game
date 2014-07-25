#include <orz/Toolkit_Plus/Iconv/Iconv.h>
#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
#	include <iconv/iconv.h>
#else
#	include <iconv.h>
#endif
using namespace Orz;


Iconv::Iconv()
{
}

Iconv::~Iconv()
{
}

Iconv& Iconv::instance()
{
	static Iconv singleton;
	return singleton;
}

#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
int32 Iconv::code_convert(char* from_charset, char* to_charset, const char* inbuf, int inlen, char* outbuf, int outlen)
{
	iconv_t	cd;   
	const char	**pin = &inbuf;   
	char	**pout = &outbuf;   

	cd = iconv_open(to_charset, from_charset);   
	if (cd == 0)
		return 1;

	memset(outbuf, 0, outlen);   

	if (iconv(cd, pin, (size_t*)&inlen, pout, (size_t*)&outlen) == -1)
		return 1;

	iconv_close(cd);
	return 0;
}
#else
int32 Iconv::code_convert(char* from_charset, char* to_charset, char* inbuf, int inlen, char* outbuf, int outlen)
{
	iconv_t	cd;   
	int		rc;   
	char	**pin = &inbuf;   
	char	**pout = &outbuf;   

	cd = iconv_open(to_charset, from_charset);   
	if (cd == 0)
		return 1;

	memset(outbuf, 0, outlen);   

	if (iconv(cd, pin, (size_t*)&inlen, pout, (size_t*)&outlen) == -1)
		return 1;

	iconv_close(cd);
	return 0;
}
#endif
