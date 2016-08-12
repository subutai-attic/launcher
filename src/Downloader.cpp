#include "Downloader.h"

namespace SubutaiLauncher {

    const std::string Downloader::URL = "https://cdn.subut.ai:8338";
    const std::string Downloader::REST = "/kurjun/rest/raw";

    Downloader::Downloader() : _content(""), _done(false) {
        std::printf("Starting Downloader\n");
    }

    Downloader::~Downloader() {

    }

    void Downloader::setFilename(const std::string& filename) {
        _filename = filename;
    }

    std::string Downloader::buildRequest(std::string path, std::string key, std::string value) {
        char r[1024];
        if (!key.empty()) {
            std::sprintf(r, "%s%s/%s?%s=%s", URL.c_str(), REST.c_str(), path.c_str(), key.c_str(), value.c_str());
        } else {
            std::sprintf(r, "%s%s/%s", URL.c_str(), REST.c_str(), path.c_str());
        }
        std::printf("Requesting: %s\n", r);
        return std::string(r);
    }

    bool Downloader::retrieveFileInfo() {
        auto curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, buildRequest("info", "name", _filename).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleInfo);
        auto result = curl_easy_perform(curl);
        if (result == 0) {
            return true;
        }
        return false;
    }

    size_t Downloader::handleInfo(char *data, size_t size, size_t nmemb, void *p) {
        return static_cast<Downloader*>(p)->handleInfoImpl(data, size, nmemb);
    }

    size_t Downloader::handleInfoImpl(char* data, size_t size, size_t nmemb) {
        _content.clear();
        std::printf("Parsing file info: %s\n", data);
        _content.append(data, size * nmemb);
        std::printf("content append failed\n");
        Json::Value root;
        std::istringstream str(_content);
        // TODO: Review stream
        str >> root;

        const Json::Value owners = root["owner"];
        for (int i = 0; i < owners.size(); ++i) {
            _file.owner = owners[i].asString();
        }
        _file.name = root.get("name", "").asString();
        _file.id = root.get("id", "").asString();
        _file.size = root.get("size", "").asLargestInt();

        std::printf("File info:\n");
        std::printf("Owner: %s\n", _file.owner.c_str());
        std::printf("Name: %s\n", _file.name.c_str());
        std::printf("Id: %s\n", _file.id.c_str());
        std::printf("Size: %o\n", _file.size);

        return size * nmemb;
    }

    std::thread Downloader::download() {
        _progress = 0;
        std::printf("Starting download of a file: %s\n", _filename.c_str());
        _done = false;
        //return std::thread(&Downloader::downloadImpl, this);
        return std::thread( [=] { downloadImpl(); } );
    }

    void Downloader::downloadImpl() {
        std::printf("Starting downloader thread\n");
        auto curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, buildRequest("get", "name", _filename).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleFile);
        auto result = curl_easy_perform(curl);
        if (result == 0) {
            std::printf("Download completed without any errors\n");
        } else {
            std::printf("Download completed with errors\n");
        }
        _done = true;
    }

    size_t Downloader::handleFile(char *data, size_t size, size_t nmemb, void *p) {
        return static_cast<Downloader*>(p)->handleFileImpl(data, size, nmemb);
    }

    size_t Downloader::handleFileImpl(char* data, size_t size, size_t nmemb) {
        _content.clear();
        _content.append(data, size * nmemb);

        _progress += size * nmemb;

        std::ofstream out(_file.name.c_str(), std::fstream::app);
        if (!out) {
            std::printf("Couldn't open file for writing");
        } else {
            out << _content;
            out.close();
        }

        return size * nmemb;
    }

    bool Downloader::isDone() {
        return _done;
    }

    int Downloader::getPercent() {
        return (int)(_progress/(_file.size/100));
    }

    bool Downloader::verifyDownload() {
        return true;
    }

};
