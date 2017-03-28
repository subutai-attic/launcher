#include "Downloader.h"

const std::string SubutaiLauncher::Downloader::URL = "https://devcdn.subut.ai:8338";
const std::string SubutaiLauncher::Downloader::REST = "/kurjun/rest/raw";
const std::string SubutaiLauncher::Downloader::HOST = "devcdn.subut.ai";

SubutaiLauncher::Downloader::Downloader() : 
    _content(""),
    _done(false),
    _outputDir(".")
{
    Log::instance()->logger()->debug() << "Starting Downloader instance" << std::endl;
}

SubutaiLauncher::Downloader::~Downloader()
{

}

void SubutaiLauncher::Downloader::reset()
{
    _file.name = "";
    _file.owner = "";
    _file.id = "";
    _file.size = 0;
    _content = "";
    _filename = "";
    _progress = 0;
    _done = false;
}

void SubutaiLauncher::Downloader::setFilename(const std::string& filename)
{
    _filename = filename;
    Log::instance()->logger()->debug() << "Downloader::setFilename after" << _filename << std::endl;
}

std::string SubutaiLauncher::Downloader::buildRequest(std::string path, std::string key, std::string value)
{
    char r[1024];
    if (!key.empty()) {
#if LAUNCHER_WINDOWS
        sprintf_s(r, sizeof(r), "%s%s/%s?%s=%s", URL.c_str(), REST.c_str(), path.c_str(), key.c_str(), value.c_str());
#else
        std::sprintf(r, "%s%s/%s?%s=%s", URL.c_str(), REST.c_str(), path.c_str(), key.c_str(), value.c_str());
#endif
    }
    else {
#if LAUNCHER_WINDOWS
        sprintf_s(r, sizeof(r), "%s%s/%s", URL.c_str(), REST.c_str(), path.c_str());
#else
        std::sprintf(r, "%s%s/%s", URL.c_str(), REST.c_str(), path.c_str());
#endif
    }
    Log::instance()->logger()->debug() << "Requesting " << r << std::endl;
    return std::string(r);
}

bool SubutaiLauncher::Downloader::retrieveFileInfo()
{
    auto l = Log::instance()->logger();
    Poco::Net::HTTPSClientSession s(HOST, PORT);
    std::string path(REST);
    path.append("/info");
    l->debug() << "Requesting file info: " << path << std::endl;
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path);
    Poco::Net::HTMLForm form;
    form.add("name", _filename);
    form.prepareSubmit(request);
    s.sendRequest(request);

    Poco::Net::HTTPResponse response;
    std::istream& rs = s.receiveResponse(response);
    std::string output;
    Poco::StreamCopier::copyToString(rs, output);

    l->debug() << "Received file info: " << output << std::endl;

    //l->debug() << "Before JSon(change to Poco!): responce: " << output << std::endl;

    // TODO: Replace JSON lib with Poco
    Json::Value root;

    //l->debug() << "JSon(change to Poco!): root copied: " << std::endl;
    try { 
        std::istringstream str(output);
        str >> root;

        Json::Value el;
        if (root.isArray()) {
            l->debug() << "JSON root is array" << std::endl;
            el = root[0];
        } else {
            l->debug() << "JSON root is not array" << std::endl;
            el = root;
        }
        const Json::Value owners = el["owner"];
        for (unsigned int i = 0; i < owners.size(); ++i) {
            _file.owner = owners[i].asString();
        }
        _file.name = el.get("name", "").asString();
        _file.id = el.get("id", "").asString();
        _file.size = el.get("size", "").asLargestInt();
    } catch (Json::RuntimeError e) {
        l->error() << "Failed to parse JSON: " << e.what() << std::endl;
        return false;
    } catch (Json::LogicError e) {
        l->error() << "Failed to parse JSON: " << e.what() << std::endl;
        return false;
    }

    l->debug() << "Owner: " << _file.owner << std::endl;
    l->debug() << "Name: " << _file.name << std::endl;
    l->debug() << "ID: " << _file.id << std::endl;
    l->debug() << "Size: " << _file.size << std::endl;

    return true;
}

std::thread SubutaiLauncher::Downloader::download()
{
    Log::instance()->logger()->info() << "Downloader::download() start of sub, file name: " << _filename << std::endl;
    info();
    Log::instance()->logger()->info() << "Downloader::download() info received: " << _filename << std::endl;
    _progress = 0;
    _done = false;
    Log::instance()->logger()->info() << "Downloader::download() Downloading before thread"  << std::endl;
    return std::thread([=] { downloadImpl(); });
    Log::instance()->logger()->info() << "Downloader::download() after thread start"  << std::endl;
}

