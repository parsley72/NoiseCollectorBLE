cmake_minimum_required(VERSION 3.5)
project(NoiseCollectorBLE CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(sdbus-c++ REQUIRED)

file(GLOB SOURCES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE SDBusCpp::sdbus-c++)