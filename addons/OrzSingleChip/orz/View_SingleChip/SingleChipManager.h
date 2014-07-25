#ifndef __Orz_ViewSingleChip_SingleChipManager_h__
#define __Orz_ViewSingleChip_SingleChipManager_h__

#include <orz/View_SingleChip/ViewSingleChipConfig.h>
#include <orz/Toolkit_Base/Singleton.h>
#include <boost/circular_buffer.hpp>

#include <orz/Framework_Base/System/SystemInterface.h>
namespace Orz
{
class _OrzViewSingleChipExport SingleChipListener
{
public:
	virtual void read(boost::circular_buffer<unsigned char> & data) = 0;
	virtual ~SingleChipListener(void){}

};
class SingleChipManagerImpl;
class _OrzViewSingleChipExport SingleChipManager : public Singleton<SingleChipManager>
{
public:
	SingleChipManager(void);
	~SingleChipManager(void);
	bool init(void);
	void shutdown(void);
	bool write(unsigned char * data, int num);
	bool write(char * data, int num);
	bool write(const MsgBuffer & buffer);

	bool update(TimeType i);

	void addListener(SingleChipListener * listener);
	void removeListener(SingleChipListener * listener);
private:
	boost::scoped_ptr<SingleChipManagerImpl> _impl;
};


SYSTEMUNIT_BEGIN(SingleChipManager) 
SYSTEMUNIT_INIT 
SYSTEMUNIT_UPDATE 
SYSTEMUNIT_SHUTDOWN 
SYSTEMUNIT_END(SingleChipManager)


}



#endif