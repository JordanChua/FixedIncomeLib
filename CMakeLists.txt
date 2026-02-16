cmake_minimum_required(VERSION 3.21)
project(FixedIncomeLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force MSVC runtime to match vcpkg "static" triplet (MT/MTd)
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_package(QuantLib CONFIG REQUIRED)

add_executable(testdate
    fixedincomelib/tests/testdate.cpp
    fixedincomelib/Date/basics.cpp
    fixedincomelib/market/basics.cpp
)

target_include_directories(testdate PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(testdate PRIVATE QuantLib::QuantLib)

