//
// Created by MorningTZH on 2023/1/20.
//

#ifndef MORNINGRING_RESTFUL_HPP
#define MORNINGRING_RESTFUL_HPP

#include <map>
#include <string>
#include <esp_log.h>
#include "esp_http_server.h"

static const char* MODULE = "Restful";

typedef esp_err_t (*RestfulHandler)(httpd_req_t *r);

struct RestfulContext {

};

class Restful {
private:
    RestfulContext ctx;
    std::map<std::string, httpd_uri_t> uris;
    httpd_handle_t server = nullptr;

    static Restful *instance;

    Restful() {
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();
        config.uri_match_fn = httpd_uri_match_wildcard;

        ESP_LOGI(MODULE, "Starting HTTP Server");
        httpd_start(&server, &config);
    }
public:

    static Restful *api() {
        if (!instance) {
            instance = new Restful;
        }

        return instance;
    }


    void RegisterUri(const std::string &name, const char *uri, http_method method, RestfulHandler handle) {
        httpd_uri_t uriInfo {
                uri,
                method,
                handle,
                &ctx
        };

        uris[name] = uriInfo;

        auto err = httpd_register_uri_handler(server, &uriInfo);
        if (err == ESP_ERR_HTTPD_HANDLERS_FULL) {
            ESP_LOGE(MODULE, "Restful Register API is full");
        }
    }
};

class RestfulRegister {
public:
    RestfulRegister(const std::string &name, const char *uri, http_method method, RestfulHandler handle) {
        Restful::api()->RegisterUri(name, uri, method, handle);
    }
};

#define RESTFUL_REGISTER(name, uri, method, handle)  \
RestfulRegister RestfulRegister##__FILE__##__LINE__##name(#name, uri, method, handle);

#endif //MORNINGRING_RESTFUL_HPP
