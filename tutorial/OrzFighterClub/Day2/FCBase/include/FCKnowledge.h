#ifndef __Orz_FCBase_FCKnowledge_h__
#define __Orz_FCBase_FCKnowledge_h__


#include "FCBaseConfig.h"
#include "FCMapInfo.h"
#include <orz/Toolkit_Base/Singleton.h>
namespace Orz
{
class _FCBaseExport FCKnowledge: public Singleton<FCKnowledge>
{
public:
	FCKnowledge(void);
	virtual ~FCKnowledge(void);
	FCMapInfo & mapInfo(void);
private:
	FCMapInfo _map;
};


typedef  boost::shared_ptr<FCKnowledge> FCKnowledgePtr;
}

#endif