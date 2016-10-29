#include "InstallImpl_Base.h"

void SubutaiLauncher::InstallImpl_Base::setFilename(const std::string& filename)
{
	_filename = filename;
}

void SubutaiLauncher::InstallImpl_Base::setSrcPath(const std::string& path)
{
	_srcPath = path;
}

void SubutaiLauncher::InstallImpl_Base::setDstPath(const std::string& path)
{
	_dstPath = path;
}

std::string SubutaiLauncher::InstallImpl_Base::getFilename() const
{
	return _filename;
}

std::string SubutaiLauncher::InstallImpl_Base::getSrcPath() const
{
	return _srcPath;
}

std::string SubutaiLauncher::InstallImpl_Base::getDstPath() const
{
	return _dstPath;
}
