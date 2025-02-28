#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <string>
#include <string_view>
#include "constants.hpp"

std::string get_coordinates_from_GeoJson(const std::string& completeGeoJSON, std::string& coordinates);

void convert_GeoJSON_coordinates_to_request_coordinates(const std::string& geoCoord, std::string& reqCoord);

/**
 * @brief Function to convert a string from a complete GeoJSON format to a simple GeoJSON format
 * 
 * OSRM returns a lot of data, but to draw the shape of a route and to see it in tools like https://geojson.io/ we only need the points coordinates in a simpler format.
 * Converts to a FeatureCollection format with a LineString feature representing one route
 * 
 * @param completeGeoJSON A string with the contents of the complete GeoJSON format
 * @return A string with the contents of the simple GeoJSON format ready to be used in tools like https://geojson.io/
 */
std::string convert_completeGeoJSON_to_simpleGeoJSON(const std::string& completeGeoJSON);

#endif 
