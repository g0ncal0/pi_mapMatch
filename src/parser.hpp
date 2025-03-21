#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>
#include "rapidcsv.h"

std::vector<std::tuple<std::string, std::string, std::string, int>> get_stops_from_trip(const std::string& route_id, const std::string direction_id);

std::string get_coordinates_string_from_stops(const std::vector<std::tuple<std::string, std::string, std::string, int>>& stops);
