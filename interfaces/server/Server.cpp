//
// Created by anas jaidi on 27/3/2023.
//


# include "./Server.hpp"


Server::Server() {
    std::cout << "Default constructor called \n";
}

Server::Server(int family, int socket_type, const char * port) : port(port) {
    int status;

    status = getaddrinfo(NULL, this->port, &hints, &res);

    if (status < 0)
        throw AddrInfoError();
    else
        std::cout << "GetAddrInfo Success " << status << std::endl;
}

Server::~Server() {
    std::cout << "Destructor called \n";
}



const char *Server::AddrInfoError::what() const throw() {
    return  "GetAddrInfo Function failed To fill infos!";
}

const char *Server::getPort() const {
    return port;
}

void Server::setPort(const char *port) {
    Server::port = port;
}

const addrinfo &Server::getHints() const {
    return hints;
}

void Server::setHints(const addrinfo &hints) {
    Server::hints = hints;
}

addrinfo *Server::getRes() const {
    return res;
}

void Server::setRes(addrinfo *res) {
    Server::res = res;
}

int Server::getSocketFd() const {
    return socket_fd;
}

void Server::setSocketFd(int socketFd) {
    socket_fd = socketFd;
}
