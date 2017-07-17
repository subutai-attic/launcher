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
        _logger = &Poco::Logger::get("subutai");
        _logger->debug("Starting Assets Manager");
    }

    AssetsManager::~AssetsManager()
    {

    }

    void AssetsManager::verify()
    {
        _logger->debug("Verifying assets");
        //std::string pDir = Session::instance()->getSettings()->getTmpPath();
        Downloader *pDownloader = Session::instance()->getDownloader();
        if (!pDownloader)
        {
            _logger->error("Failed to retrieve downloader for assets manager");
            return;
        }
        for (auto it = ASSETS_LIST.begin(); it != ASSETS_LIST.end(); it++)
        {
            _logger->trace("Downloading %s", (*it));
            pDownloader->reset();
            pDownloader->setFilename((*it));
            if (pDownloader->retrieveFileInfo())
            {
                _logger->trace("Downloading asset: %s", (*it));
                auto t = pDownloader->download();
                t.join();
            }
            else
            {
                _logger->debug("Skipping %s", (*it));
            }
        }
    }
};
