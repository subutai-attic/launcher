#include "Downloader.h"
#include "Session.h"

#ifdef BUILD_SCHEME_DEV
const std::string SubutaiLauncher::Downloader::URL = "https://devcdn.subut.ai:8338";
const std::string SubutaiLauncher::Downloader::REST = "/kurjun/rest/raw";
const std::string SubutaiLauncher::Downloader::TREST = "/kurjun/rest/template";
const std::string SubutaiLauncher::Downloader::HOST = "devcdn.subut.ai";
#elif BUILD_SCHEME_MASTER
const std::string SubutaiLauncher::Downloader::URL = "https://mastercdn.subut.ai:8338";
const std::string SubutaiLauncher::Downloader::REST = "/kurjun/rest/raw";
const std::string SubutaiLauncher::Downloader::TREST = "/kurjun/rest/template";
const std::string SubutaiLauncher::Downloader::HOST = "mastercdn.subut.ai";
#elif BUILD_SCHEME_PRODUCTION
const std::string SubutaiLauncher::Downloader::URL = "https://cdn.subut.ai:8338";
const std::string SubutaiLauncher::Downloader::REST = "/kurjun/rest/raw";
const std::string SubutaiLauncher::Downloader::TREST = "/kurjun/rest/template";
const std::string SubutaiLauncher::Downloader::HOST = "cdn.subut.ai";
#else
#error Build scheme was not specified
#endif

SubutaiLauncher::Downloader::Downloader() : 
    _content(""),
    _done(true),
    _running(false),
    _outputDir(".")
{
    _logger = &Poco::Logger::get("subutai");
    _logger->debug("Starting Downloader instance");
}

SubutaiLauncher::Downloader::~Downloader()
{

}

void SubutaiLauncher::Downloader::reset()
{
    _logger->debug("Resetting downloader");
    _file.name = "";
    _file.owner = "";
    _file.id = "";
    _file.size = 0;
    _content = "";
    _filename = "";
    _progress = 0;
    _done = false;
    _running = false;
}

void SubutaiLauncher::Downloader::setFilename(const std::string& filename)
{
    _filename = filename;
    _logger->trace("Downloader::setFilename %s", _filename);
}

std::string SubutaiLauncher::Downloader::buildRequest(std::string path, std::string key, std::string value)
{
    char r[1024];
    if (!key.empty()) 
    {
#if LAUNCHER_WINDOWS
        sprintf_s(r, sizeof(r), "%s%s/%s?%s=%s", URL.c_str(), REST.c_str(), path.c_str(), key.c_str(), value.c_str());
#else
        std::sprintf(r, "%s%s/%s?%s=%s", URL.c_str(), REST.c_str(), path.c_str(), key.c_str(), value.c_str());
#endif
    }
    else 
    {
#if LAUNCHER_WINDOWS
        sprintf_s(r, sizeof(r), "%s%s/%s", URL.c_str(), REST.c_str(), path.c_str());
#else
        std::sprintf(r, "%s%s/%s", URL.c_str(), REST.c_str(), path.c_str());
#endif
    }
    _logger->debug("Requesting %s", std::string(r));
    return std::string(r);
}

bool SubutaiLauncher::Downloader::retrieveFileInfo(bool tmpl)
{
    std::string output;
    std::string path;
    try 
    {
        Poco::Net::HTTPSClientSession s(HOST, PORT);

		if (tmpl) 
		{
			path.append(TREST);
		}
		else
		{
			path.append(REST);
		}
        
        path.append("/info");
        _logger->debug("Requesting file info for %s", path);
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path);
        Poco::Net::HTMLForm form;
        form.add("name", _filename);
        form.prepareSubmit(request);
        s.sendRequest(request);

        Poco::Net::HTTPResponse response;
        std::istream& rs = s.receiveResponse(response);
        if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_NOT_FOUND) 
        {
            _logger->error("Requested file not found: %s", _filename);
            return false;
        }
        Poco::StreamCopier::copyToString(rs, output);
        _logger->debug("Received file info for %s: %s", path, output);
    } 
    catch (Poco::TimeoutException& e)
    {
        _logger->fatal("Request timeout: %s", e.displayText());
        std::string err;
        Poco::format(err, "Request timeout: %s [%s]", path, e.displayText());
        throw SubutaiException(err);
    }
    catch (Poco::Net::SSLException& e) 
    {
        _logger->fatal("Failed to retrieve file info: %s", e.displayText());
        return false;
    }


    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result;

    try 
    {
        result = parser.parse(output);
    } 
    catch (Poco::JSON::JSONException& e) 
    {
        _logger->error("Failed to parse JSON while getting information about remote file: %s", _filename);
        _logger->debug("Failing JSON: %s", output);
        return false;
    } 
    catch (Poco::SyntaxException& e) 
    {
      _logger->error("Failed to parse JSON while getting information about remote file: %s", _filename);
      _logger->debug("Failing JSON: %s", output);
      return false;
    }

    if (result.isEmpty()) 
    {
        _logger->error("File info is empty after parsing JSON");
        return false;
    } 

    Poco::JSON::Array::Ptr arr = result.extract<Poco::JSON::Array::Ptr>();
    if (arr->size() == 0) 
    {
        _logger->error("JSON Array is empty");
        return false;
    }

    // Get the first element
    Poco::JSON::Object::Ptr obj = arr->getObject(0);
    _file.id = obj->get("id").toString();
    _file.name = obj->get("filename").toString();
    _file.size = obj->get("size").extract<int>();
    _file.owner = obj->get("owner").extract<Poco::JSON::Array::Ptr>()
        ->get(0).extract<std::string>();

    _logger->debug("Owner: %s", _file.owner);
    _logger->debug("Name: %s", _file.name);
    _logger->debug("ID: %s", _file.id);
    _logger->debug("Size: %ld", _file.size);

    return true;
}

