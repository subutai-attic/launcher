#include "InstallImpl_Windows.h"

#if LAUNCHER_WINDOWS
SubutaiLauncher::InstallImpl::InstallImpl()
{

}

void SubutaiLauncher::InstallImpl::preInstall()
{
	FileSystem fs;
	fs.setPath(_srcPath);
	if (!fs.isFileExists(_filename))
	{
		throw SubutaiException("Can't install. Source file was not found");
	}
}

void SubutaiLauncher::InstallImpl::install()
{
	
}

void SubutaiLauncher::InstallImpl::postInstall()
{
	
}
#endif