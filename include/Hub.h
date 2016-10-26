#ifndef __HUB_H__
#define __HUB_H__

#include <string>
#include <cstring>
#include <map>
#include <curl/curl.h>
#include <json/json.h>

namespace SubutaiLauncher {

    class Hub {
        public:
            static const std::string URL;
            static const std::string REST;
            Hub();
            ~Hub();
            void setLogin(std::string login);
            void setPassword(std::string password);
            bool auth();
            bool balance();
            static size_t handleResponse(char* data, size_t size, size_t nmemb, void *p);
            static size_t handleHeaders(char* data, size_t size, size_t nitems, void *p);
            size_t handleResponseImpl(char* data, size_t size, size_t nmemb);
            size_t handleHeadersImpl(char* data, size_t size, size_t nitems);
        private:
            std::string performRequest(bool post, std::string endpoint, std::map<std::string, std::string> query);
            void printCookies();
            std::string performPostRequest(std::string endpoint, std::map<std::string, std::string> query);
            std::string performGetRequest(std::string endpoint, std::map<std::string, std::string> query);
            std::string _login;
            std::string _password;
            std::string _response;
            CURL* curl;
    };

};

#endif
