//
// Created by anas jaidi on 27/3/2023.
//


# include "./Server.hpp"


Server::Server() : sock_type(-1), family(-1) {
    std::cout << "Default constructor called \n";
}

Server::SeverErrors::SeverErrors(ErrorCode _errorCode) : errorCode(_errorCode) {}

Server::Server(int family, int socket_type, const char *service) : service(service), sock_type(socket_type),
                                                                   family(family) {
    int status;

    bzero(&hints, sizeof hints);

    hints.ai_family = family;

    hints.ai_socktype = socket_type;

    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(nullptr, service, &hints, &res);

    if (status < 0)
        throw SeverErrors();
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
        throw SeverErrors();
    else
        std::cout << "GetAddrInfo Success " << status << std::endl;

}

Server::~Server() {
    std::cout << "Destructor called \n";
}


/*
 * Start Exceptions What Functions implementation
 * */
const char *Server::SeverErrors::what() const throw() {

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


void Server::get_socket_fd() throw(SeverErrors) {
    int fd;

    fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (fd < 0)
        throw SeverErrors();
    else {
        this->socket_fd = fd;
        std::cout << "GetSocketFd Success " << this->socket_fd << std::endl;
    }
}

void Server::bind_socket_fd() throw(SeverErrors) {
    int status = bind(this->socket_fd, res->ai_addr, res->ai_addrlen);

    if (status < 0)
        throw SeverErrors();
    else
        std::cout << "BindSocketFd Success " << this->socket_fd << std::endl;
}

void Server::listen_to_socket() throw(SeverErrors) {
    int status = listen(this->socket_fd, 12);

    if (status < 0)
        throw SeverErrors(SeverErrors::ErrorCode::ListenError);
    else
        std::cout <<  "Listen Success " << status << std::endl;
}