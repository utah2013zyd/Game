
#ifndef __Orz_FrameworkBase_ISoundManager_h__
#define __Orz_FrameworkBase_ISoundManager_h__

#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include <orz/Toolkit_Base/Singleton.h>

namespace Orz{

class _OrzFrameworkBaseExport SoundPlayer
{

public:
	SoundPlayer(const std::string & name, bool stream = false);

	/**播放音频对象
	@param times 播放次数，默认值为一次，如果设置为0或者负数，将会循环播放。
	@returns 是否播放成功
	*/
	virtual bool play(uint32 times = 1) = 0;

	///检查是否正在播放中
	virtual bool playing(void) = 0;
	///载入音频数据到内存中
	virtual bool load(void) = 0;
	///从内存中卸载音频数据
	virtual bool unload(void) = 0;
	///停止播放
	virtual bool stop(void) = 0;
	///设置音频是否暂停
	virtual bool setPaused(bool pause) = 0;
	///设置音频音量大小
	virtual bool setVolume(float volume) = 0;
	///设置音频播放位置
	virtual bool setPosition(float position) = 0;
	///得到当前音频位置
	virtual float getPosition(void) = 0;
	///得到音频总长度
	virtual float getLength(void) = 0;
	virtual ~SoundPlayer(void);
	
protected:
	std::string _name;
	bool _stream;
};

//class ISoundManagerImpl;

//template class _OrzFrameworkBaseExport boost::scoped_ptr<ISoundManagerImpl>;


class _OrzFrameworkBaseExport ISoundManager : public Singleton<ISoundManager>
{
private:
	typedef std::vector<SoundPlayerPtr> SoundPlayerList;
public:
	
	ISoundManager(void);
	virtual ~ISoundManager(void);
	///设置资源所在目录
	void setResourcesPath(const std::string & path);
	///创建一个音频实力
	virtual SoundPlayerPtr createPlayer(const std::string & fileName, bool stream = false) = 0;

	///简单播放接口，只播放一遍。
	void quickPlay(const std::string & fileName, bool stream = false);



protected:
	
	const std::string & getResourcesPath(void) const;
	void updateQuickPlay();
private:
	std::string _path;
	SoundPlayerList _players;

};


}

#endif
