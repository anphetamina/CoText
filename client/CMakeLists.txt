cmake_minimum_required(VERSION 3.14)

project(CoText-client)

set(common_dir ../common)
include_directories(${common_dir})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_BUILD_TYPE RELEASE)
if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif (CMAKE_BUILD_TYPE STREQUAL "RELEASE")


IF(WIN32)
    SET(OS_SPECIFIC_LIBS netapi32 wsock32)
ENDIF(WIN32)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)
#find_package(Qt5 COMPONENTS Sql REQUIRED)
find_package(Qt5 COMPONENTS Svg REQUIRED)
find_package(Qt5 COMPONENTS PrintSupport REQUIRED)
find_package(Qt5WebSockets REQUIRED)

file(GLOB client_src "*.h" "*.cpp" "*.ui" "Resources.qrc")
file(GLOB common_src "${common_dir}/*.h" "${common_dir}/*.cpp")


add_executable(CoText-client ${common_src} ${client_src})

target_link_libraries(CoText-client Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Network Qt5::Svg Qt5::PrintSupport Qt5::WebSockets ${OS_SPECIFIC_LIBS})
