#include "parser.hpp"

std::vector<std::tuple<std::string, std::string, std::string, int>> get_stops_from_trip(const std::string& route_id, const std::string direction_id) {
    rapidcsv::Document trips("../gtfs_data/trips.txt", rapidcsv::LabelParams(-1, -1));
    rapidcsv::Document stop_times("../gtfs_data/stop_times.txt", rapidcsv::LabelParams(-1, -1));
    rapidcsv::Document stops("../gtfs_data/stops.txt", rapidcsv::LabelParams(-1, -1));

    std::string trip_id;
    for (size_t i = 0; i < trips.GetRowCount(); i++) {
        if (trips.GetCell<std::string>(0, i) == route_id && trips.GetCell<std::string>(1, i) == direction_id) {  // route_id and direction
            trip_id = trips.GetCell<std::string>(3, i); // trip_id
            break;
        }
    }

    std::vector<std::tuple<std::string, std::string, std::string, int>> stopsResults;

    if (trip_id.empty()) {
        std::cout << "Invalid Route or Direction" << std::endl;
        return stopsResults;
    }

    bool already_found = false;

    for (size_t i = 0; i < stop_times.GetRowCount(); i++) {
        if (stop_times.GetCell<std::string>(0, i) == trip_id) { // trip_id
            already_found = true;
            std::string stop_id = stop_times.GetCell<std::string>(3, i); // stop_id
            int stop_sequence = stop_times.GetCell<int>(4, i); // stop_sequence

            for (size_t j = 0; j < stops.GetRowCount(); j++) {
                if (stops.GetCell<std::string>(0, j) == stop_id) {  // stop_id
                    std::string lat = stops.GetCell<std::string>(3, j); // stop_lat
                    std::string lon = stops.GetCell<std::string>(4, j); // stop_lon
                    stopsResults.push_back({stop_id, lat, lon, stop_sequence});
                    break;
                }
            }
        }
        else if (already_found) break;
    }

    for (auto tuple : stopsResults) {
        std::cout << "ID = " << std::get<0>(tuple) << "   Lat ->  " << std::get<1>(tuple) << "    Lon ->   " << std::get<2>(tuple) << "    Sequence -> " << std::get<3>(tuple) << std::endl;
    }

    return stopsResults;
}
