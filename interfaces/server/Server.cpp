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