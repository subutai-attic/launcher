#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <curl/curl.h>
#include <json/json.h>

struct SubutaiFile {
    std::string owner;
    std::string md5;
    std::string name;
    std::string id;
    std::string fingerprint;
    long size;
};

class SubutaiDownloader {
    public:
        static const std::string URL;
        static const std::string REST;

        SubutaiDownloader(std::string filename);
        ~SubutaiDownloader();
        bool retrieveFileInfo();
        bool download();
        bool isDone();
        int getPercent();
        static size_t handleInfo(char* data, size_t size, size_t nmemb, void *p);
        size_t handleInfoImpl(char* data, size_t size, size_t nmemb);
        static size_t handleFile(char* data, size_t size, size_t nmemb, void *p);
        size_t handleFileImpl(char* data, size_t size, size_t nmemb);
    private:
        std::string buildRequest(std::string path, std::string key, std::string value);
        std::string _filename;
        std::string _content;
        SubutaiFile _file;
        long _progress;
        bool _done;
};

#endif
