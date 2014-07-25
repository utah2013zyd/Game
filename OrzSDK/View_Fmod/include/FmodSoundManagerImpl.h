#ifndef __Orz_ViewFmod_FmodSoundManagerImpl_h__
#define __Orz_ViewFmod_FmodSoundManagerImpl_h__


#include <orz/View_Fmod/ViewFmodConfig.h>
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

namespace Orz
{
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}


class _OrzViewFmodPrivate FmodSoundPlayer: public SoundPlayer
{

public:
	FmodSoundPlayer(FMOD::System * system,const std::string & name, bool stream = false):SoundPlayer(name, stream),
		_sound(NULL),
		_channel(NULL),
		_system(system)
	{

	}
	virtual ~FmodSoundPlayer(void){}
protected:


	
	virtual bool load()
	{
		if(_stream)
			_system->createSound(_name.c_str(),  FMOD_SOFTWARE|FMOD_CREATESTREAM|FMOD_2D, 0, &_sound);
		else
			_system->createSound(_name.c_str(),  FMOD_SOFTWARE|FMOD_2D, 0, &_sound);
		//_sound->setMode(FMOD_LOOP_OFF);
		return true;
	}
	virtual bool play(uint32 times)
	{
		_system->playSound(FMOD_CHANNEL_FREE, _sound, false, &_channel);

		if(times <= 0)
			_channel->setMode(FMOD_LOOP_NORMAL);
		else
			_channel->setLoopCount(times);//setLoopCount

		//if(this->_useStream)
		//	setVolume(0.5f);
		//else
		//	setVolume(1.0f);

		return true;
	}


	virtual bool setVolume(float volume)
	{
		_channel->setVolume(volume);
		return true;
	}


	virtual bool setPosition(float posi)
	{
		_channel->setPosition(static_cast<int>(posi * 1000.0f), FMOD_TIMEUNIT_MS);
		return true;

	}
	virtual float getPosition(void)
	{
		unsigned int posi;
		_channel->getPosition(&posi, FMOD_TIMEUNIT_MS);
		return static_cast<float>(posi)/1000.0f;
	}
	virtual float getLength(void)
	{
		unsigned int length;
		_sound->getLength(&length,FMOD_TIMEUNIT_MS );
		return static_cast<float>(length)/1000.0f;
	}


	virtual bool stop(void)
	{
		_channel->stop();
		return true;
	}
	virtual bool setPaused(bool paused)
	{
		_channel->setPaused(paused);
		return true;
	}

	virtual bool playing(void)
	{
		bool playing;
		_channel->isPlaying(&playing);
		return playing;
	}
	virtual bool unload(void)
	{
		_sound->release();
		return true;
	}

private:
	FMOD::Sound      * _sound;
    FMOD::Channel    * _channel;
    FMOD::System    * _system;
};




class _OrzViewFmodPrivate FmodSoundManagerImpl
{

public:
	FmodSoundManagerImpl(void):_system(NULL)
	{
	
	}
	~FmodSoundManagerImpl(void)
	{
	
	}
	bool init(void)
	{
	
		
		FMOD_RESULT   result;
		result = FMOD::System_Create(&_system);
		ERRCHECK(result);
#if ORZ_PLATFORM == ORZ_PLATFORM_LINUX
		result = _system->setOutput(FMOD_OUTPUTTYPE_ALSA);
#endif
		result = _system->init(32, FMOD_INIT_NORMAL, 0);
		ERRCHECK(result);

		return true;
	}
	void shutdown(void)
	{
		_system->close();
		_system->release();    
	}
	bool update(TimeType)
	{
		_system->update();
		return true;
	}
	SoundPlayerPtr createPlayer(const std::string & fileName, bool stream)
	{
	
		SoundPlayerPtr player(new FmodSoundPlayer(_system, fileName, stream));

	
		return player;
	}


private:
	FMOD::System * _system;

};



}

#endif 
