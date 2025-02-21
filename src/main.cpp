#include <iostream>
#include <string>
#include <curl/curl.h>
#include "crow.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void setup_routes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });
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

int main()
{
    std::string res;

    if (make_request("http://localhost:5000/match/v1/driving/-8.598872,41.167089;-8.601544,41.167101;-8.602729,41.165494;-8.606549,41.166443?steps=true&geometries=geojson&overview=full&annotations=false", res) == 0) {
        std::cout << res << std::endl;
    }

    crow::SimpleApp app;

    setup_routes(app);

    app.port(18080).multithreaded().run();

    return 0;
}
