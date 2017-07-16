#include "AssetsManager.h"

namespace SubutaiLauncher
{
    const std::vector<std::string> AssetsManager::ASSETS_LIST({
            "launcher-logo.png",
            "launcher-facebook.png",
            "launcher-github.png"
            });

    AssetsManager::AssetsManager()
    {

    }

    AssetsManager::~AssetsManager()
    {

    }

    void AssetsManager::verify()
    {
        std::string pDir = Session::instance()->getSettings()->getTmpPath();
        Downloader *pDownloader = Session::instance()->getDownloader();
        for (auto it = ASSETS_LIST.begin(); it != ASSETS_LIST.end(); it++)
        {
            pDownloader->reset();
            pDownloader->setFilename((*it));
            pDownloader->download();
        }
    }
};
