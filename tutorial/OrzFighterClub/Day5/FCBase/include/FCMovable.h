#ifndef __Orz_FCBase_FCMovable_h__
#define __Orz_FCBase_FCMovable_h__

#include "FCBaseConfig.h"
#include "FCObject.h"

#include "FCPather.h"
namespace Orz
{

	class _FCBaseExport FCMovable : public FCObject
	{
	public:
	

		FCMovable(const std::string & name = IDManager::BLANK, int x = 0, int y = 0);
		virtual ~FCMovable(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//1在每一个frame调用
		bool patherSolve(int x, int y);
		virtual bool canMove(int x, int y);
	protected:
		
		FCPatherPtr _pather;
	
	};
	

		

}

#endif 