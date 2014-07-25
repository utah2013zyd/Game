#include "ShrewMouseManager.h"
#include "ShrewMouse.h"

ShrewMouseManager::ShrewMouseManager(Ogre::SceneManager * sm):_sm(sm), _time(0.f)
{
    using namespace Ogre;
    for(int i=0; i<3; ++i)
    {
        for(int j =0; j<3; ++j)
        {
            int n = i+j*3;
            //我们把上一课的模型在这里替换成地鼠类型（ShrewMouse）
            _mice[n] = new ShrewMouse("head"+Ogre::StringConverter::toString(i+j*3), _sm, Ogre::Vector3(100 *i, 0, 100 *j));
        }
    }
}

//在析构函数中删除所有地鼠类型
ShrewMouseManager::~ShrewMouseManager(void)
{ 
    for(int i=0; i<9; ++i)
    {
        if(_mice[i])
        {
            delete _mice[i];
            _mice[i] = NULL;
        }
    }
}



//每间隔一秒钟，随机一个地鼠从洞里钻出来
void ShrewMouseManager::update(float interval)
{
    //累计时间值
    _time+=interval;
    //检查累计时间里面有多少个1秒
    while(_time >= 1.f)
    {
        _time -= 1.f;
       // 每秒钟随机启动一个地鼠类型
        _mice[rand()%9]->enable();
    }
    //在这里更新九个地鼠的动作
    for(int i=0; i<9; ++i)
    {
        if(_mice[i]->isEnable())
        {
            _mice[i]->update(interval);
        }
    }
}