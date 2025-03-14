#include "converter.hpp"

std::string get_coordinates_from_GeoJson(const std::string& completeGeoJSON, std::string& coordinates) {
    size_t start = completeGeoJSON.find("coordinates");

    if (start != std::string::npos) {
        size_t end = completeGeoJSON.find("]]", start);
        if (end != std::string::npos) {
            coordinates = completeGeoJSON.substr(start + 14, end - start - 13);
        }
        else {
            return "Error: Bad Format";
        }
    }
    else {
        return "Error: Bad Format";
    }

    return "OK";
}

void convert_GeoJSON_coordinates_to_request_coordinates(const std::string& geoCoord, std::string& reqCoord) {
    std::ostringstream oss;
    std::istringstream iss(geoCoord.substr(1, geoCoord.size() - 2));
    std::string buf;
    char trash;
    bool first = true;

    while (std::getline(iss, buf, ']')) {
        if (!first) oss << ';';
        else first = false;

        oss << buf;

        iss >> trash >> trash;
    }

    reqCoord = oss.str();
}

std::string convert_coordinates_to_GeoJSON_feature(const std::string& coordinates, const std::string& type, bool coordsFromGeoJSON) {
    std::ostringstream oss;

    oss << BEGIN_FEATURE << type << MIDDLE_FEATURE;

    size_t pos = 1, end, aux_end;
    std::string_view longitude, latitude;
    bool first = true;

    char coord_divider_char = ',';
    char coord_end_char = coordsFromGeoJSON ? ']' : ';';
    int jump_end = coordsFromGeoJSON ? 3 : 1;

    while (pos < coordinates.length()) {
        end = coordinates.find(coord_divider_char, pos);
        longitude = coordinates.substr(pos, end - pos);
        pos = end + 1;

        end = coordinates.find(coord_end_char, pos);
        aux_end = (end != std::string::npos) ? end - pos : end;
        latitude = coordinates.substr(pos, aux_end);
        pos = end + jump_end;

        if (!first) oss << ',';
        else first = false;
        
        oss << "\n                    [\n                        " << longitude << ",\n                        " << latitude << "\n                    ]";
    }

    oss << END_FEATURE;

    return oss.str();
}

std::string convert_completeGeoJSON_to_simpleGeoJSON(const std::string& completeGeoJSON) {
    std::string coordinates, aux_res;

    aux_res = get_coordinates_from_GeoJson(completeGeoJSON, coordinates);

    std::cout << coordinates << std::endl;

    if (aux_res != "OK") return aux_res;

    std::ostringstream oss;

    oss << BEGIN_SIMPLE_GEOJSON;

    size_t pos = 1, end;
    std::string_view longitude, latitude;
    bool first = true;

    while (pos < coordinates.length()) {
        end = coordinates.find(',', pos);
        longitude = coordinates.substr(pos, end - pos);
        pos = end + 1;

        end = coordinates.find(']', pos);
        latitude = coordinates.substr(pos, end - pos);
        pos = end + 3;

        if (!first) oss << ',';
        else first = false;
        
        oss << "\n                    [\n                        " << longitude << ",\n                        " << latitude << "\n                    ]";
    }

    oss << END_SIMPLE_GEOJSON;

    return oss.str();
}

void convert_input_coordinates_to_valhalla_coordinates(const std::string& inputCoord, std::string& valhallaCoord) {
    std::ostringstream oss;
    std::istringstream iss(inputCoord);

    std::string lat, lon;
    bool first = true;

    while (!iss.eof()) {
        if (!first) oss << ',';
        else first = false;

        std::getline(iss, lon, ',');
        std::getline(iss, lat, ';');

        oss << "{\"lat\":" << lat << ",\"lon\":" << lon << "}";
    }

    valhallaCoord = oss.str();
}
