#include "converter.hpp"

std::string convert_completeGeoJSON_to_simpleGeoJSON(const std::string& completeGeoJSON) {
    size_t start = completeGeoJSON.find("coordinates");
    std::string coordinates;

    if (start != std::string::npos) {
        size_t end = completeGeoJSON.find("]]", start);
        if (end != std::string::npos) {
            coordinates = completeGeoJSON.substr(start + 14, end - start - 13);
        }
        else {
            std::cout << "Bad Format" << std::endl;

            return "Error";
        }
    }
    else {
        std::cout << "Bad Format" << std::endl;

        return "Error";
    }

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
