#include "Downloader.h"


const std::string SubutaiLauncher::Downloader::URL = "https://cdn.subut.ai:8338";
const std::string SubutaiLauncher::Downloader::REST = "/kurjun/rest/raw";

SubutaiLauncher::Downloader::Downloader() : _content(""),
_done(false),
_outputDir(".")
{
	std::printf("Starting Downloader\n");
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
}

std::string SubutaiLauncher::Downloader::buildRequest(std::string path, std::string key, std::string value)
{
	char r[1024];
	if (!key.empty()) {
		std::sprintf(r, "%s%s/%s?%s=%s", URL.c_str(), REST.c_str(), path.c_str(), key.c_str(), value.c_str());
	}
	else {
		std::sprintf(r, "%s%s/%s", URL.c_str(), REST.c_str(), path.c_str());
	}
	std::printf("Requesting: %s\n", r);
	return std::string(r);
}

bool SubutaiLauncher::Downloader::retrieveFileInfo()
{
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

size_t SubutaiLauncher::Downloader::handleInfo(char *data, size_t size, size_t nmemb, void *p)
{
	return static_cast<Downloader*>(p)->handleInfoImpl(data, size, nmemb);
}

size_t SubutaiLauncher::Downloader::handleInfoImpl(char* data, size_t size, size_t nmemb)
{
	_content.clear();
	std::printf("Parsing file info: %s\n", data);
	_content.append(data, size * nmemb);
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
	std::printf("Size: %lu\n", _file.size);

	return size * nmemb;
}

std::thread SubutaiLauncher::Downloader::download()
{
	info();
	_progress = 0;
	std::printf("Starting download of a file: %s\n", _filename.c_str());
	_done = false;
	return std::thread([=] { downloadImpl(); });
}

void SubutaiLauncher::Downloader::downloadImpl()
{
	FileSystem fs(_outputDir);
	if (fs.isFileExists(_file.name)) {
		std::printf("File %s already exists. Validating checksum\n", _file.name.c_str());
		if (verifyDownload()) {
			std::printf("File %s already exists and it was not changed on remote host\n", _file.name.c_str());
			_done = true;
			_progress = _file.size;
			return;
		}
		else {
			fs.removeFile(_file.name);
		}
	}
	std::printf("Starting downloader thread\n");
	auto curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, buildRequest("get", "name", _filename).c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleFile);
	auto result = curl_easy_perform(curl);
	if (result == 0) {
		std::printf("Download completed without any errors\n");
	}
	else {
		std::printf("Download completed with errors\n");
	}
	_done = true;
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
		std::printf("Couldn't open file %s for writing\n", path.c_str());
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

int SubutaiLauncher::Downloader::getPercent()
{
	return (int)(_progress / (_file.size / 100));
}

bool SubutaiLauncher::Downloader::verifyDownload()
{
	std::string path(_outputDir);
	path.append(PATH_DELIM);
	path.append(_file.name);

	std::ifstream file(path.c_str());
	std::string buffer;
	file.seekg(0, std::ios::end);
	buffer.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	buffer.assign((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	auto sum = md5sum(buffer.c_str(), buffer.length());
	std::printf("File checksum: %s, remote checksum: %s\n", sum.c_str(), _file.id.c_str());
	if (sum == _file.id) {
		return true;
	}

	return false;
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

