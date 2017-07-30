#ifndef __ASSETS_MANAGER_H__
#define __ASSETS_MANAGER_H__

#include <vector>
#include <string>

#include "Poco/Logger.h"

#include "Vars.h"
#include "Session.h"
#include "Downloader.h"

namespace SubutaiLauncher
{
    class AssetsManager
    {
        static const std::vector<std::string> ASSETS_LIST;
        public:
            AssetsManager();
            ~AssetsManager();
            void verify();
            void download(const std::string& name);
        private:
            Poco::Logger* _logger;
        protected:
    };
}

#endif
