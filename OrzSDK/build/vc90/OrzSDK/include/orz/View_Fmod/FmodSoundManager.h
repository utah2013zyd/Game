#ifndef __Orz_ViewFmod_FmodSoundManager_h__
#define __Orz_ViewFmod_FmodSoundManager_h__


#include <orz/View_Fmod/ViewFmodConfig.h>
#include <orz/Framework_Base/System.h>
namespace Orz
{
class FmodSoundManagerImpl;
template class _OrzViewFmodExport boost::scoped_ptr<FmodSoundManagerImpl>;
class _OrzViewFmodExport FmodSoundManager : public ISoundManager
{

public:
	FmodSoundManager(void);
	virtual ~FmodSoundManager(void);
	
	bool init(void);
	void shutdown(void);
	bool update(TimeType interval);
	SoundPlayerPtr createPlayer(const std::string & fileName, bool stream);
private:
	boost::scoped_ptr<FmodSoundManagerImpl> _impl;
};


SYSTEMUNIT_BEGIN(FmodSoundManager) 
SYSTEMUNIT_INIT 
SYSTEMUNIT_UPDATE 
SYSTEMUNIT_SHUTDOWN 
SYSTEMUNIT_END(FmodSoundManager)

}



//\
//	 \
//SYSTEMUNIT_FACTORY(Interface, FmodSoundManager)




#endif 
