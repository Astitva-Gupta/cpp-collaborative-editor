cmake_minimum_required(VERSION 3.16)
project(CollaborativeEditor)

find_package(Boost REQUIRED COMPONENTS system thread)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(editor_server
    main.cpp
    editor_server.cpp
    editor_server.hpp
)

add_executable(editor_client
    editor_client.cpp
)

target_link_libraries(editor_server
    PRIVATE
        Boost::system
        Boost::thread
        nlohmann_json::nlohmann_json
)

target_link_libraries(editor_client
    PRIVATE
        Boost::system
        Boost::thread
        nlohmann_json::nlohmann_json
)

target_compile_definitions(editor_server PRIVATE _WIN32_WINNT=0x0601)
target_compile_definitions(editor_client PRIVATE _WIN32_WINNT=0x0601)

target_include_directories(editor_server PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(editor_client PRIVATE ${Boost_INCLUDE_DIRS})
