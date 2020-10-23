cmake_minimum_required(VERSION 3.14)
project(CoText-server)

set(common_dir ../common)
include_directories(${common_dir})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(INSTALL_EXAMPLEDIR "examples/websockets/sslechoserver")
IF(APPLE OR LINUX)
    set(DYLD_LIBRARY_PATH  "/usr/local/mysql/lib/")
ENDIF(APPLE OR LINUX)
set(QT_DEBUG_PLUGINS 1)

#set(CMAKE_BUILD_TYPE RELEASE)

if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif (CMAKE_BUILD_TYPE STREQUAL "RELEASE")

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS WebSockets)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Sql REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

file(GLOB server_src "*.h" "*.cpp")
file(GLOB common_src "${common_dir}/*.h" "${common_dir}/*.cpp")

add_executable(CoText-server ${common_src} ${server_src})

target_link_libraries(CoText-server PUBLIC
        Qt5::Core
        Qt5::Network
        Qt5::WebSockets
        Qt5::Gui
        Qt5::Sql
        ${OS_SPECIFIC_LIBS}
        )
#target_link_libraries(CoText-client Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Network Qt5::Sql Qt5::Svg Qt5::PrintSupport Qt5::WebSockets ${OS_SPECIFIC_LIBS})
# Resources:
set(securesocketclient_resource_files
        "localhost.cert"
        "localhost.key"
        )

qt5_add_resources(CoText-server "securesocketclient"
        PREFIX
        "securesocketclient.qrc"
        FILES
        ${securesocketclient_resource_files}
        )

#install(TARGETS sslechoserver
#        RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
#        BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
#        LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
#        )
#https://stackoverflow.com/questions/33743493/why-visual-studio-2015-cant-run-exe-file-ucrtbased-dll