#include <iostream>
#include <unordered_map>
#include <list>
#include <set>
#include "rapidcsv.h"

/**
 * @brief Get the stops from a specific trip. This function reads the GTFS data files to obatain this data
 * 
 * @param route_id A string with the route ID
 * @param direction_id A string with the direction ("0" or "1")
 * @return std::list<std::tuple<std::string, std::string, std::string, int>> with all stops and respective lat, lon and stop_sequence
 */
std::list<std::tuple<std::string, std::string, std::string, int>> get_stops_from_trip(const std::string& route_id, const std::string direction_id);

/**
 * @brief Get a string with the coordinates of stops, in the API format, from the list of stops. This function also reads the GTFS data files to obtain this data
 * 
 * @param stops the list with all stops and respective information
 * @return std::string with the stops coordinates in the API format
 */
std::string get_coordinates_string_from_stops(const std::list<std::tuple<std::string, std::string, std::string, int>>& stops);

/**
 * @brief Get the coords from one specific stop
 * 
 * @param stopID the ID of the stop
 * @param lat where to store the latitude of the stop
 * @param lon where to store the longitude of the stop
 */
void get_coords_from_stop(const std::string stopID, std::string& lat, std::string& lon);
