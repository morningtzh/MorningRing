//
// Created by MorningTZH on 2023/1/20.
//

#include <sys/param.h>
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


bool GetQuery(httpd_req_t *r, const std::string &key, std::string &value) {
    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    size_t buf_len = httpd_req_get_url_query_len(r) + 1;
    if (buf_len < 1) {
        return false;
    }

    auto buf = (char *)malloc(buf_len);
    if (httpd_req_get_url_query_str(r, buf, buf_len) != ESP_OK) {
        free(buf);
        return false;
    }

    ESP_LOGI(MODULE, "Found URL query => %s", buf);
    char param[32];
    /* Get value of expected key from query string */
    if (httpd_query_key_value(buf, key.c_str(), param, sizeof(param)) != ESP_OK) {
        free(buf);
        return false;
    }

    value = param;

    free(buf);

    return true;
}

std::list<std::pair<std::string, std::string>> GetQuery(httpd_req_t *r, std::list<std::string> keys) {
    /* Read URL query string length and allocate memory for length + 1,
 * extra byte for null termination */

    std::list<std::pair<std::string, std::string>> query;

    size_t buf_len = httpd_req_get_url_query_len(r) + 1;
    if (buf_len < 1) {
        return query;
    }

    auto buf = (char *)malloc(buf_len);
    if (httpd_req_get_url_query_str(r, buf, buf_len) != ESP_OK) {
        free(buf);
        return query;
    }

    ESP_LOGI(MODULE, "Found URL query => %s", buf);
    char value[32];

    for (auto &key: keys) {
        /* Get value of expected key from query string */
        if (httpd_query_key_value(buf, key.c_str(), value, sizeof(value)) == ESP_OK) {
            query.emplace_back(key, value);
        }
    }

    free(buf);

    return query;
}

bool GetData(httpd_req_t *r, std::string &data) {
    char buf[512];
    int ret, remaining = r->content_len;

    if (remaining > 0) {
        /* Read the data for the request */
        if ((ret = httpd_req_recv(r, buf,
                                  MIN(remaining, sizeof(buf)))) <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                /* Retry receiving if timeout occurred */
                return false;
            }
        }
    }

    data = buf;

    return true;
}
