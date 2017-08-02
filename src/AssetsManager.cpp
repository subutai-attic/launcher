#include "AssetsManager.h"

namespace SubutaiLauncher
{
    const std::vector<std::string> AssetsManager::ASSETS_LIST({
            "launcher-logo.png",
            "launcher-facebook-active.png",
            "launcher-facebook-inactive.png",
            "launcher-twitter-active.png",
            "launcher-twitter-inactive.png",
            "launcher-github-active.png",
            "launcher-github-inactive.png",
            "launcher-linkedin-active.png",
            "launcher-linkedin-inactive.png",
            "launcher-github.png",
            "launcher-news-active.png",
            "launcher-news-inactive.png",
            "launcher-library-active.png",
            "launcher-library-inactive.png",
            "launcher-templates-active.png",
            "launcher-templates-inactive.png",
            "launcher-community-active.png",
            "launcher-community-inactive.png",
            "launcher-robot.png"
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
            // Uncomment this to bypass verification
            continue;
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

    void AssetsManager::download(const std::string& name)
    {
        Downloader *pDownloader = Session::instance()->getDownloader();
        if (!pDownloader)
        {
            _logger->error("Failed to retrieve downloader for assets manager");
            return;
        }
        _logger->trace("Downloading %s", name);
        pDownloader->reset();
        pDownloader->setFilename(name);
        if (pDownloader->retrieveFileInfo())
        {
            _logger->trace("Downloading asset: %s", name);
            auto t = pDownloader->download();
            t.join();
        }
        else
        {
            _logger->debug("Skipping %s", name);
        }

    }
};
