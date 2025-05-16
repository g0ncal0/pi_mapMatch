#include "parser.hpp"

rapidcsv::Document trips("../gtfs_data/trips.txt", rapidcsv::LabelParams(-1, -1));
rapidcsv::Document stop_times("../gtfs_data/stop_times.txt", rapidcsv::LabelParams(-1, -1));
rapidcsv::Document stops("../gtfs_data/stops.txt", rapidcsv::LabelParams(-1, -1));

std::list<std::tuple<std::string, std::string, std::string, int>> get_stops_from_trip(const std::string& route_id, const std::string direction_id) {
    // We need to find the trip with the most stops for the given route
    
    std::vector<std::string> trip_ids;
    
    for (size_t i = 0; i < trips.GetRowCount(); i++) {
        if (trips.GetCell<std::string>(0, i) == route_id && 
            trips.GetCell<std::string>(1, i) == direction_id) {
            trip_ids.push_back(trips.GetCell<std::string>(3, i)); // trip_id
        }
    }

    std::list<std::tuple<std::string, std::string, std::string, int>> stopsInfo;

    if (trip_ids.empty()) {
        std::cout << "Invalid Route or Direction" << std::endl;
        return stopsInfo;
    }

    std::string best_trip_id;
    size_t max_stops = 0;
    int itStopTimes = 0;
    
    for (const auto& current_trip_id : trip_ids) {
        size_t current_stop_count = 0;
        
        for (size_t i = itStopTimes; i < stop_times.GetRowCount(); i++) {
            if (stop_times.GetCell<std::string>(0, i) == current_trip_id) {
                current_stop_count++;
            } else if (current_stop_count > 0) {
                itStopTimes = i;
                break;
            }
        }
        
        if (current_stop_count > max_stops) {
            max_stops = current_stop_count;
            best_trip_id = current_trip_id;
        }
    }

    for (size_t i = 0; i < stop_times.GetRowCount(); i++) {
        if (stop_times.GetCell<std::string>(0, i) == best_trip_id) {
            std::string stop_id = stop_times.GetCell<std::string>(3, i); // stop_id
            int stop_sequence = stop_times.GetCell<int>(4, i); // stop_sequence

            for (size_t j = 0; j < stops.GetRowCount(); j++) {
                if (stops.GetCell<std::string>(0, j) == stop_id) {
                    std::string lat = stops.GetCell<std::string>(3, j); // stop_lat
                    std::string lon = stops.GetCell<std::string>(4, j); // stop_lon
                    stopsInfo.push_back({stop_id, lat, lon, stop_sequence});
                    break;
                }
            }
        } else if (!stopsInfo.empty()) {
            break;
        }
    }

    return stopsInfo;
}

std::string get_coordinates_string_from_stops(const std::list<std::tuple<std::string, std::string, std::string, int>>& stops) {
    std::ostringstream oss;

    bool first = true;

    for (const auto& stop : stops) {
        if (!first) oss << ";";
        else first = false;

        oss << std::get<2>(stop) << "," << std::get<1>(stop);
    }

    return oss.str();
}

void get_coords_from_stop(const std::string stopID, std::string& lat, std::string& lon) {
    for (size_t i = 0; i < stops.GetRowCount(); i++) {
        if (stops.GetCell<std::string>(0, i) == stopID) {  // stop_id
            lat = stops.GetCell<std::string>(3, i); // stop_lat
            lon = stops.GetCell<std::string>(4, i); // stop_lon
            break;
        }
    }
}
