cmake_minimum_required(VERSION 3.10)

project(MapMatchAPI)

set(CMAKE_CXX_STANDARD 17)

find_package(Crow)

add_executable(map_match_api src/main.cpp)

target_link_libraries(map_match_api PUBLIC Crow::Crow)
