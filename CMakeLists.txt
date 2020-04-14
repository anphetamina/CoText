cmake_minimum_required(VERSION 3.15)
project(CoText)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebSockets REQUIRED)

include_directories(client)
include_directories(server)

add_executable(CoText
        client/icons/Notes-icon.png
        client/icons/noun_copy_2080193.png
        "client/icons/noun_Cut File_3118760.png"
        client/icons/noun_Download_2383249.png
        client/icons/noun_Exit_1558251.svg
        "client/icons/noun_file find_556585.png"
        client/icons/noun_Folder_2047792.svg
        "client/icons/noun_lock user_2052931.png"
        "client/icons/noun_new document_2429385.svg"
        "client/icons/noun_new file_2650249.png"
        client/icons/noun_open_1509551.png
        client/icons/noun_paste_2302624.png
        client/icons/noun_Redo_124669.png
        client/icons/noun_replace_1913722.png
        client/icons/noun_Save_1435429.png
        client/icons/noun_Undo_1246691.png
        "client/icons/noun_user login_178831.svg"
        client/icons/qPDS.icns
        client/CMakeLists.txt
        client/CMakeLists2.txt
        client/CoText.icns
        client/CoText.ico
        client/Identifier.cpp
        client/Identifier.h
        client/Login.cpp
        client/Login.h
        client/Login.ui
        "client/main 2.cpp"
        client/main.cpp
        client/MainWindow.cpp
        client/MainWindow.h
        client/MainWindow.ui
        client/Message.cpp
        client/Message.h
        client/NetworkServer.cpp
        client/NetworkServer.h
        client/Packet.cpp
        client/Packet.h
        client/PacketDef.h
        client/Parser.cpp
        client/Parser.h
        client/Register.cpp
        client/Register.h
        client/Register.ui
        client/Resources.qrc
        client/SharedEditor.cpp
        client/SharedEditor.h
        client/Shuffler.cpp
        client/Shuffler.h
        client/sslechoclient.cpp
        client/sslechoclient.h
        client/Symbol.cpp
        client/Symbol.h
        client/User.cpp
        client/User.h
        server/CMakeLists.txt
        server/localhost.cert
        server/localhost.key
        server/main.cpp
        server/Packet.cpp
        server/Packet.h
        server/PacketDef.h
        server/Parser.cpp
        server/Parser.h
        server/securesocketclient.qrc
        server/sslechoserver.cpp
        server/sslechoserver.h
        server/sslechoserver.pro)

target_link_libraries(CoText Qt5::Widgets)
target_link_libraries(CoText Qt5::WebSockets)