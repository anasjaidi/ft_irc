cmake_minimum_required(VERSION 3.24)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)

add_executable(ft_irc main.cpp FT_IRC.h interfaces/server_interface/server.cpp interfaces/server_interface/server.hpp commands/commands.cpp)
