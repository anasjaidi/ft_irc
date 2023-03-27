//
// Created by anas jaidi on 27/3/2023.
//


# include "./server.hpp"


Server::Server() {
    std::cout << "Default constructor called \n";
}

Server::Server(int family, int socket_type) {

}

Server::~Server() {
    std::cout << "Destructor called \n";
}