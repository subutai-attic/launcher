#ifndef __ASSETS_MANAGER_H__
#define __ASSETS_MANAGER_H__

#include <vector>
#include <string>

#include "Vars.h"
#include "Session.h"

namespace SubutaiLauncher
{
    class AssetsManager
    {
        static const std::vector<std::string> ASSETS_LIST;
        public:
            AssetsManager();
            ~AssetsManager();
            void verify();
        private:
        protected:
    };
}

#endif
