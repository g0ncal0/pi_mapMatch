#ifndef REQUESTS_HPP
#define REQUESTS_HPP

#include <iostream>
#include <string>
#include <curl/curl.h>

/**
 * @brief Callback function to handle the data received from the cURL request
 * 
 * @param contents A pointer to the received data
 * @param size The size of each element to be written
 * @param nmemb The number of elements to be written
 * @param userp A pointer to the user data where the data is appended
 * @return The number of bytes written
 */
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

/**
 * @brief Function to make a HTTP request using cURL
 * 
 * @param url The URL for the HTTP request
 * @param response The server response
 * @return 0 if no errors
 */
int make_request(const char url[], std::string& response);

#endif
