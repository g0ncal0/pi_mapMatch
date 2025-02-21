#include "algorithms.hpp"

std::string map_match(const std::string& points) {
    const char* points_char = points.c_str();
    char url[150 + points.length()] = "http://localhost:5000/match/v1/driving/";  // TODO: Tem de ser dinâmico. Pode vir a ser parametrizável
    strcat(url, points_char);
    strcat(url, "?steps=true&geometries=geojson&overview=full&annotations=false"); // TODO: Pode vir a ser parametrizável

    std::string res;

    if (make_request(url, res) == 0) {
        return res;
    }
    else {
        return "Error";
    }
}
