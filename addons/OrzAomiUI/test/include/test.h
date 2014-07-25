#ifndef test12
#define test12


#	if defined( TEST_EXPORTS )
#		define _OrzTestUIExport __declspec( dllexport )
#	else
#		define _OrzTestUIExport __declspec( dllimport )
#	endif



class _OrzTestUIExport test
{
public:
	void shutdown();
};

#endif