#include "crow.h"
#include "requests.hpp"
#include "algorithms.hpp"

void setup_routes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    CROW_ROUTE(app, "/map-match/osrm/<string>")([](const std::string& points){
        return map_match(points);
    });

    CROW_ROUTE(app, "/route/osrm/<string>")([](const std::string& points){
        return route(points);
    });

    CROW_ROUTE(app, "/map-match/valhalla/<string>")([](const std::string& points){
        return map_match_valhalla(points);
    });

    CROW_ROUTE(app, "/route/valhalla/<string>")([](const crow::request& req, const std::string& points){
        const char* excludePolygonsPtr = req.url_params.get("exclude_polygons");
        std::string excludePolygons = excludePolygonsPtr ? excludePolygonsPtr : "";
        if (excludePolygons.empty()) {
            excludePolygons = "";
        }
        
        return route_valhalla(points, excludePolygons);
    });
}

int main()
{
    //std::string test = convert_coordinates_to_GeoJSON_feature("-8.611,41.149;-8.610,41.149;-8.610,41.148;-8.611,41.148;-8.611,41.149", "LineString", false);
    //std::string test = convert_coordinates_to_GeoJSON_feature("[-8.598169,41.167126],[-8.598199,41.167062],[-8.598105,41.167032],[-8.597293,41.166804],[-8.597429,41.166511]", "Polygon", true);
    //std::string test = get_exclude_polygons_geoJSON("[-8.611,41.149;-8.610,41.149;-8.610,41.148;-8.611,41.148;-8.611,41.149]");
    
    //std::cout << test << std::endl;

    crow::SimpleApp app;

    setup_routes(app);

    app.port(18080).multithreaded().run();

    return 0;
}
