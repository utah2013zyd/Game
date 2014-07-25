#ifndef __Orz_OrzAsio_PacketEnv_h_
#define __Orz_OrzAsio_PacketEnv_h_
namespace Orz 
{
/** 
 *	\file OrzAsio提供的网络封包类实现的相关宏定义
 */

//! 封包头部中容纳这个封包体大小的内存段的长度
#define PACKET_CAPACITY_SIZE		4

//! 封包头部中容纳这个封包类型的内存段的长度
#define PACKET_TYPE_SIZE			4

//! 封包头部中容纳这个封包日期时间的内存段的长度
#define PACKET_DATETIME_SIZE		9

//! 封包头部中容纳这个封包日期时间中年的内存段的长度
#define PKT_DT_YEAR_SIZE			2

//! 封包头部中容纳这个封包日期时间中月的内存段的长度
#define PKT_DT_MONTH_SIZE			1

//! 封包头部中容纳这个封包日期时间中日的内存段的长度
#define PKT_DT_DAY_SIZE				1

//! 封包头部中容纳这个封包日期时间中小时的内存段的长度
#define PKT_DT_HOUR_SIZE			1

//! 封包头部中容纳这个封包日期时间中分钟的内存段的长度
#define PKT_DT_MIN_SIZE				1

//! 封包头部中容纳这个封包日期时间中秒的内存段的长度
#define PKT_DT_SEC_SIZE				1

//! 封包头部中容纳这个封包日期时间中毫秒的内存段的长度
#define PKT_DT_MSEC_SIZE			2

//! 封包头部中容纳这个封包中保存字符串长度的的内存段的长度
#define STR_HEAD_SIZE				2

//! 封包头部中容纳这个封包中保存数组长度的的内存段的长度
#define ARRAY_HEAD_SIZE				2

//! 封包头部的长度 == PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE + PACKET_DATETIME_SIZE
#define PACKET_HEAD_SIZE			17

}
#endif
