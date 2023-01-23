//
// Created by MorningTZH on 2023/1/20.
//

#ifndef MORNINGRING_RESTFUL_HPP
#define MORNINGRING_RESTFUL_HPP

#include <map>
#include <string>
#include <esp_log.h>
#include "esp_http_server.h"

typedef esp_err_t (*RestfulHandler)(httpd_req_t *r);

struct RestfulContext {

};

class Restful {
private:
    RestfulContext ctx;
    std::map<std::string, httpd_uri_t> uris;
    httpd_handle_t server = nullptr;

    static Restful *instance;

    Restful();

public:

    static Restful *api();

    void RegisterUri(const std::string &name, const char *uri, http_method method, RestfulHandler handle);
};

//void RegisterApi(const std::string &name, const char *uri, http_method method, RestfulHandler handle) {
//    Restful::api()->RegisterUri(name, uri, method, handle);
//}

class RestfulRegister {
public:
    RestfulRegister(const std::string &name, const char *uri, http_method method, RestfulHandler handle) {
        Restful::api()->RegisterUri(name, uri, method, handle);
    }
};

#define RESTFUL_REGISTER(name, uri, method, handle)  \
RestfulRegister RestfulRegister##__FILE__##__LINE__##name(#name, uri, method, handle);

#endif //MORNINGRING_RESTFUL_HPP
