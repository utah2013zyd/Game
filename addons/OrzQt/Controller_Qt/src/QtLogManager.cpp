#include "QtLogManager.h"

QtLogManager& QtLogManager::getSingleton(void)
{
	return static_cast<QtLogManager&> (ILogManager::getSingleton());
}
QtLogManager* QtLogManager::getSingletonPtr(void)
{
	return static_cast<QtLogManager*> (ILogManager::getSingletonPtr());
}



void QtLogManager::addWindow(LogWindow * window)
{
	_windows.push_back(window);
}
void QtLogManager::removeWindow(LogWindow * window)
{
	_windows.erase(std::remove(_windows.begin(), _windows.end(), window), _windows.end());
}

void QtLogManager::unicode(const std::wstring & unicodet)
{
	std::vector<LogWindow *>::iterator it;
	for(it = _windows.begin(); it != _windows.end(); ++it)
	{
		//QString str = ;
		(*it)->append(QString::fromStdWString(unicodet));
	}
}