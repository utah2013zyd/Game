#ifndef __ShrewMouse_H__
#define __ShrewMouse_H__
#include <Ogre.h>

class ShrewMouse
{
public:
	//���캯��������Ϊ ����ʵ�������ƣ���������������������ά�ռ��λ�á�
	ShrewMouse(const std::string & name, Ogre::SceneManager * sm, Ogre::Vector3 & pos);
	~ShrewMouse(void);
	//���º����������õ�������Ӧ�Ķ���������Ϊ���ε��õ�ʱ������
	void update(float interval);
	//���������仰˵�õ���Ӷ����������
	void enable(void);
	//����Ƿ��ڵڶ�����
	bool isEnable(void);
private:
	//�����Ƿ��ڶ�����
	bool _isEnable;
	//�����ʵ��
	Ogre::Entity * _ent;
	//һ��ʱ��ֵ�������ʱ��Ϊ���ʱ�����ض����档
	float _time;

};

#endif