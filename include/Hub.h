#ifndef __HUB_H__
#define __HUB_H__

#include <string>
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
            void Auth();
            static size_t handleResponse(char* data, size_t size, size_t nmemb, void *p);
            size_t handleResponseImpl(char* data, size_t size, size_t nmemb);
        private:
            std::string performPostRequest(std::string endpoint, std::map<std::string, std::string> query);
            std::string performGetRequest(std::string endpoint, std::map<std::string, std::string> query);
            std::string _login;
            std::string _password;
            std::string _response;
    };

};

#endif
