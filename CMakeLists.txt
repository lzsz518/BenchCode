cmake_minimum_required(VERSION 3.0)

# set the project name
project(BenchCode)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Boost)
find_package(benchmark)

# add the executable
add_executable(BenchCode main.cpp)

target_link_libraries(BenchCode Qt5::Core)
target_link_libraries(BenchCode benchmark::benchmark)
