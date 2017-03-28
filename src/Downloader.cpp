#include "Downloader.h"
#include "Session.h"

const std::string SubutaiLauncher::Downloader::URL = "https://devcdn.subut.ai:8338";
const std::string SubutaiLauncher::Downloader::REST = "/kurjun/rest/raw";
const std::string SubutaiLauncher::Downloader::HOST = "devcdn.subut.ai";

SubutaiLauncher::Downloader::Downloader() : 
    _content(""),
    _done(false),
    _outputDir(".")
{
    //Log::instance()->logger()->debug() << "Starting Downloader instance" << std::endl;
    //Session::instance()->logger().debug("Starting Downloader instance");
    Poco::Logger::get("subutai").debug("Starting Downloader instance");
    _logger = &Poco::Logger::get("subutai");
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
    _logger->debug("Downloader::setFilename %s", _filename);
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
    _logger->debug("Requesting %s", std::string(r));
    return std::string(r);
}

bool SubutaiLauncher::Downloader::retrieveFileInfo()
{
    Poco::Net::HTTPSClientSession s(HOST, PORT);
    std::string path(REST);
    path.append("/info");
    _logger->debug("Requesting file info for %s", path);
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path);
    Poco::Net::HTMLForm form;
    form.add("name", _filename);
    form.prepareSubmit(request);
    s.sendRequest(request);

    Poco::Net::HTTPResponse response;
    std::istream& rs = s.receiveResponse(response);
    std::string output;
    Poco::StreamCopier::copyToString(rs, output);

    _logger->debug("Received file info for %s: %s", path, output);

    //l->debug() << "Before JSon(change to Poco!): responce: " << output << std::endl;

    // TODO: Replace JSON lib with Poco
    Json::Value root;

    //l->debug() << "JSon(change to Poco!): root copied: " << std::endl;
    try { 
        std::istringstream str(output);
        str >> root;

        Json::Value el;
        if (root.isArray()) {
            el = root[0];
        } else {
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
        _logger->error("Failed to parse JSON: %s", e.what());
        return false;
    } catch (Json::LogicError e) {
        _logger->error("Failed to parse JSON: %s", e.what());
        return false;
    }

    _logger->debug("Owner: %s", _file.owner);
    _logger->debug("Name: %s", _file.name);
    _logger->debug("ID: %s", _file.id);
    _logger->debug("Size: %lu", _file.size);

    return true;
}

std::thread SubutaiLauncher::Downloader::download()
{
    info();
    _progress = 0;
    _done = false;
    return std::thread([=] { downloadImpl(); });
}

void SubutaiLauncher::Downloader::downloadImpl()
{
    _logger->debug("Starting Download thread");

    try {
        _logger->debug("Unregistering HTTP(S) Stream factory");
        Poco::Net::HTTPStreamFactory::unregisterFactory();
        Poco::Net::HTTPSStreamFactory::unregisterFactory();
    } catch (...) {
        _logger->error("Failed to unregister HTTP(S) Stream factory");
    }

    try {
        Poco::Net::HTTPStreamFactory::registerFactory();
        _logger->debug("Registering HTTP Stream Factory");
    }
    catch (...){
        _logger->error("Failed to register HTTP Stream factory");
    }

    Poco::Net::HTTPSStreamFactory::registerFactory();
    Poco::URI uri(buildRequest("get", "name", _filename));

    std::string path;
    try {
        std::auto_ptr<std::istream> pStr(Poco::URIStreamOpener::defaultOpener().open(uri));
        path.append(_outputDir);
        path.append(PATH_DELIM);
        path.append(_filename.c_str());
        _rfile = path;
        Poco::File f(path);

        if (f.exists()) {
            _logger->information("File %s already exists", _file.name);
            if (verifyDownload()) {
                _logger->information("File %s is up to date", _file.name);
                _done = true;
                _progress = _file.size;
                return;
            } else {
                _logger->information("File %s is outdated. Removing it", _file.name);
                f.remove();
            }
        }

        _logger->information("Starting file downloads [%s]", _file.name);
        std::ofstream out(path, std::fstream::app);
        Poco::StreamCopier::copyStream(*pStr.get(), out);
    } catch (Poco::Net::HTTPException e) {
        _logger->error("File download failed: %s", e.displayText());
    }

    _done = true;
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
    _logger->information("Verifying %s MD5 checksum", _rfile);
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
        _logger->error("File verification: File not found");
    } catch (std::exception e) {
        _logger->error("File verification: Unknown exception");
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

