#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

const std::string BEGIN_SIMPLE_GEOJSON = R"({
    "type": "FeatureCollection",
    "features": [)";

const std::string END_SIMPLE_GEOJSON = R"(
}
)";

const std::string BEGIN_FEATURE = R"(
        {
            "type": "Feature",
            "geometry": {
                "type": ")";

const std::string MIDDLE_FEATURE = R"(",
                "coordinates": [)";

const std::string END_FEATURE = R"(
                ]
            },
            "properties": {}
        })";

#endif
