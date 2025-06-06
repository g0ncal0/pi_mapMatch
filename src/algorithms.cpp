#include "algorithms.hpp"

bool no_match(const std::string& res) {
    return (res.find("NoMatch") != std::string::npos);
}

std::string map_match_osrm(const std::string& points) {
    const char* points_char = points.c_str();
    char url[150 + points.length()] = "http://router.project-osrm.org/match/v1/driving/";
    strcat(url, points_char);
    strcat(url, "?steps=false&geometries=geojson&overview=full&annotations=false");

    std::string res;

    if (make_request(url, "", res) != 0) {
        return "Error: Bad Request";
    }

    if (no_match(res)) {
        std::string geoJSON = route(points);
        std::string coord, reqCoord;

        get_coordinates_from_GeoJson(geoJSON, coord);

        convert_GeoJSON_coordinates_to_request_coordinates(coord, reqCoord);

        return map_match(reqCoord);       // TODO: tem de ser dada informação que não é um map matching normal
        // TODO: isto é um bocado disparate porque a route com overview full fica praticamente igual ao que estou a fazer e deve ser mais rápido -> Verificar se compensa ou não
    }

    return convert_completeGeoJSON_to_simpleGeoJSON(res, "");
}

std::string route_osrm(const std::string& points) {
    const char* points_char = points.c_str();
    char url[150 + points.length()] = "http://router.project-osrm.org/route/v1/driving/";
    strcat(url, points_char);
    strcat(url, "?steps=false&geometries=geojson&overview=simplified&annotations=false");

    std::string res;

    if (make_request(url, "", res) != 0) {
        return "Error: Bad Request";
    }

    if (no_match(res)) {
        return "Error: No Match Found";
    }

    return convert_completeGeoJSON_to_simpleGeoJSON(res, "");

    // TODO: se for para também ter serviço de routes isto não pode ficar assim
}

std::string map_match_valhalla(const std::string& points, int radius) {
    // When running the project localy the URL should be with "localhost" -> "http://localhost:8002/trace_route"
    // When running the project with docker the URL should be with "valhalla" -> "http://valhalla:8002/trace_route"
    char url[500 + points.length()*2] = "http://localhost:8002/trace_route";  // TODO: Tem de ser dinâmico. Pode vir a ser parametrizável

    std::string valhallaCoord;
    convert_input_coordinates_to_valhalla_coordinates(points, valhallaCoord, radius);

    std::string json = R"({"shape":[)" + valhallaCoord + R"(],"costing":"bus","shape_match":"map_snap","format":"osrm","shape_format":"geojson","narrative":false})";
    std::string res;

    if (make_request(url, json, res) != 0) {
        return "Error: Bad Request";
    }

    return convert_completeGeoJSON_to_simpleGeoJSON(res, "");
}

std::string route_valhalla(const std::string& points, const std::string& excludePolygons) {
    // When running the project localy the URL should be with "localhost" -> "http://localhost:8002/route"
    // When running the project with docker the URL should be with "valhalla" -> "http://valhalla:8002/route"
    char url[500 + points.length()*2 + excludePolygons.length()*2] = "http://localhost:8002/route";

    std::string valhallaCoord;
    convert_input_coordinates_to_valhalla_coordinates(points, valhallaCoord);

    std::string excludePolygonsGeoJSON = get_exclude_polygons_geoJSON(excludePolygons);

    std::string json = R"({"locations":[)" + valhallaCoord + R"(],"costing":"bus","narrative":false,"format":"osrm","shape_format":"geojson")" + (excludePolygons != "" ? "," + excludePolygonsGeoJSON : "") + "}";

    std::string res;

    if (make_request(url, json, res) != 0) {
        return "Error: Bad Request";
    }

    return convert_completeGeoJSON_to_simpleGeoJSON(res, excludePolygons);
}

bool pointIsInsidePolygon(double lon, double lat, const std::vector<std::pair<double, double>>& polygon) {
    int n = polygon.size();
    bool inside = false;
    
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;

        double x1 = polygon[i].first, y1 = polygon[i].second;
        double x2 = polygon[j].first, y2 = polygon[j].second;

        if (((y1 > lat) != (y2 > lat)) &&
            (lon < (x2 - x1) * (lat - y1) / (y2 - y1) + x1)) {
            inside = !inside;
        }
    }

    return inside;
}

