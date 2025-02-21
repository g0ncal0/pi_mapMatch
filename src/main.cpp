#include <iostream>
#include <string>
#include <curl/curl.h>
#include "crow.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int make_request(const char url[], std::string& readBuffer) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        return 0;
    }
    else {
        std::cout << "Invalid URL" << std::endl;

        return 1;
    }
}

std::string map_match(const std::string& points) {
    const char* points_char = points.c_str();
    char url[150 + points.length()] = "http://localhost:5000/match/v1/driving/";
    strcat(url, points_char);
    strcat(url, "?steps=true&geometries=geojson&overview=full&annotations=false");

    std::string res;

    if (make_request(url, res) == 0) {
        return res;
    }
    else {
        return "Error";
    }
}

void setup_routes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    CROW_ROUTE(app, "/map-match/<string>")([](const std::string& points){
        return map_match(points);
    });
}

int main()
{
    crow::SimpleApp app;

    setup_routes(app);

    app.port(18080).multithreaded().run();

    return 0;
}
