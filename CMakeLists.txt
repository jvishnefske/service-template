cmake_minimum_required(VERSION 3.25)
project(web_server_realtime)

set(CMAKE_CXX_STANDARD 17)
#set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)


#find_package(sqlite3 CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_path(UWEBSOCKETS_INCLUDE_DIRS "uwebsockets/App.h")
find_package(spdlog CONFIG REQUIRED)

add_library(uwebsockets INTERFACE)
target_include_directories(uwebsockets INTERFACE ${UWEBSOCKETS_INCLUDE_DIRS})
add_library(uwebsockets::uwebsockets ALIAS uwebsockets)
set(dependencies unofficial::sqlite3::sqlite3 spdlog::spdlog uwebsockets nlohmann_json::nlohmann_json)

add_library(backend backend.cpp)
target_link_libraries(backend PRIVATE ${dependencies})
add_executable(main main.cpp)
target_link_libraries(main PRIVATE backend)

add_subdirectory(test)

