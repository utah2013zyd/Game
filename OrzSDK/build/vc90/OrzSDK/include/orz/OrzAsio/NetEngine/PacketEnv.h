#ifndef __Orz_OrzAsio_PacketEnv_h_
#define __Orz_OrzAsio_PacketEnv_h_
namespace Orz 
{
/** 
 *	\file OrzAsio�ṩ����������ʵ�ֵ���غ궨��
 */

//! ���ͷ�����������������С���ڴ�εĳ���
#define PACKET_CAPACITY_SIZE		4

//! ���ͷ�����������������͵��ڴ�εĳ���
#define PACKET_TYPE_SIZE			4

//! ���ͷ������������������ʱ����ڴ�εĳ���
#define PACKET_DATETIME_SIZE		9

//! ���ͷ������������������ʱ��������ڴ�εĳ���
#define PKT_DT_YEAR_SIZE			2

//! ���ͷ������������������ʱ�����µ��ڴ�εĳ���
#define PKT_DT_MONTH_SIZE			1

//! ���ͷ������������������ʱ�����յ��ڴ�εĳ���
#define PKT_DT_DAY_SIZE				1

//! ���ͷ������������������ʱ����Сʱ���ڴ�εĳ���
#define PKT_DT_HOUR_SIZE			1

//! ���ͷ������������������ʱ���з��ӵ��ڴ�εĳ���
#define PKT_DT_MIN_SIZE				1

//! ���ͷ������������������ʱ��������ڴ�εĳ���
#define PKT_DT_SEC_SIZE				1

//! ���ͷ������������������ʱ���к�����ڴ�εĳ���
#define PKT_DT_MSEC_SIZE			2

//! ���ͷ���������������б����ַ������ȵĵ��ڴ�εĳ���
#define STR_HEAD_SIZE				2

//! ���ͷ���������������б������鳤�ȵĵ��ڴ�εĳ���
#define ARRAY_HEAD_SIZE				2

//! ���ͷ���ĳ��� == PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE + PACKET_DATETIME_SIZE
#define PACKET_HEAD_SIZE			17

}
#endif
