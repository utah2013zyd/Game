#ifndef __Orz_ToolkitPlus_Logger_h__
#define __Orz_ToolkitPlus_Logger_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Log/LogFreeList.h>
#include <orz/Toolkit_Plus/Log/Log.hpp>
#include <orz/Toolkit_Plus/Plugin/IOrzPlugin.h>
namespace Orz
{
class LoggerImpl;
//! Logger��
class _OrzToolkitPlusExport OrzAsioLogger
	: private boost::noncopyable
	, public IOrzPlugin
{
public:
	//! ���캯��
	/*!
	 *	\param[in] _config log4cxx�������ļ�·�����ļ���
	 *	\param[in] _buffCount ����������Ŀ���м�����������ͬ��Ŀ���߳�ͬʱwaitfree��ʹ��
	 *	\param[in] _buffQueueSize һ��������ʹ�õĶ����ǵ�����Ŷ��е�Ԫ�ظ���
	 *	\param[in] _buffQueueSize ����������ʹ�õĶ��е�����Ŷ��е�Ԫ�ظ���
	 *	\note �����_buffQueueSize*_buffCount == _doubleBuffQueueSize
	 */
	OrzAsioLogger(const char* _config,  uint32 _buffCount=5, uint32 _buffQueueSize=1000, 
		uint32 _doubleBuffQueueSize=5000, uint32 _logPoolClr=10, uint32 _logPoolPer=100) throw();
	~OrzAsioLogger() throw();

public:
	//! ����Logϵͳ
	bool					boost					();

	//! �ر�Logϵͳ
	void					shutdown				();

	//! �ϲ�Logϵͳ�ڲ��̵߳�����OrzAsioLogger::boost���߳�
	void					join					();

	//! дLog
	/*!
	 *	\param[in] _ol Log�ĵȼ�����OutLevel
	 *	\param[in] _str Ҫ������ַ���
	 *	\note ����Ľ�βһ��Ҫ��0
	 *	\note ÿһ��_str��С��Ҫ����1024�ֽ�
	 *	\note �˷�����ʹ��double-buff�����log
	 example:
		sLogger->out(OL_DEBUG, "Log - Id:[", d2s(1).c_str(), "]!", 0);
	 */
	void					out						(OutLevel _ol, const char* _str, ... ) throw();

	//! дLog
	/*!
	 *	\param[in] _ol Log�ĵȼ�����OutLevel
	 *	\param[in] _str Ҫ������ַ���
	 *	\note ����Ľ�βһ��Ҫ��0
	 *	\note ÿһ��_str��С��Ҫ����1024�ֽ�
	 *	\note �˷���ʹ��double-buff�����log
	 example:
		sLogger->out(OL_DEBUG, "Log - Id:[", d2s(1).c_str(), "]!", 0);
	 */
	void					outDBuff				(OutLevel _ol, const char* _str, ... ) throw();

	//! дLog
	/*!
	 *	\param[in] _ol Log�ĵȼ�����OutLevel
	 *	\param[in] _str Ҫ������ַ���
	 *	\param[in] _str Ҫ������ַ�������
	 *	\note ����Ľ�βһ��Ҫ��0
	 *	\note ÿһ��_str��С��Ҫ����1024�ֽ�
	 *	\note ÿ��_str������������ַ����ĳ��ȣ����д0��ϵͳʹ��strlen��ȷ������
	 example:
		sLogger->outByte(OL_DEBUG, "Log - Id:[", 0, d2s(1).c_str(), d2s(1).size(), "]!", 0, 0);
	 */
	void					outByte					(OutLevel _ol, const char* _str, uint32 _len, ... ) throw();

	//! дLog
	/*!
	 *	\param[in] _ol Log�ĵȼ�����OutLevel
	 *	\param[in] _str Ҫ������ַ���
	 *	\note ����Ľ�βһ��Ҫ��0
	 *	\note ÿһ��_str��С��Ҫ����1024�ֽ�
	 *	\note �˷���ʹ��double-buff�����log
	 example:
		sLogger->out(OL_DEBUG, "Log - Id:[", d2s(1).c_str(), "]!", 0);
	 */
	void					outByteDBuff			(OutLevel _ol, const char* _str, uint32 _len, ... ) throw();

	//! дLog
	/*!
	 *	\param[in] _data Ҫ�����Log����
	 example:
		Log log(OL_INFO);
		log.put("Log id: %d", 1);
		sLogger->out(log);
	 */
	void					out						(LogShrPtr _data) throw();

	//! дLog
	/*!
	 *	\param[in] _usingDoubleBuff �Ƿ�ʹ��doubule-buff�����log
	 *	\param[in] _str Ҫ�������Ϣ
	 example:
		sLogger->out("Log id: %d", 1);
	 */
	void					outFormat				(OutLevel _ol, bool _usingDoubleBuff, const char* _str, ... ) throw();

private:
	LoggerImpl*				m_impl;
};
}
#endif
