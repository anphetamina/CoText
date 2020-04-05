cmake_minimum_required(VERSION 3.1.0)

project(CoText-client)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x")

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
find_package(Qt5 COMPONENTS Sql REQUIRED)
find_package(Qt5 COMPONENTS Svg REQUIRED)
find_package(Qt5 COMPONENTS PrintSupport REQUIRED)

file(GLOB CLIENT "*.h" "*.cpp" "*.ui" "Resources.qrc")

add_executable(CoText-client main.cpp ${CLIENT})

target_link_libraries(CoText-client Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Network Qt5::Sql Qt5::Svg Qt5::PrintSupport)