#ifndef __Orz_ToolkitPlus_PreDeclare_H_
#define __Orz_ToolkitPlus_PreDeclare_H_

//template <typename Mutex> class boost::shared_lock;
//template <typename Mutex> class boost::unique_lock;
namespace Orz
{	
// boost
	class boost::thread;
	typedef boost::shared_ptr<boost::thread> ThreadShrPtr;

	class boost::shared_mutex;
	typedef boost::shared_ptr<boost::shared_mutex> MutexShrPtr;

// Toolkit:
	class FastIdr;
	class FastIdrCluster;
	class Framer;
	class NumChooser;
	class Rander;
	class WeakTimer;
	class Agent;
	class Log;
	struct ClrId;
	class IFreeList;
	class OrzAsioLogger;
	class OrzAsioException;
	class IQueryResult;
	class ISqlField;
	class IDatabase;
	class IPostQueryHdr;
	class IQueryResultHdr;
	class Query;
	class QueryArg;
	class QueryQueue;
	class ITimer;
	class ITimerThread;
	class MTDynLib;
	class MTDynLibMgr;
	class MTPluginMgr;
	class IOrzPlugin;
	typedef uint32 DBCnId;
	typedef std::string DBUserData;
	typedef uint32 DBId;
	typedef ClrId TmrId;
	typedef ClrId AynQId;
	typedef uint32	OrzPluginId;
	typedef uint32	MTDynLibId;
	template <typename Resource> class ResCluster;

	typedef boost::shared_ptr<FastIdr> FastIdrShrPtr;
	typedef boost::shared_ptr<FastIdrCluster> FastIdrClusterShrPtr;
	typedef boost::shared_ptr<Framer> FramerShrPtr;
	typedef boost::shared_ptr<NumChooser> NumChooserShrPtr;
	typedef boost::shared_ptr<Rander> RanderShrPtr;
	typedef boost::shared_ptr<WeakTimer> WeakTimerShrPtr;
	typedef boost::shared_ptr<Agent> AgentShrPtr;
	typedef boost::shared_ptr<IFreeList> IFreeListShrPtr;
	typedef boost::shared_ptr<OrzAsioLogger> OrzAsioLoggerShrPtr;
	typedef boost::shared_ptr<OrzAsioException> OrzAsioExceptionShrPtr;
	typedef boost::shared_ptr<IQueryResult> IQueryResultShrPtr;
	typedef boost::shared_ptr<WeakTimer> WeakTimerShrPtr;
	typedef boost::shared_ptr<ITimer> ITimerShrPtr;
	typedef boost::shared_ptr<ITimerThread> ITimerThreadShrPtr;
	typedef boost::shared_ptr<MTDynLib> MTDynLibShrPtr;
	typedef boost::shared_ptr<MTDynLibMgr> MTDynLibMgrShrPtr;
	typedef boost::shared_ptr<MTPluginMgr> MTPluginMgrShrPtr;
	typedef boost::shared_ptr<boost::any> BoostAnyPtr;
	typedef boost::shared_ptr<IPostQueryHdr> IPostQueryHdrShrPtr;
	typedef boost::shared_ptr<IQueryResultHdr> IQueryResultHdrShrPtr;
	typedef boost::shared_ptr<QueryQueue> QueryQueueShrPtr;
	typedef boost::shared_ptr<Query> QueryShrPtr;
	typedef boost::shared_ptr<Log> LogShrPtr;
}
#endif
