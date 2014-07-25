#ifndef __Orz_FCBase_FCObject_h__
#define __Orz_FCBase_FCObject_h__

#include "FCBaseConfig.h"

namespace Orz
{

	class _FCBaseExport FCObject : public Actor
	{
	public:
		FCObject(const std::string & name = IDManager::BLANK, int x = 0, int y = 0);
		virtual ~FCObject(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//1在每一个frame调用

		int getX(void);
		int getY(void);
	protected:
		int _x;
		int _y;
		
		//EventSpacePtr _otdES;
	
	};
	

		

}

#endif 