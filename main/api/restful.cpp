//
// Created by MorningTZH on 2023/1/20.
//

#include "restful.hpp"

static const char *MODULE = "Restful";

Restful *Restful::instance = nullptr;

void Restful::RegisterUri(const std::string &name, const char *uri, http_method method, RestfulHandler handle) {

    httpd_uri_t uriInfo{
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

    ESP_LOGI(MODULE, "Restful API[%s:%s] Registered.", name.c_str(), uri);
}

Restful *Restful::api() {
    if (!instance) {
        instance = new Restful;
    }

    return instance;

}

Restful::Restful() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI(MODULE, "Starting HTTP Server");
    httpd_start(&server, &config);
}
