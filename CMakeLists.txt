cmake_minimum_required(VERSION 3.10)

project(websocket_test)

find_package(Boost REQUIRED COMPONENTS
    system)

add_executable(websocket_test
    base64.cpp
    # main.cpp)
    echo_server.cpp)

target_link_libraries(websocket_test
    ${Boost_LIBRARIES})

target_include_directories(websocket_test
    PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/json
    PRIVATE ${CMAKE_SOURCE_DIR}/3rd_party/websocketpp)