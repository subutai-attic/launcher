#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include "Vars.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <cstdio>
#include <thread>
#include <memory>

#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include "Poco/URI.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/File.h"
#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
#include "Poco/FileStream.h"
#include "Poco/Logger.h"
#include "Poco/LogStream.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/SSLException.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/FTPStreamFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/HTTPResponse.h"

#include "FileSystem.h"

using Poco::Logger;
using Poco::LogStream;

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
			static const std::string TREST;
            static const std::string HOST;
            static const int PORT = 8338;

            Downloader();
            ~Downloader();
            void reset();
            void setFilename(const std::string& filename);
            bool retrieveFileInfo(bool tmpl = false);
			bool retrieveTemplateInfo();
            SubutaiFile info();
            std::thread download();
            void downloadImpl();
            bool isDone();
            double getPercent();
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
            std::string _rfile; // output file
            SubutaiFile _file;
            long _progress;
            bool _done;
            Poco::Logger* _logger;
	};

}

#endif
