cmake_minimum_required(VERSION 3.16)
project(PoloniexTradeBot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lcurl")

add_executable(PoloniexTradeBot main.cpp sha512.hpp sha256.hpp hmac.hpp sha256.cpp sha512.cpp API.cpp hmac.cpp)
target_link_libraries(PoloniexTradeBot curl)