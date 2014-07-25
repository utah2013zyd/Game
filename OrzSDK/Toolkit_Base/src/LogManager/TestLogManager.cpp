#include <orz/Toolkit_Base/LogManager/TestLogManager.h>
using namespace Orz;


void TestLogManager::unicode(const std::wstring & text)
{
	_logs.push_back(text);
}
const std::wstring & TestLogManager::get(int i)
{
	return _logs.at(i);
}

size_t TestLogManager::size()
{
	return _logs.size();
}
void TestLogManager::clear()
{
	_logs.clear();
}

