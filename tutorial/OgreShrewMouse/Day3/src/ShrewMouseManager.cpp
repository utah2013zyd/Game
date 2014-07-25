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
            //���ǰ���һ�ε�ģ���������滻�ɵ������ͣ�ShrewMouse��
            _mice[n] = new ShrewMouse("head"+Ogre::StringConverter::toString(i+j*3), _sm, Ogre::Vector3(100 *i, 0, 100 *j));
        }
    }
}

//������������ɾ�����е�������
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



//ÿ���һ���ӣ����һ������Ӷ��������
void ShrewMouseManager::update(float interval)
{
    //�ۼ�ʱ��ֵ
    _time+=interval;
    //����ۼ�ʱ�������ж��ٸ�1��
    while(_time >= 1.f)
    {
        _time -= 1.f;
       // ÿ�����������һ����������
        _mice[rand()%9]->enable();
    }
    //��������¾Ÿ�����Ķ���
    for(int i=0; i<9; ++i)
    {
        if(_mice[i]->isEnable())
        {
            _mice[i]->update(interval);
        }
    }
}