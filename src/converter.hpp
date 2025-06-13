#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <utility>
#include <regex>
#include "constants.hpp"

/**
 * @brief Function to get the coordinates of a complete geo JSON response
 * 
 * @param completeGeoJSON A string with the contents of the complete GeoJSON format
 * @param coordinates A string where the coordinates will be saved
 * @return "Ok" if no errors. Else return a string with information about an error
 */
std::string get_coordinates_from_GeoJson(const std::string& completeGeoJSON, std::string& coordinates);

/**
 * @brief Function used to convert coordinates from the GeoJSON format to the one used in this API 
 * 
 * @param geoCoord The coordinates in the GeoJSON format
 * @param reqCoord The request coordinates. Where the result is going to be saved
 */
void convert_GeoJSON_coordinates_to_request_coordinates(const std::string& geoCoord, std::string& reqCoord);

/**
 * @brief Function used to convert a string with coordinates to a string with the coordinates in the GeoJSON format
 * 
 * @param coordinates The string with the coordinates to be converted
 * @param type Can be "LineString" or "Polygon", this is important because the format of the coordinates in GeoJSON is different for each type
 * @param coordsFromGeoJSON Variable to know if the coordinates are already in a GeoJSON not formatted format or if it is in the format that this API uses in inputs because the representation is different
 * @return std::string with the coordinates in the GeoJSON format
 */
std::string convert_coordinates_to_formated_GeoJSON_coordinates(const std::string& coordinates, const std::string& type, bool coordsFromGeoJSON);

/**
 * @brief Function to convert a string with coordinates in a GeoJSON formatted feature
 * 
 * @param coordinates The string with the coordinates to be used in the feature
 * @param type of the feature, can be "LineString" or "Polygon"
 * @param coordsFromGeoJSON Variable to be passed to convert_coordinates_to_formated_GeoJSON_coordinates function
 * @return std::string with the feature in the GeoJSON format
 */
std::string convert_coordinates_to_GeoJSON_feature(const std::string& coordinates, const std::string& type, bool coordsFromGeoJSON);

/**
 * @brief Function to convert a string from a complete GeoJSON format to a simple GeoJSON format
 * 
 * OSRM returns a lot of data, but to draw the shape of a route and to see it in tools like https://geojson.io/ we only need the points coordinates in a simpler format.
 * Converts to a FeatureCollection format with a LineString feature representing one route and Polygon features to represent the exclude polygons
 * 
 * @param completeGeoJSON A string with the contents of the complete GeoJSON format
 * @param excludePolygons A string with the contents of all exclude polygons used to calculate the route
 * @return A string with the contents of the simple GeoJSON format ready to be used in tools like https://geojson.io/
 */
std::string convert_completeGeoJSON_to_simpleGeoJSON(const std::string& completeGeoJSON, const std::string& excludePolygons);

/**
 * @brief Function to convert coordinates from the API input format to the Valhalla format
 * 
 * @param inputCoord A string with the input coordinates
 * @param valhallaCoord The string where the output coordinates wwill be saved
 * @param radius The radius of the points to be matched in meters. Default is 0
 */
void convert_input_coordinates_to_valhalla_coordinates(const std::string& inputCoord, std::string& valhallaCoord, int radius = 0);

/**
 * @brief Function to get the json of exclude polygons in the GeoJSON format to be used in the valhalla API
 * 
 * @param excludePolygons The string with the exclude polygons in the input API format
 * @return std::string with the json to be used for exclude polygons in the Valhalla API
 */
std::string get_exclude_polygons_geoJSON(const std::string& excludePolygons);

/**
 * @brief Get the exclude polygons geoJSON features to be used in the final output in the GeoJson format
 * 
 * @param excludePolygons The string with the exclude polygons in the input API format
 * @return std::string with all exclude polygons features
 */
std::string get_exclude_polygons_geoJSON_features(const std::string& excludePolygons);

/**
 * @brief Get a list with all exclude polygons. This is used to get ease the use of each polygon to check for collisions
 * 
 * @param excludePolygons The string with the exclude polygons in the input API format
 * @return std::vector<std::vector<std::pair<double, double>>> with all exclude polygons
 */
std::vector<std::vector<std::pair<double, double>>> get_exclude_polygons_list(const std::string& excludePolygons);

/**
 * @brief Aux function for get_exclude_polygons_list. This function only gets one exclude polygon
 * 
 * @param excludePolygon The string with the exclude polygon in the input API format
 * @return std::vector<std::pair<double, double>> with one exclude polygon
 */
std::vector<std::pair<double, double>> get_exclude_polygon_list(const std::string& excludePolygon);

void extract_json_field(const std::string& json_str, const std::string& key, std::string& value);

bool check_points_format(const std::string& points);

#endif 