bool SubutaiLauncher::Downloader::retrieveTemplateInfo()
{
	return retrieveFileInfo(true);
}

std::thread SubutaiLauncher::Downloader::download()
{
    info();
    _progress = 0;
    _done = false;
    _running = true;
    return std::thread([=] { downloadImpl(); });
}

void SubutaiLauncher::Downloader::downloadImpl()
{
    _logger->debug("Starting Download thread");

    try 
    {
        _logger->debug("Unregistering HTTP(S) Stream factory");
        Poco::Net::HTTPStreamFactory::unregisterFactory();
        Poco::Net::HTTPSStreamFactory::unregisterFactory();
    } 
    catch (...) 
    {
        _logger->error("Failed to unregister HTTP(S) Stream factory");
    }

    try 
    {
        Poco::Net::HTTPStreamFactory::registerFactory();
        _logger->debug("Registering HTTP Stream Factory");
    }
    catch (...)
    {
        _logger->error("Failed to register HTTP Stream factory");
    }

    Poco::Net::HTTPSStreamFactory::registerFactory();
    Poco::URI uri(buildRequest("get", "name", _filename));

    std::string path;
    try 
	{
        std::unique_ptr<std::istream> pStr(Poco::URIStreamOpener::defaultOpener().open(uri));
        path.append(_outputDir);
        path.append(PATH_DELIM);
        path.append(_filename.c_str());
        _rfile = path;
        Poco::File f(path);

        if (f.exists()) 
        {
            _logger->information("File %s already exists", _file.name);
            if (verifyDownload()) 
            {
                _logger->information("File %s is up to date", _file.name);
                _done = true;
                _running = false;
                _progress = _file.size;
                return;
            } 
            else 
            {
                _logger->information("File %s is outdated. Removing it", _file.name);
                f.remove();
            }
        }

        _logger->information("Starting file downloads [%s]", _file.name);
        std::ofstream out(path, std::fstream::app | std::fstream::out | std::fstream::binary);
        Poco::StreamCopier::copyStream(*pStr.get(), out);
    } 
    catch (Poco::Net::HTTPException e) 
    {
        _logger->error("File download failed: %s", e.displayText());
    }

    _done = true;
    _running = false;
}

long SubutaiLauncher::Downloader::currentProgress()
{
    return _progress;
}

bool SubutaiLauncher::Downloader::isDone()
{
    return _done;
}

bool SubutaiLauncher::Downloader::isRunning()
{
    return _running;
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
    try 
	{
        Poco::DigestOutputStream ostr(md5);
        Poco::FileInputStream fs(path);
        Poco::StreamCopier::copyStream(fs, ostr);
        ostr.close();
        fs.close();
    } 
	catch (Poco::FileNotFoundException e) 
	{
        _logger->error("File verification: File not found");
    } 
	catch (std::exception e) 
	{
        _logger->error("File verification: Unknown exception");
    }

    std::string hash = Poco::DigestEngine::digestToHex(md5.digest());
    return (hash == _file.id);
}

SubutaiLauncher::SubutaiFile SubutaiLauncher::Downloader::info()
{
    if (_file.name == "") 
	{
        retrieveFileInfo();
    }
    return _file;
}

void SubutaiLauncher::Downloader::setOutputDirectory(const std::string& dir)
{
	_logger->debug("Setting Download output directory to %s", dir);
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

