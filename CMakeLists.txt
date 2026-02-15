cmake_minimum_required(VERSION 3.20)
project(myql LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QuantLib CONFIG REQUIRED)

add_executable(myql main.cpp)
target_link_libraries(myql PRIVATE QuantLib::QuantLib)