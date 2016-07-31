#include "Downloader.h"

const std::string SubutaiDownloader::URL = "https://cdn.subut.ai:8338";
const std::string SubutaiDownloader::REST = "/kurjun/rest/file";

SubutaiDownloader::SubutaiDownloader(std::string filename) {
    _filename = filename;
    std::printf("Starting downloader\n");
}

SubutaiDownloader::~SubutaiDownloader() {

}

std::string SubutaiDownloader::buildRequest(std::string path, std::string key, std::string value) {
    char r[1024];
    if (!key.empty()) {
        std::sprintf(r, "%s%s/%s?%s=%s", URL.c_str(), REST.c_str(), path.c_str(), key.c_str(), value.c_str());
    } else {
        std::sprintf(r, "%s%s/%s", URL.c_str(), REST.c_str(), path.c_str());
    }
    std::printf("Requesting: %s\n", r);
    return std::string(r);
}

bool SubutaiDownloader::retrieveFileInfo() {
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

size_t SubutaiDownloader::handleInfo(char *data, size_t size, size_t nmemb, void *p) {
    return static_cast<SubutaiDownloader*>(p)->handleInfoImpl(data, size, nmemb);
}

size_t SubutaiDownloader::handleInfoImpl(char* data, size_t size, size_t nmemb) {
    _content.append(data, size * nmemb);
    Json::Value root;
    std::istringstream str(_content);
    // TODO: Review stream
    str >> root;

    const Json::Value owners = root["owner"];
    for (int i = 0; i < owners.size(); ++i) {
        _file.owner = owners[i].asString();
    }
    _file.md5 = root.get("md5Sum", "").asString();
    _file.name = root.get("name", "").asString();
    _file.id = root.get("id", "").asString();
    _file.fingerprint = root.get("fingerprint", "").asString();
    _file.size = root.get("size", "").asLargestInt();

    std::printf("File info:\n");
    std::printf("Owner: %s\n", _file.owner.c_str());
    std::printf("Name: %s\n", _file.name.c_str());
    std::printf("Checksum: %s\n", _file.md5.c_str());
    std::printf("Id: %s\n", _file.id.c_str());
    std::printf("Fingerprint: %s\n", _file.fingerprint.c_str());
    std::printf("Size: %d\n", _file.size);

    return size * nmemb;
}

bool SubutaiDownloader::download() {
    auto curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, buildRequest("get", "name", _filename).c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleFile);
    auto result = curl_easy_perform(curl);
    if (result == 0) {
        _done = true;
        return true;
    }
    return false;
}

size_t SubutaiDownloader::handleFile(char *data, size_t size, size_t nmemb, void *p) {
    return static_cast<SubutaiDownloader*>(p)->handleFileImpl(data, size, nmemb);
}

size_t SubutaiDownloader::handleFileImpl(char* data, size_t size, size_t nmemb) {
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

bool SubutaiDownloader::isDone() {
    return _done;
}

int SubutaiDownloader::getPercent() {
    return 1;
}
