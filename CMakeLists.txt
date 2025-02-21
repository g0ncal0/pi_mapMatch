cmake_minimum_required(VERSION 3.10)

project(MapMatchAPI)

set(CMAKE_CXX_STANDARD 17)

find_package(Crow)
find_package(CURL)
find_package(Doxygen REQUIRED)

set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/DoxygenDocs)

add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
)

add_executable(map_match_api src/converter.cpp src/requests.cpp src/algorithms.cpp src/main.cpp)

target_link_libraries(map_match_api PUBLIC Crow::Crow)
target_link_libraries(map_match_api PRIVATE CURL::libcurl)
