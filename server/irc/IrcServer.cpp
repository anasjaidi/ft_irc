//
// Created by anas jaidi on 30/3/2023.
//

#include "IrcServer.hpp"

IrcServer::IrcServer()  : Server() {
    std::cout << "Default constructor called \n";
}


IrcServer::IrcServer(int family, int socket_type, const char *service) : Server(family, socket_type, service) {
}

IrcServer::IrcServer(const char *node, int family, int socket_type, const char *service) : Server(node, family, socket_type, service) {
}