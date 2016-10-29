#include "Hub.h"


const std::string SubutaiLauncher::Hub::URL = "https://hub.subut.ai";
const std::string SubutaiLauncher::Hub::REST = "/rest/v1";

SubutaiLauncher::Hub::Hub() : curl(curl_easy_init()) {
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
}

SubutaiLauncher::Hub::~Hub() {
	curl_easy_cleanup(curl);
}

void SubutaiLauncher::Hub::setLogin(std::string login) {
	_login = login;
}

void SubutaiLauncher::Hub::setPassword(std::string password) {
	_password = password;
}

bool SubutaiLauncher::Hub::auth() {
	std::map<std::string, std::string> query;
	query["email"] = _login;
	query["password"] = _password;
	auto response = performPostRequest("tray/login", query);
	long code = 0;
	std::printf("[HUB] Received during auth: %s\n", response.c_str());
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
	if (code == 200 && code != CURLE_ABORTED_BY_CALLBACK) {
		return true;
	}
	else {
		return false;
	}
	return false;
}

bool SubutaiLauncher::Hub::balance() {
	std::map<std::string, std::string> query;
	auto response = performGetRequest("tray/balance", query);
	long code = 0;
	std::printf("[HUB] Received during balance: %s\n", response.c_str());
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
	if (code == 200 && code != CURLE_ABORTED_BY_CALLBACK) {
		return true;
	}
	else {
		return false;
	}
	return false;
}

std::string SubutaiLauncher::Hub::performGetRequest(std::string endpoint, std::map<std::string, std::string> query) {
	return performRequest(false, endpoint, query);
}

std::string SubutaiLauncher::Hub::performPostRequest(std::string endpoint, std::map<std::string, std::string> query) {
	return performRequest(true, endpoint, query);
}

std::string SubutaiLauncher::Hub::performRequest(bool post, std::string endpoint, std::map<std::string, std::string> query) {
	_response.clear();
	char request[4096];
	std::sprintf(request, "%s%s/%s", URL.c_str(), REST.c_str(), endpoint.c_str());
	if (!query.empty()) {
		std::sprintf(request, "%s?", request);
		for (auto it = query.begin(); it != query.end(); it++) {
			std::sprintf(request, "%s%s=%s&", request, it->first.c_str(), it->second.c_str());
		}
	}

	std::printf("Performing request: %s\n", request);

	curl_easy_reset(curl);
	printCookies();
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_URL, request);
	if (post) {
		curl_easy_setopt(curl, CURLOPT_POST, 1);
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, handleHeaders);
	auto result = curl_easy_perform(curl);
	if (result == 0) {
		return _response;
	}
	else {
		return "";
	}
}

size_t SubutaiLauncher::Hub::handleResponse(char* data, size_t size, size_t nmemb, void *p) {
	return static_cast<Hub*>(p)->handleResponseImpl(data, size, nmemb);
}

size_t SubutaiLauncher::Hub::handleResponseImpl(char* data, size_t size, size_t nmemb) {
	_response.append(data, size * nmemb);
	return size * nmemb;
}

size_t SubutaiLauncher::Hub::handleHeaders(char* data, size_t size, size_t nitems, void *p) {
	return static_cast<Hub*>(p)->handleHeadersImpl(data, size, nitems);
}

size_t SubutaiLauncher::Hub::handleHeadersImpl(char* data, size_t size, size_t nitems) {
	if (size * nitems <= 2) {
		return nitems * size;
	}
	std::printf("Header: %s", data);

	/*
	   auto h = std::string(data);

	   auto pos = h.find_first_of(" ");
	   if (pos != std::npos) {
	   auto header = h.substr(pos);
	   if (header == "HTTP/1.1") {

	   }
	   }
	   */

	return nitems * size;
}

void SubutaiLauncher::Hub::printCookies() {
	CURLcode res;
	struct curl_slist *cookies;
	struct curl_slist *nc;
	int i;

	printf("Cookies, curl knows:\n");
	res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
	if (res != CURLE_OK) {
		fprintf(stderr, "Curl curl_easy_getinfo failed: %s\n",
			curl_easy_strerror(res));
		exit(1);
	}
	nc = cookies, i = 1;
	while (nc) {
		printf("[%d]: %s\n", i, nc->data);
		nc = nc->next;
		i++;
	}
	if (i == 1) {
		printf("(none)\n");
	}
	curl_slist_free_all(cookies);
}

