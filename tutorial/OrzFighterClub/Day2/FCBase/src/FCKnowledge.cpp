#include "FCKnowledge.h"
#include "FCMapInfo.h"
using namespace Orz;

template<> FCKnowledge* Singleton<FCKnowledge>::_singleton = NULL;
FCKnowledge::FCKnowledge(void)
{

}
FCKnowledge::~FCKnowledge(void)
{

}
FCMapInfo & FCKnowledge::mapInfo(void)
{
	return _map;
}