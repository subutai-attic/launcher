#include "InstallImpl_Base.h"

namespace SubutaiLauncher
{
    void InstallImpl_Base::setFilename(const std::string& filename)
    {
        _filename = filename;
    }

    void InstallImpl_Base::setSrcPath(const std::string& path)
    {
        _srcPath = path;
    }

    void InstallImpl_Base::setDstPath(const std::string& path)
    {
        _dstPath = path;
    }

    std::string InstallImpl_Base::getFilename() const
    {
        return _filename;
    }

    std::string InstallImpl_Base::getSrcPath() const
    {
        return _srcPath;
    }

    std::string InstallImpl_Base::getDstPath() const 
    {
        return _dstPath;
    }
};
