//
// Created by anas jaidi on 30/3/2023.
//

#include "IrcServer.hpp"

IrcServer::IrcServer()  : Server() {
    std::cout << "Default constructor called \n";
}

IrcServer::~IrcServer() {
    std::cout << "Destructor called \n";
}

IrcServer::IrcServer(int family, int socket_type, const char *service, std::string server_pass) : Server(family, socket_type, service), server_password(server_pass) {
}

IrcServer::IrcServer(const char *node, int family, int socket_type, const char *service , std::string server_pass) : Server(node, family, socket_type, service), server_password(server_pass) {
}


const char *IrcServer::SeverErrors::what() const throw() {

    static std::string error; // use a static variable to avoid returning a stack address
    error.clear(); // clear the string before each use

    switch (errorCode) {
        case SeverErrors::AddrInfoError :
            error = "GetAddrInfo Function Failed: ";
            break;
        case SeverErrors::SocketFdError :
            error = "GetSocketFd Function Failed: ";
            break;
        case SeverErrors::BindFdError :
            error = "BindSocketFd Function Failed: ";
            break;
        case SeverErrors::ListenError :
            error = "Listen Function Failed: ";
            break;
        default:
            return "Undefined Error!";
    }

    error += std::strerror(errno); // append the system error message to the error string
    return error.c_str();
}

IrcServer::SeverErrors::SeverErrors(ErrorCode _errorCode) : errorCode(_errorCode) {}


void remove_whitespaces(std::string &str) {
    while (std::isspace(str[0])) str.erase(0, 1);
    while (std::isspace(str[str.length() - 1])) str.erase(str.length() - 1, 1);
}

int IrcServer::handle(std::string req, int client_fd) throw() {



    int pos = req.find(' ');

    if (pos == -1)
        pos = req.find('\r');

    if (pos == -1)
        pos = req.find('\n');

    std::string cmd = req.substr(0, pos);

    std::string payload = req.substr(pos + 1, req.length() - 2 );

    remove_whitespaces(payload);

    if (cmd == "pass" || cmd == "PASS") pass(payload, client_fd, server_password);
    else if (cmd == "nick" || cmd == "NICK") nick(req);
   else if (cmd == "user" || cmd == "USER") user(req);
   else if (cmd == "JOIN" || cmd == "join") {
       std::cout << "join is called\n";
   }
}

int IrcServer::signup(std::pair<struct sockaddr_storage, int> &new_client) {
    add_client(new_client.second, new_client.first);
}