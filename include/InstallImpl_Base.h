#ifndef __INSTALL_IMPL_BASE_H__
#define __INSTALL_IMPL_BASE_H__

#include <string>

namespace SubutaiLauncher
{
    class InstallImpl_Base
    {
        public:
            void setFilename(const std::string& filename);
            void setSrcPath(const std::string& path);
            void setDstPath(const std::string& path);
            std::string getFilename() const;
            std::string getSrcPath() const;
            std::string getDstPath() const;
        protected:
            std::string _filename;
            std::string _srcPath;
            std::string _dstPath;
    };
};

#endif