void SubutaiLauncher::Downloader::downloadImpl()
{
    auto l = Log::instance()->logger();
    l->info() << "DownloadImpl start " << std::endl;

    try {
        Poco::Net::HTTPStreamFactory::unregisterFactory();
        Poco::Net::HTTPSStreamFactory::unregisterFactory();
        l->info() << "DownloadImpl Downloading UNREGISTER " << _filename << std::endl;
    } catch (...) {
        l->error() << "DownloadImpl Downloading UNREGISTER failed " << _filename << std::endl;
    }

    try {
        Poco::Net::HTTPStreamFactory::registerFactory();
        l->info() << "DownloadImpl Poco::Net::HTTPStreamFactory::registerFactory(); " << std::endl;
    }
    catch (...){
        l->info() << "DownloadImpl Stream factory unknown error: "   << std::endl;
        //std::cerr << e.what();
    }
    Poco::Net::HTTPSStreamFactory::registerFactory();
    l->info() << "DownloadImpl  Poco::Net::HTTPSStreamFactory::registerFactory(); " << std::endl;
    Poco::URI uri(buildRequest("get", "name", _filename));
    l->info() << "DownloadImpl  Poco::URI uri " << buildRequest("get", "name", _filename)  << std::endl;

    std::string path;
    try {
        std::auto_ptr<std::istream> pStr(Poco::URIStreamOpener::defaultOpener().open(uri));
        path.append(_outputDir);
        path.append(PATH_DELIM);
        path.append(_filename.c_str());
        _rfile = path;
        l->info() << "DownloadImpl  path _file.name.c_str() " << path << std::endl;
        FileSystem fs(_outputDir);
        if (fs.isFileExists(_filename)) {
            l->info() << "DownloadImpl  _file.name " << _file.name << " already exists. Validating checksum" << std::endl;
            if (verifyDownload()) {
                l->info() << "DownloadImpl file " << _file.name << " is in actual state" << std::endl;
                _done = true;
                _progress = _file.size;
                return;
            } else {
                l->info() << "Removing outdated file" << std::endl;
                fs.removeFile(_file.name);
            }
        }
        l->debug() << "DownloadImpl Spawning downloader thread" << std::endl;

        //downloading
        std::ofstream out(path, std::fstream::app);
        Poco::StreamCopier::copyStream(*pStr.get(), out);
        //l->info() << "DownloadImpl Downloading " << _filename << std::endl;
    } catch (Poco::Net::HTTPException e) {
        l->error() << e.displayText() << std::endl;
    }


    _done = true;
    return;

    /*
       FileSystem fs(_outputDir);
       if (fs.isFileExists(_file.name)) {
       l->info() << "File " << _file.name << " is already exists. Validating checksum" << std::endl;
       if (verifyDownload()) {
       l->info() << "File " << _file.name << " is in actual state" << std::endl;
       _done = true;
       _progress = _file.size;
       return;
       }
       else {
       fs.removeFile(_file.name);
       }
       }
       l->debug() << "Spawning downloader thread" << std::endl;
       auto curl = curl_easy_init();
       curl_easy_setopt(curl, CURLOPT_URL, buildRequest("get", "name", _filename).c_str());
       curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
       curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleFile);
       auto result = curl_easy_perform(curl);
       if (result == 0) {
       l->info() << "Download completed" << std::endl;
       }
       else {
       l->error() << "Failed to download " << _filename << std::endl;
       }
       _done = true;
       */
}

size_t SubutaiLauncher::Downloader::handleFile(char *data, size_t size, size_t nmemb, void *p)
{
    return static_cast<Downloader*>(p)->handleFileImpl(data, size, nmemb);
}

size_t SubutaiLauncher::Downloader::handleFileImpl(char* data, size_t size, size_t nmemb)
{
    _content.clear();
    _content.append(data, size * nmemb);

    _progress += size * nmemb;

    std::string path(_outputDir);
    path.append(PATH_DELIM);
    path.append(_file.name.c_str());

    std::ofstream out(path, std::fstream::app);
    if (!out) {
        Log::instance()->logger()->error() << "Can't open " << path << " for writing" << std::endl;
        _done = true;
    }
    else {
        out << _content;
        out.close();
    }

    return size * nmemb;
}

long SubutaiLauncher::Downloader::currentProgress()
{
    return _progress;
}

bool SubutaiLauncher::Downloader::isDone()
{
    return _done;
}

double SubutaiLauncher::Downloader::getPercent()
{
    Poco::File f(_rfile);
    if (!f.exists()) return 0.0;
    return (double)(f.getSize() / _file.size * 100);
}

bool SubutaiLauncher::Downloader::verifyDownload()
{
    std::string path(_outputDir);
    path.append(PATH_DELIM);
    path.append(_file.name);

    Poco::MD5Engine md5;
    try {
    Poco::DigestOutputStream ostr(md5);
    Poco::FileInputStream fs(path);
    Poco::StreamCopier::copyStream(fs, ostr);
    ostr.close();
    fs.close();
    } catch (Poco::FileNotFoundException e) {
        Log::instance()->logger()->error() << "Couldn't verify file: Not found" << std::endl;
    } catch (std::exception e) {
        Log::instance()->logger()->error() << "Undefined exception" << std::endl;
    }

    std::string hash = Poco::DigestEngine::digestToHex(md5.digest());
    return (hash == _file.id);
}

SubutaiLauncher::SubutaiFile SubutaiLauncher::Downloader::info()
{
    if (_file.name == "") {
        retrieveFileInfo();
    }
    return _file;
}

void SubutaiLauncher::Downloader::setOutputDirectory(const std::string& dir)
{
    _outputDir = dir;
}

std::string SubutaiLauncher::Downloader::getOutputDirectory() const
{
    return _outputDir;
}

std::string SubutaiLauncher::Downloader::getFullPath() const
{
    std::string path(_outputDir);
    path.append(PATH_DELIM);
    path.append(_file.name);
    return path;
}

