#include "crow.h"
#include "requests.hpp"
#include "algorithms.hpp"

void setup_routes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    CROW_ROUTE(app, "/map-match/<string>")([](const std::string& points){
        return map_match(points);
    });

    CROW_ROUTE(app, "/route/osrm/<string>")([](const std::string& points){
        return route(points);
    });

    CROW_ROUTE(app, "/route/valhalla/<string>")([](const std::string& points){
        return route_valhalla(points);
    });
}

int main()
{
    crow::SimpleApp app;

    setup_routes(app);

    app.port(18080).multithreaded().run();

    return 0;
}
