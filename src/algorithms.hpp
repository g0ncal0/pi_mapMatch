#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <cstring>
#include <list>
#include "requests.hpp"
#include "converter.hpp"
#include "parser.hpp"

/**
 * @brief Function to check if the response from OSRM API indicates that there is no match
 * 
 * @param res The response from the OSRM API
 * @return true if no match was found 
 */
bool no_match(const std::string& res);

/**
 * @brief Function to perform a map matching algorithm with the OSRM API
 * 
 * This functions takes points with coordinates and uses OSRM API to perform a map matching algorithm with the input points
 * It returns the route adjusted to the map in GeoJSON format
 * 
 * @param points A string with all the points in a format like "-8.598872,41.167089;-8.601544,41.167101;-8.602729,41.165494;-8.606549,41.166443" -> First longitude then latitude
 * @return A string with the contents of the route adjusted to the map in GeoJSON format ready to be used in tools like https://geojson.io/
 */
std::string map_match(const std::string& points);

/**
 * @brief Function to perform a routing algorithm with the OSRM API
 * 
 * This functions takes points with coordinates and uses OSRM API to perform a routing algorithm with the input points
 * It returns the route adjusted to the map in GeoJSON format
 * 
 * @param points A string with all the points in a format like "-8.598872,41.167089;-8.601544,41.167101;-8.602729,41.165494;-8.606549,41.166443" -> First longitude then latitude
 * @return A string with the contents of the route adjusted to the map in GeoJSON format ready to be used in tools like https://geojson.io/
 */
std::string route(const std::string& points);

/**
 * @brief Function to perform a map matching algorithm with the Valhalla API
 * 
 * This functions takes points with coordinates and uses Valhalla API to perform a map matching algorithm with the input points
 * It returns the route adjusted to the map in GeoJSON format
 * 
 * @param points A string with all the points in a format like "-8.598872,41.167089;-8.601544,41.167101;-8.602729,41.165494;-8.606549,41.166443" -> First longitude then latitude
 * @param radius The radius of the points to be matched in meters. Default is 0
 * @return A string with the contents of the route adjusted to the map in GeoJSON format ready to be used in tools like https://geojson.io/
 */
std::string map_match_valhalla(const std::string& points, int radius = 0);

/**
 * @brief Function to perform a routing algorithm with the Valhalla API
 * 
 * This functions takes points with coordinates and uses Valhalla API to perform a routing algorithm with the input points
 * It returns the route adjusted to the map in GeoJSON format
 * 
 * @param points A string with all the points in a format like "-8.598872,41.167089;-8.601544,41.167101;-8.602729,41.165494;-8.606549,41.166443" -> First longitude then latitude
 * @param excludePolygons A string with all the polygons to be excluded in a format like "[-8.598872,41.167089;-8.601544,41.167101;-8.602729,41.165494;-8.606549,41.166443],[-8.598872,41.167089;-8.601544,41.167101;-8.602729,41.165494;-8.606549,41.166443]"
 * @return A string with the contents of the route adjusted to the map in GeoJSON format ready to be used in tools like https://geojson.io/
 */
std::string route_valhalla(const std::string& points, const std::string& excludePolygons);

/**
 * @brief Function to get a specific bus route from Porto STCP network
 * 
 * @param routeID A string with the route ID
 * @param directionID A string with the direction ID (0 for outbound and 1 for inbound)
 * @param excludePolygons A string with all the polygons to be excluded in a format like "[-8.598872,41.167089;-8.601544,41.167101;-8.602729,41.165494;-8.606549,41.166443],[-8.598872,41.167089;-8.601544,41.167101;-8.602729,41.165494;-8.606549,41.166443]"
 * @return std::string A string with all excluded stops (collided with one or more excluded zones) if needed and then the contents of the route adjusted to the map in GeoJSON format ready to be used in tools like https://geojson.io/
 */
std::string bus_route(const std::string& routeID, const std::string& directionID, const std::string& excludePolygons, const std::string& changes);

/**
 * @brief A function to check if a point is inside a polygon. It uses the ray-casting algorithm.
 * 
 * @param lon longitude of the point
 * @param lat latitude of the point
 * @param polygon vector of pairs of doubles representing the polygon vertices
 * @return true if the point is inside the polygon
 */
bool pointIsInsidePolygon(double lon, double lat, const std::vector<std::pair<double, double>>& polygon);

/**
 * @brief remove all the stops of a route that are inside the exclude polygons
 * 
 * @param stops the stops of the route
 * @param excludePolygons the vector with all the exclude polygons
 * @return std::string with the information of the stops that were removed
 */
std::string remove_stops_in_excluded_zones(std::list<std::tuple<std::string, std::string, std::string, int>>& stops, const std::vector<std::vector<std::pair<double, double>>>& excludePolygons);

/**
 * @brief function to process one change made to the route that is putting a new (already existing) stop to the route
 * 
 * @param putInfo the information of the stop (stopID) and where to put it
 * @param stops the list with original stops of the route (and where the changes will be made)
 * @return std::string "OK" if everything went well
 */
std::string process_put_stop(const std::string& putInfo, std::list<std::tuple<std::string, std::string, std::string, int>>& stops);

/**
 * @brief function to process one change made to the route that is putting a new stop to the route (with the coords of it)
 * 
 * @param putInfo the information of the stop (coordinates) and where to put it
 * @param stops the list with original stops of the route (and where the changes will be made)
 * @return std::string "OK" if everything went well
 */
std::string process_put_coord(const std::string& putInfo, std::list<std::tuple<std::string, std::string, std::string, int>>& stops);

/**
 * @brief function to process one change made to the route that is deleting a stop from the route
 * 
 * @param deleteInfo the information of the stop to be removed (stopID)
 * @param stops the list with original stops of the route (and where the changes will be made)
 * @return std::string "OK" if everything went well
 */
std::string process_delete_stop(const std::string& deleteInfo, std::list<std::tuple<std::string, std::string, std::string, int>>& stops);

/**
 * @brief Function to process all the changes made to one route
 * 
 * @param changes a formatted string with all the changes made to the route
 * @param stops the list with original stops of the route (and where the changes will be made)
 * @return std::string "OK" if everything went well
 */
std::string process_changes(const std::string& changes, std::list<std::tuple<std::string, std::string, std::string, int>>& stops);

#endif
