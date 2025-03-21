#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <cstring>
#include <list>
#include "requests.hpp"
#include "converter.hpp"
#include "parser.hpp"

bool no_match(const std::string& res);

/**
 * @brief Function to perform a map matching algorithm
 * 
 * This functions takes points with coordinates and uses OSRM API to perform a map matching algorithm with the input points
 * It returns the route adjusted to the map in GeoJSON format
 * 
 * @param points A string with all the points in a format like "-8.598872,41.167089;-8.601544,41.167101;-8.602729,41.165494;-8.606549,41.166443" -> First longitude then latitude
 * @return A string with the contents of the route adjusted to the map in GeoJSON format ready to be used in tools like https://geojson.io/
 */
std::string map_match(const std::string& points);

std::string route(const std::string& points);

std::string map_match_valhalla(const std::string& points);

std::string route_valhalla(const std::string& points, const std::string& excludePolygons);

std::string bus_route(const std::string& routeID, const std::string& directionID, const std::string& excludePolygons);

std::string remove_stops_in_excluded_zones(std::list<std::tuple<std::string, std::string, std::string, int>>& stops, const std::vector<std::vector<std::pair<double, double>>>& excludePolygons);

#endif
