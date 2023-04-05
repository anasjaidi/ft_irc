cmake_minimum_required(VERSION 3.24)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)

add_executable(ft_irc main.cpp FT_IRC.h abstract/server/Server.cpp abstract/server/Server.hpp abstract/socket/Socket.cpp abstract/socket/Socket.hpp server/irc/IrcServer.cpp server/irc/IrcServer.hpp models/client/client.cpp models/client/client.hpp server/irc/commands/Commands.cpp server/irc/commands/Commands.hpp models/client/client_manager/client_manager.cpp models/client/client_manager/client_manager.hpp server/irc/channel/channel.cpp)
