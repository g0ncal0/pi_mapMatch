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

std::string convert_coordinates_to_formated_GeoJSON_coordinates(const std::string& coordinates, const std::string& type, bool coordsFromGeoJSON) {
    std::ostringstream oss;

    size_t pos = coordsFromGeoJSON ? 1 : 0, end = 0, aux_end;
    std::string longitude, latitude;
    bool first = true;

    char coord_divider_char = ',';
    char coord_end_char = coordsFromGeoJSON ? ']' : ';';
    int jump_end = coordsFromGeoJSON ? 3 : 1;

    if (type == "Polygon")
        oss << "\n                    [";

    while (pos < coordinates.length() && end != std::string::npos) {
        end = coordinates.find(coord_divider_char, pos);
        longitude = coordinates.substr(pos, end - pos);
        pos = end + 1;

        end = coordinates.find(coord_end_char, pos);
        aux_end = (end != std::string::npos) ? end - pos : end;
        latitude = coordinates.substr(pos, aux_end);

        if (pos != std::string::npos)
            pos = end + jump_end;

        if (!first) oss << ',';
        else first = false;
        
        if (type == "LineString")
            oss << "\n                    [\n                        " << longitude << ",\n                        " << latitude << "\n                    ]";
        else
            oss << "\n                         [" << longitude << ", " << latitude << "]";
    }

    if (type == "Polygon")
        oss << "\n                    ]";

    return oss.str();
}

std::string convert_coordinates_to_GeoJSON_feature(const std::string& coordinates, const std::string& type, bool coordsFromGeoJSON) {
    std::ostringstream oss;

    oss << BEGIN_FEATURE << type << MIDDLE_FEATURE;

    oss << convert_coordinates_to_formated_GeoJSON_coordinates(coordinates, type, coordsFromGeoJSON);

    oss << END_FEATURE;

    return oss.str();
}

std::string create_GeoJSON(const std::string& coordinates, const std::string& excludePolygons) {
    std::ostringstream oss;

    oss << BEGIN_SIMPLE_GEOJSON;

    oss << convert_coordinates_to_GeoJSON_feature(coordinates, "LineString", true);

    if (excludePolygons != "") {
        oss << ",";
        oss << get_exclude_polygons_geoJSON_features(excludePolygons);
    }

    oss << END_SIMPLE_GEOJSON;

    return oss.str();
}

std::string convert_completeGeoJSON_to_simpleGeoJSON(const std::string& completeGeoJSON, const std::string& excludePolygons) {
    std::string coordinates, aux_res;

    aux_res = get_coordinates_from_GeoJson(completeGeoJSON, coordinates);

    if (aux_res != "OK") return aux_res;

    return create_GeoJSON(coordinates, excludePolygons);
}

void convert_input_coordinates_to_valhalla_coordinates(const std::string& inputCoord, std::string& valhallaCoord, int radius) {
    std::ostringstream oss;
    std::istringstream iss(inputCoord);

    std::string lat, lon, radius_str;
    bool first = true;

    if (radius > 0) radius_str = ",\"radius\":" + std::to_string(radius);
    else radius_str = "";

    while (!iss.eof()) {
        if (!first) oss << ',';
        else first = false;

        std::getline(iss, lon, ',');
        std::getline(iss, lat, ';');

        oss << "{\"lat\":" << lat << ",\"lon\":" << lon << radius_str << "}";
    }

    valhallaCoord = oss.str();
}

std::string get_exclude_polygons_geoJSON(const std::string& excludePolygons) {
    if (excludePolygons == "") return "";

    std::ostringstream oss;
    oss << "\n  \"exclude_polygons\": [";

    size_t pos = 1, end;
    std::string excludePolygon;
    bool first = true; 

    while (pos < excludePolygons.length()) {
        if (!first) oss << ",";
        else first = false;

        end = excludePolygons.find(']', pos);
        excludePolygon = excludePolygons.substr(pos, end - pos);
        pos = end + 3;

        oss << convert_coordinates_to_formated_GeoJSON_coordinates(excludePolygon, "Polygon", false);
    }

    oss << "\n  ]\n";

    return oss.str();
}

std::string get_exclude_polygons_geoJSON_features(const std::string& excludePolygons) {
    if (excludePolygons == "") return "";

    std::ostringstream oss;

    size_t pos = 1, end;
    std::string excludePolygon;
    bool first = true; 

    while (pos < excludePolygons.length()) {
        if (!first) oss << ",";
        else first = false;

        end = excludePolygons.find(']', pos);
        excludePolygon = excludePolygons.substr(pos, end - pos);
        pos = end + 3;

        oss << convert_coordinates_to_GeoJSON_feature(excludePolygon, "Polygon", false);
    }

    return oss.str();
}

std::vector<std::vector<std::pair<double, double>>> get_exclude_polygons_list(const std::string& excludePolygons) {
    std::vector<std::vector<std::pair<double, double>>> polygons_list;

    if (excludePolygons == "") return polygons_list;

    size_t pos = 1, end;
    std::string excludePolygon;

    while (pos < excludePolygons.length()) {
        end = excludePolygons.find(']', pos);
        excludePolygon = excludePolygons.substr(pos, end - pos);
        pos = end + 3;


        polygons_list.push_back(get_exclude_polygon_list(excludePolygon));
    }

    return polygons_list;
}

std::vector<std::pair<double, double>> get_exclude_polygon_list(const std::string& excludePolygon) {
    std::vector<std::pair<double, double>> polygonList;

    std::istringstream iss(excludePolygon);

    size_t pos = 0, end = 0, aux_end;
    double longitude, latitude;
    char trash;

    while (iss >> longitude >> trash >> latitude >> trash) {
        polygonList.push_back({longitude, latitude});
    }

    if (polygonList.front() == polygonList.back()) {
        polygonList.pop_back();
    }

    return polygonList;   
}

void extract_json_field(const std::string& json_str, const std::string& key, std::string& value) {
    size_t key_pos = json_str.find("\"" + key + "\":");
    if (key_pos == std::string::npos) return;

    size_t start = json_str.find('"', key_pos + key.length() + 2) + 1;
    size_t end = json_str.find('"', start);
    value = json_str.substr(start, end - start);
}

bool check_points_format(const std::string& points) {
    std::regex pattern("^(-?\\d+(\\.\\d+)?,-?\\d+(\\.\\d+)?)(;-?\\d+(\\.\\d+)?,-?\\d+(\\.\\d+)?)*$");

    if (!std::regex_match(points, pattern)) {
        return false;
    }

    return true;
}

bool check_polygons_format(const std::string& polygons) {
    std::string points_format = "(-?\\d+(\\.\\d+)?,-?\\d+(\\.\\d+)?)(;-?\\d+(\\.\\d+)?,-?\\d+(\\.\\d+)?)*";
    std::string polygon_format = "\\[" + points_format + "\\]";
    std::string full_pattern = "^" + polygon_format + "(," + polygon_format + ")*$";

    std::regex pattern(full_pattern);

    if (!std::regex_match(polygons, pattern) && polygons != "") {
        return false;
    }

    return true;
}