std::string remove_stops_in_excluded_zones(std::list<std::tuple<std::string, std::string, std::string, int>>& stops, const std::vector<std::vector<std::pair<double, double>>>& excludePolygons) {
    std::ostringstream oss;
    bool first = true;
    
    auto it = stops.begin();

    while (it != stops.end()) {
        double longitude = std::stod(std::get<2>(*it));
        double latitude = std::stod(std::get<1>(*it));

        for (auto polygon : excludePolygons) {
            if (pointIsInsidePolygon(longitude, latitude, polygon)) {
                if (!first) oss << ", ";
                else first = false;

                oss << std::get<0>(*it);

                it = stops.erase(it);
                it--;
                break;
            }
        }

        it++;
    }

    std::string reportRemovedStops = oss.str();

    if (!reportRemovedStops.empty()) {
        return reportRemovedStops + " stops had to be removed because collided with one or more excluded zones.\n";
    }

    return "";
}

std::string process_put_stop(const std::string& putInfo, std::list<std::tuple<std::string, std::string, std::string, int>>& stops) {
    auto it = stops.begin();

    std::string stopID = putInfo.substr(putInfo.find_last_of(':')+1);
    int newStopIndex = stoi(putInfo.substr(2, putInfo.find_last_of(':')-2));
    std::string lat, lon;
    get_coords_from_stop(stopID, lat, lon);

    if (newStopIndex > stops.size()) {
        return "ERROR: This route don't have that many stops";
    }

    int i = newStopIndex;

    while (i > 0) {
        i--;
        it++;
    }

    it = stops.insert(it, {stopID, lat, lon, newStopIndex});

    return "OK";
}

std::string process_put_coord(const std::string& putInfo, std::list<std::tuple<std::string, std::string, std::string, int>>& stops) {
    auto it = stops.begin();

    std::string coords = putInfo.substr(putInfo.find_last_of(':')+1);
    std::string lat = coords.substr(0, coords.find(','));
    std::string lon = coords.substr(coords.find(',') + 1);
    int newStopIndex = stoi(putInfo.substr(2, putInfo.find_last_of(':')-2));

    if (newStopIndex > stops.size()) {
        return "ERROR: This route don't have that many stops";
    }

    int i = newStopIndex;

    while (i > 0) {
        i--;
        it++;
    }

    it = stops.insert(it, {"StopTest", lat, lon, newStopIndex});

    return "OK";
}

std::string process_delete_stop(const std::string& deleteInfo, std::list<std::tuple<std::string, std::string, std::string, int>>& stops) {
    auto it = stops.begin();

    std::string stopID = deleteInfo.substr(deleteInfo.find_last_of(':')+1);
    std::string auxStopId;

    while (it != stops.end()) {
        auxStopId = std::get<0>(*it);
        if (auxStopId == stopID) {
            stops.erase(it);
            return "OK";
        }
        it++;
    }

    return "ERROR: That stop doesnt exist in this route!";
}

std::string process_changes(const std::string& changes, std::list<std::tuple<std::string, std::string, std::string, int>>& stops) {
    size_t pos = 0, end;
    std::string change;

    while (end != std::string::npos) {
        end = changes.find(';', pos);
        change = changes.substr(pos, end - pos);
        pos = end + 1;

        if (change.front() == 'P') process_put_stop(change, stops);
        else if (change.front() == 'C') process_put_coord(change, stops);
        else if (change.front() == 'D') process_delete_stop(change, stops);
        else {
            std::cerr << "Error: Unknown change type: " << change.front() << std::endl;
            return "Error: Unknown change type";
        }
    }

    return "OK";
}

std::string bus_route(const std::string& routeID, const std::string& directionID, const std::string& excludePolygons, const std::string& changes) {
    int stops_radius;
    if (excludePolygons.empty()) stops_radius = 20;
    else stops_radius = 0;

    std::list<std::tuple<std::string, std::string, std::string, int>> stops = get_stops_from_trip(routeID, directionID);
    std::vector<std::vector<std::pair<double, double>>> excludePolygonsList = get_exclude_polygons_list(excludePolygons);
    std::string reportRemovedStops = remove_stops_in_excluded_zones(stops, excludePolygonsList);
    process_changes(changes, stops);
    std::string points = get_coordinates_string_from_stops(stops);

    if (excludePolygons.empty()) {
        return map_match_valhalla(points, 20);
    }

    return reportRemovedStops + route_valhalla(points, excludePolygons);
}
