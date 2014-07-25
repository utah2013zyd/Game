#ifndef __Orz_ToolkitPlus_Logger_h__
#define __Orz_ToolkitPlus_Logger_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Log/LogFreeList.h>
#include <orz/Toolkit_Plus/Log/Log.hpp>
#include <orz/Toolkit_Plus/Plugin/IOrzPlugin.h>
namespace Orz
{
class LoggerImpl;
//! Logger类
class _OrzToolkitPlusExport OrzAsioLogger
	: private boost::noncopyable
	, public IOrzPlugin
{
public:
	//! 构造函数
	/*!
	 *	\param[in] _config log4cxx的配置文件路径及文件名
	 *	\param[in] _buffCount 缓冲区的数目，有几个就能让相同数目的线程同时waitfree的使用
	 *	\param[in] _buffQueueSize 一级缓冲区使用的队列们的最大排队中的元素个数
	 *	\param[in] _buffQueueSize 二级缓冲区使用的队列的最大排队中的元素个数
	 *	\note 最好让_buffQueueSize*_buffCount == _doubleBuffQueueSize
	 */
	OrzAsioLogger(const char* _config,  uint32 _buffCount=5, uint32 _buffQueueSize=1000, 
		uint32 _doubleBuffQueueSize=5000, uint32 _logPoolClr=10, uint32 _logPoolPer=100) throw();
	~OrzAsioLogger() throw();

public:
	//! 启动Log系统
	bool					boost					();

	//! 关闭Log系统
	void					shutdown				();

	//! 合并Log系统内部线程到调用OrzAsioLogger::boost的线程
	void					join					();

	//! 写Log
	/*!
	 *	\param[in] _ol Log的等级，见OutLevel
	 *	\param[in] _str 要输出的字符串
	 *	\note 输出的结尾一定要用0
	 *	\note 每一个_str大小不要超过1024字节
	 *	\note 此方法不使用double-buff来输出log
	 example:
		sLogger->out(OL_DEBUG, "Log - Id:[", d2s(1).c_str(), "]!", 0);
	 */
	void					out						(OutLevel _ol, const char* _str, ... ) throw();

	//! 写Log
	/*!
	 *	\param[in] _ol Log的等级，见OutLevel
	 *	\param[in] _str 要输出的字符串
	 *	\note 输出的结尾一定要用0
	 *	\note 每一个_str大小不要超过1024字节
	 *	\note 此方法使用double-buff来输出log
	 example:
		sLogger->out(OL_DEBUG, "Log - Id:[", d2s(1).c_str(), "]!", 0);
	 */
	void					outDBuff				(OutLevel _ol, const char* _str, ... ) throw();

	//! 写Log
	/*!
	 *	\param[in] _ol Log的等级，见OutLevel
	 *	\param[in] _str 要输出的字符串
	 *	\param[in] _str 要输出的字符串长度
	 *	\note 输出的结尾一定要用0
	 *	\note 每一个_str大小不要超过1024字节
	 *	\note 每个_str后面必须跟这个字符串的长度，如果写0则系统使用strlen来确定长度
	 example:
		sLogger->outByte(OL_DEBUG, "Log - Id:[", 0, d2s(1).c_str(), d2s(1).size(), "]!", 0, 0);
	 */
	void					outByte					(OutLevel _ol, const char* _str, uint32 _len, ... ) throw();

	//! 写Log
	/*!
	 *	\param[in] _ol Log的等级，见OutLevel
	 *	\param[in] _str 要输出的字符串
	 *	\note 输出的结尾一定要用0
	 *	\note 每一个_str大小不要超过1024字节
	 *	\note 此方法使用double-buff来输出log
	 example:
		sLogger->out(OL_DEBUG, "Log - Id:[", d2s(1).c_str(), "]!", 0);
	 */
	void					outByteDBuff			(OutLevel _ol, const char* _str, uint32 _len, ... ) throw();

	//! 写Log
	/*!
	 *	\param[in] _data 要输出的Log对象
	 example:
		Log log(OL_INFO);
		log.put("Log id: %d", 1);
		sLogger->out(log);
	 */
	void					out						(LogShrPtr _data) throw();

	//! 写Log
	/*!
	 *	\param[in] _usingDoubleBuff 是否使用doubule-buff来输出log
	 *	\param[in] _str 要输出的信息
	 example:
		sLogger->out("Log id: %d", 1);
	 */
	void					outFormat				(OutLevel _ol, bool _usingDoubleBuff, const char* _str, ... ) throw();

private:
	LoggerImpl*				m_impl;
};
}
#endif
