cmake_minimum_required(VERSION 3.29)
project(cpp_cli_auth_boilerplate)

set(CMAKE_CXX_STANDARD 20)

find_package(Sqlite3 REQUIRED)
add_executable(cpp_cli_auth_boilerplate main.cpp
        database/DatabaseManager.cpp
        database/DatabaseManager.h
        model/User.h
        service/UserService.h
        service/UserService.cpp
        model/User.cpp
)
target_link_libraries(cpp_cli_auth_boilerplate sqlite3)