#include <iostream>
#include <string>
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
}

int main()
{
    crow::SimpleApp app;

    setup_routes(app);

    app.port(18080).multithreaded().run();

    return 0;
}
