//
// Created by anas jaidi on 27/3/2023.
//


# include "./Server.hpp"


Server::Server() : sock_type(-1), family(-1) {
    std::cout << "Default constructor called \n";
}

Server::Server(int family, int socket_type, const char *service) : service(service), sock_type(socket_type),
                                                                   family(family) {
    int status;

    bzero(&hints, sizeof hints);

    hints.ai_family = family;

    hints.ai_socktype = socket_type;

    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, service, &hints, &res);

    if (status < 0)
        throw AddrInfoError();
    else
        std::cout << "GetAddrInfo Success " << status << std::endl;
}

Server::Server(const char *node, int family, int socket_type, const char *service) : sock_type(socket_type),
                                                                                                family(family) {
    int status;

    bzero(&hints, sizeof hints);

    hints.ai_family = family;

    hints.ai_socktype = socket_type;

    status = getaddrinfo(node, service, &hints, &res);

    if (status < 0)
        throw AddrInfoError();
    else
        std::cout << "GetAddrInfo Success " << status << std::endl;

}

Server::~Server() {
    std::cout << "Destructor called \n";
}


/*
 * Start Exceptions What Functions implementation
 * */
const char *Server::AddrInfoError::what() const throw() {
    return "GetAddrInfo Function failed To fill infos!";
}

const char *Server::SocketFdError::what() const throw() {
    return "GetSocketFd Function failed to create socket file descriptor!";
}

/*
 * start Attributes Getters and Setters
 * */
std::string Server::getService() const {
    return this->service;
}

void Server::setService(std::string &service) {
    this->service = service;
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


void Server::get_socket_fd() throw(SocketFdError) {
    int fd;

    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (fd < 0)
        throw SocketFdError();
    else {
        this->socket_fd = fd;
        std::cout << "GetSocketFd Success " << this->socket_fd << std::endl;
    }
}