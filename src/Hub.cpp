#include "Hub.h"

namespace SubutaiLauncher {

    const std::string Hub::URL = "https://hub.subut.ai";
    const std::string Hub::REST = "/rest/v1";

    Hub::Hub() {

    }

    Hub::~Hub() {

    }

    void Hub::setLogin(std::string login) {
        _login = login;
    }

    void Hub::setPassword(std::string password) {
        _password = password;
    }

    void Hub::Auth() {
        std::map<std::string, std::string> query;
        query["email"] = _login;
        query["password"] = _password;
        auto response = performPostRequest("tray/login", query);
        std::printf("Received after auth: %s\n", response.c_str());
    }

    std::string Hub::performPostRequest(std::string endpoint, std::map<std::string, std::string> query) {
        char request[4096];
        std::sprintf(request, "%s%s/%s", URL.c_str(), REST.c_str(), endpoint.c_str());
        char post[4096] = "";
        if (!query.empty()) {
            for (auto it = query.begin(); it != query.end(); it++) {
                std::sprintf(post, "%s%s=%s&", post, it->first.c_str(), it->second.c_str());
            }
        }

        std::printf("Performing request: %s, POST: %s\n", request, post);

        auto curl = curl_easy_init();
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_URL, request);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
        auto result = curl_easy_perform(curl);
        if (result == 0) {
            return _response;
        } else {
            return "";
        }
    }

    std::string Hub::performGetRequest(std::string endpoint, std::map<std::string, std::string> query) {
        char request[4096];
        std::sprintf(request, "%s%s/%s", URL.c_str(), REST.c_str(), endpoint.c_str());
        if (!query.empty()) {
            std::sprintf(request, "%s?", request);
            for (auto it = query.begin(); it != query.end(); it++) {
                std::sprintf(request, "%s%s=%s&", request, it->first.c_str(), it->second.c_str());
            }
        }

        std::printf("Performing request: %s\n", request);

        auto curl = curl_easy_init();
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_URL, request);
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handleResponse);
        auto result = curl_easy_perform(curl);
        if (result == 0) {
            return _response;
        } else {
            return "";
        }
    }

    size_t Hub::handleResponse(char* data, size_t size, size_t nmemb, void *p) {
        return static_cast<Hub*>(p)->handleResponseImpl(data, size, nmemb);
    } 

    size_t Hub::handleResponseImpl(char* data, size_t size, size_t nmemb) {
        _response.append(data, size * nmemb);
        return size * nmemb;
    }
};

