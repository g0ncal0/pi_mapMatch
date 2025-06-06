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
        
        return route_valhalla(points, excludePolygons);
    });

    CROW_ROUTE(app, "/bus-route/<string>/<string>")([](const crow::request& req, const std::string& routeID, const std::string& directionID){
        const char* excludePolygonsPtr = req.url_params.get("exclude_polygons");
        std::string excludePolygons = excludePolygonsPtr ? excludePolygonsPtr : "";
        const char* changesPtr = req.url_params.get("changes");
        std::string changes = changesPtr ? changesPtr : "";
        
        return bus_route(routeID, directionID, excludePolygons, changes);
    });
}

int main()
{
    crow::SimpleApp app;

    setup_routes(app);

    app.port(18080).multithreaded().run();

    return 0;
}
