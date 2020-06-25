cmake_minimum_required(VERSION 3.16)
project(PoloniexTradeBot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lcurl")

add_executable(PoloniexTradeBot main.cpp sha256.cpp sha512.cpp API.h API.cpp)
target_link_libraries(PoloniexTradeBot curl)