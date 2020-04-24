cmake_minimum_required(VERSION 3.15)
project(CoText-server)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
#set(INSTALL_EXAMPLEDIR "examples/websockets/sslechoserver")

find_package(Qt5 COMPONENTS WebSockets)
find_package(Qt5 COMPONENTS Gui REQUIRED)

add_executable(CoText-server main.cpp sslechoserver.cpp sslechoserver.h PacketDef.h Packet.cpp Packet.h PingPacket.cpp PingPacket.h LoginPacket.cpp LoginPacket.h AccountPacket.cpp AccountPacket.h Client.cpp Client.h User.cpp User.h)

target_link_libraries(CoText-server PUBLIC
        Qt5::WebSockets
        Qt5::Gui
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
