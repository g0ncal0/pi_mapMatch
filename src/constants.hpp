#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

const std::string BEGIN_SIMPLE_GEOJSON = R"({
    "type": "FeatureCollection",
    "features": [
        {
            "type": "Feature",
            "geometry": {
                "type": "LineString",
                "coordinates": [)";

const std::string END_SIMPLE_GEOJSON = R"(
                ]
            },
            "properties": {}
        }
    ]
}
)";

#endif
