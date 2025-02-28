#include "algorithms.hpp"

bool no_match(const std::string& res) {
    return (res.find("NoMatch") != std::string::npos);
}

std::string map_match(const std::string& points) {
    const char* points_char = points.c_str();
    char url[150 + points.length()] = "http://router.project-osrm.org/match/v1/driving/";  // TODO: Tem de ser dinâmico. Pode vir a ser parametrizável
    strcat(url, points_char);
    strcat(url, "?steps=true&geometries=geojson&overview=full&annotations=false"); // TODO: Pode vir a ser parametrizável

    std::string res;

    if (make_request(url, res) != 0) {
        return "Error: Bad Request";
    }

    if (no_match(res)) {
        return "Error: No Match Found";
    }

    return convert_completeGeoJSON_to_simpleGeoJSON(res);
}
