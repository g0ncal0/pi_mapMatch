#include "algorithms.hpp"

bool no_match(const std::string& res) {
    return (res.find("NoMatch") != std::string::npos);
}

std::string map_match(const std::string& points) {
    const char* points_char = points.c_str();
    char url[150 + points.length()] = "http://router.project-osrm.org/match/v1/driving/";  // TODO: Tem de ser dinâmico. Pode vir a ser parametrizável
    strcat(url, points_char);
    strcat(url, "?steps=false&geometries=geojson&overview=full&annotations=false"); // TODO: Pode vir a ser parametrizável

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

std::string route(const std::string& points) {
    const char* points_char = points.c_str();
    char url[150 + points.length()] = "http://router.project-osrm.org/route/v1/driving/";  // TODO: Tem de ser dinâmico. Pode vir a ser parametrizável
    strcat(url, points_char);
    strcat(url, "?steps=false&geometries=geojson&overview=simplified&annotations=false"); // TODO: Pode vir a ser parametrizável

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

std::string map_match_valhalla(const std::string& points) {
    char url[500 + points.length()*2] = "http://localhost:8002/trace_route";  // TODO: Tem de ser dinâmico. Pode vir a ser parametrizável

    std::string valhallaCoord;
    convert_input_coordinates_to_valhalla_coordinates(points, valhallaCoord);

    std::string json = R"({"shape":[)" + valhallaCoord + R"(],"costing":"bus","shape_match":"map_snap","format":"osrm","shape_format":"geojson","narrative":false})";
    std::string res;

    if (make_request(url, json, res) != 0) {
        return "Error: Bad Request";
    }

    return convert_completeGeoJSON_to_simpleGeoJSON(res, "");
}

std::string route_valhalla(const std::string& points, const std::string& excludePolygons) {
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

std::string bus_route(const std::string& routeID, const std::string& directionID, const std::string& excludePolygons) {
    std::vector<std::tuple<std::string, std::string, std::string, int>> stops = get_stops_from_trip(routeID, directionID);

    std::string points = get_coordinates_string_from_stops(stops);

    return route_valhalla(points, excludePolygons);
}
