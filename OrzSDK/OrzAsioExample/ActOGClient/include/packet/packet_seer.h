#ifndef __Orz_ActOGServer_packet_seer_h__
#define __Orz_ActOGServer_packet_seer_h__
#include <packet/packet.h>
namespace Orz
{
#define ARG_NAME_MAX_SIZE 16
#define ARG_DATA_MAX_SIZE 32

typedef MemString<ARG_NAME_MAX_SIZE> arg_name;
typedef MemString<ARG_DATA_MAX_SIZE> arg_data;

struct Arg
{
	Arg() {}
	~Arg() {}

	arg_name		name;
	arg_data		arg;
};

struct Args
{
	Args() {}
	~Args() {}

	std::vector<Arg>	argList;
};
class packet_seer
{
public:
	packet_seer() {}
	~packet_seer() {}

public:
	void				analyse_packet					(IPacketShrPtr _pkt, Args& _args, const std::vector<char>& _spts)
	{
		typedef MemString<PKT_MAX_SIZE> msg_str;
		typedef MemString<PKT_MAX_SIZE> mstr;

		StringPtr ms;
		_pkt->get(ms, _pkt->getSize());
		msg_str msg(ms.ptr(), ms.size());

		_args.argList.clear();
		std::vector<Arg>& argList = _args.argList;

		std::deque<mstr> str_list;
		str_list.push_back(mstr(msg.c_str(), msg.size()));

		// 首先分解字符串到指定的最低层，装在一个deque中
		//analyse_string(_pBuff, 0, '=', '&', '|', '\0', argList);
		std::vector<char>::const_iterator itr(_spts.begin());
		for (; itr!=_spts.end(); ++itr)
		{
			char spt = *itr;
			std::deque<mstr> strs_temp;
			std::deque<mstr>::iterator sitr(str_list.begin());
			for (; sitr!=str_list.end(); ++sitr)
			{
				mstr& str = *sitr;
				std::vector<PacketData> strs;
				separateStr(str.c_str(), (uint32)str.size(), spt, strs);
				std::vector<PacketData>::iterator itr2(strs.begin());
				for (; itr2!=strs.end(); ++itr2)
				{
					strs_temp.push_back(mstr((*itr2).c_str(), (*itr2).size()));
				}
			}
			str_list.clear();
			str_list = strs_temp;
		}

		// 开始解析
		std::deque<mstr>::iterator stritr(str_list.begin());
		for (; stritr!=str_list.end(); ++stritr)
		{
			mstr& str = *stritr;
			std::vector<Arg> arg_list_temp;
			analyse_string(str.c_str(), (uint32)str.size(), 0, '=', '&', '|', '\0', arg_list_temp);
			std::vector<Arg>::iterator aitr(arg_list_temp.begin());
			for (; aitr!=arg_list_temp.end(); ++aitr)
			{
				argList.push_back(*aitr);
			}
		}
	}

	void					analyse_packet					(IPacketShrPtr _pkt, Args& _args, char _spt)
	{
		typedef MemString<PKT_MAX_SIZE> msg_str;
		StringPtr ms;
		_pkt->get(ms, _pkt->getSize());
		msg_str msg(ms.ptr(), ms.size());

		_args.argList.clear();
		std::vector<Arg>& argList = _args.argList;

		analyse_string(msg.c_str(), msg.size(), 0, '=', _spt, '|', '\0', argList);
	}

	size_t					analyse_string					(const char* _pBuff, uint32 _size, uint32 _seek, const char _nlinka, const char _spt, const char _endc, const char _argdt, std::vector<Arg>& _argList)
	{
		if (!_pBuff)
			return 0;

		if (_pBuff[0] == _nlinka || _pBuff[0] == _endc || _pBuff[0] == '\0' || _pBuff[0] == _spt || _pBuff[0] == _argdt)
		{
			++_seek;
			return 0;
		}

		uint32 size = _size;

		if (_seek >= size)
			return 0;

		uint32 i =_seek;
		uint32 seek = i;
		_argList.push_back(Arg());
		while (i<size)
		{
			uint32 j=i;
			if (j >= size || _pBuff[j] == _endc || _pBuff[j] == '\0')
			{
				seek = ++j;
				goto OVER;
			}
			// 保存参数的名字
			while (j < size && _pBuff[j] != _nlinka && _pBuff[j] != _spt && _pBuff[j] != _endc && _pBuff[j] != '\0')
			{
				_argList.back().name.push_back(_pBuff[j]);
				++j;
			}
			if (j >= size || _pBuff[j] == _endc || _pBuff[j] == '\0')
			{
				seek = ++j;
				goto OVER;
			}

			uint32 temp = ++j;
			if (j >= size || _pBuff[j] == _endc || _pBuff[j] == '\0')
			{
				seek = ++j;
				goto OVER;
			}
			
			// 保存参数本身
			while (j < size && _pBuff[j] != _nlinka && _pBuff[j] != _spt && _pBuff[j] != _endc && _pBuff[j] != '\0')
			{
				if (!(_argdt != '\0' && _pBuff[j] == _argdt))
					_argList.back().arg.push_back(_pBuff[j]);
				j++;
				if (j >= size || _pBuff[j] == _endc)
				{
					seek = ++j;
					goto OVER;
				}
				else if (_pBuff[j] == _spt)
					_argList.push_back(Arg());
			}
			if (j == temp)
				_argList.push_back(Arg());
			i = ++j;
		}

	OVER:
		return seek;
	}
};
}
#endif
