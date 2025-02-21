#ifndef REQUESTS_HPP
#define REQUESTS_HPP

#include <iostream>
#include <string>
#include <curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

int make_request(const char url[], std::string& readBuffer);

#endif
