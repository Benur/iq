cmake_minimum_required(VERSION 3.0.0)
project(task2 VERSION 0.1.0)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5Charts)

include(CTest)
enable_testing()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(task2 main.cpp plot.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(task2 Qt5::Widgets Qt5::Charts)
