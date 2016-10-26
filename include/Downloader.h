#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <cstdio>
#include <curl/curl.h>
#include <json/json.h>
#include <thread>

#include "FileSystem.h"
#include "MD5.h"

namespace SubutaiLauncher 
{

    struct SubutaiFile 
    {
        std::string owner;
        std::string name;
        std::string id;
        long size;
    };

    class Downloader 
    {
        public:
            static const std::string URL;
            static const std::string REST;

            Downloader();
            ~Downloader();
            void reset();
            void setFilename(const std::string& filename);
            bool retrieveFileInfo();
            SubutaiFile info();
            std::thread download();
            void downloadImpl();
            bool isDone();
            int getPercent();
            static size_t handleInfo(char* data, size_t size, size_t nmemb, void *p);
            size_t handleInfoImpl(char* data, size_t size, size_t nmemb);
            static size_t handleFile(char* data, size_t size, size_t nmemb, void *p);
            size_t handleFileImpl(char* data, size_t size, size_t nmemb);
            bool verifyDownload();
            long currentProgress();
            void setOutputDirectory(const std::string& dir);
            std::string getOutputDirectory() const;
            std::string getFullPath() const;
        private:
            std::string buildRequest(std::string path, std::string key, std::string value);
            std::string _filename;
            std::string _content;
            std::string _outputDir;
            SubutaiFile _file;
            long _progress;
            bool _done;
    };

};

#endif
