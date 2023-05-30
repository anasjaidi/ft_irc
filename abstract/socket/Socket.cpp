//
// Created by anas jaidi on 30/3/2023.
//

#include "Socket.hpp"


Socket::Socket() : sock_type(-1), family(-1) {}

Socket::~Socket() {}


Socket::Socket(int family, int socket_type, const char *service)  : service(service), sock_type(socket_type),
                                                                   family(family) {
    int status;

    bzero(&hints, sizeof hints);

    hints.ai_family = family;

    hints.ai_socktype = socket_type;

    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(nullptr, service, &hints, &res);

    if (status < 0)
        throw SeverErrors(SeverErrors::AddrInfoError);
    else
        std::cout << "GetAddrInfo Success " << status << std::endl;
}

Socket::Socket(const char *node, int family, int socket_type, const char *service) : sock_type(socket_type),
                                                                                     family(family) {
    int status;

    bzero(&hints, sizeof hints);

    hints.ai_family = family;

    hints.ai_socktype = socket_type;

    status = getaddrinfo(node, service, &hints, &res);

    if (status < 0)
        throw SeverErrors(SeverErrors::AddrInfoError);
    else
        std::cout << "GetAddrInfo Success " << status << std::endl;

}

const char *Socket::SeverErrors::what() const throw() {

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
        default:
            return "Undefined Error!";
    }
    return "";
}

void Socket::get_socket_fd() throw(SeverErrors) {
    int fd;

    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    int o = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &o, sizeof(int)) < 0)
        ;
    if (fd < 0)
        throw SeverErrors(SeverErrors::SocketFdError);
    else {
        this->socket_fd = fd;
        std::cout << "GetSocketFd Success " << this->socket_fd << std::endl;
    }
}

void Socket::bind_socket_fd() throw(SeverErrors) {
    int status = bind(this->socket_fd, res->ai_addr, res->ai_addrlen);

    if (status < 0)
        throw SeverErrors(SeverErrors::BindFdError);
    else
        std::cout << "BindSocketFd Success " << this->socket_fd << std::endl;
}

void Socket::listen_to_socket() throw(SeverErrors) {
    int status = listen(this->socket_fd, 12);

    if (status < 0)
        throw SeverErrors(SeverErrors::ListenError);
    else
        std::cout <<  "Listen Success " << status << std::endl;
}

Socket::SeverErrors::SeverErrors(ErrorCode _errorCode) : errorCode(_errorCode) {}