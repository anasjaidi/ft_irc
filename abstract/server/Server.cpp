//
// Created by anas jaidi on 27/3/2023.
//


# include "./Server.hpp"

/**
 * @brief Default Constructor
 * */
Server::Server() : Socket() {
    std::cout << "Default constructor called \n";
}


Server::Server(int family, int socket_type, const char *service) : Socket(family, socket_type, service) {
}

Server::Server(const char *node, int family, int socket_type, const char *service) : Socket(node, family, socket_type, service) {
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

Server::SeverErrors::SeverErrors(ErrorCode _errorCode) : errorCode(_errorCode) {}

void Server::accept_incoming_requests() {


    char buff[1024];

    pfds.push_back((struct pollfd){.fd = socket_fd, .events = POLLIN, .revents = 0});

    while (1) {
        int len = poll(pfds.data(), pfds.size(), -1);

        for (int i = 0; i < pfds.size(); i++) {

            if (pfds[i].revents & POLLIN) {

                if (pfds[i].fd == socket_fd) {
                    std::pair<struct sockaddr_storage, int> r = accept_and_add_new_client();
                    signup(r);
                } else {

                    char buff[1024];
                    int key = recv(pfds[i].fd, buff, 1024, 0);
                    
                    buff[key] = 0;
                    if (key <= 0) {

                        if (key < 0) throw SeverErrors(SeverErrors::ReadError);
                        remove_client_from_server(pfds[i].fd);
                    }
                    handle(buff, pfds[i].fd);
                }
            }
        }

    }
}

std::pair<struct sockaddr_storage, int> Server::accept_and_add_new_client() throw(SeverErrors) {

    struct sockaddr_storage their_addr = {};
    socklen_t addr_size;

    std::cout << "yes its a new member " << pfds.size() << std::endl;

    const int new_client_fd = accept(socket_fd, (struct sockaddr *) &their_addr, &addr_size);

    if (new_client_fd < 0)
        throw SeverErrors(SeverErrors::AcceptError);


    pfds.push_back((struct pollfd){.fd = new_client_fd, .events = POLLIN, .revents = 0});

    return std::make_pair(their_addr, new_client_fd);
}

int Server::remove_client_from_server(int fd) {
    std::cout << "start remove client from server" << std::endl;
    int i;

    for (i = 0; i < pfds.size(); i++) {
        if (pfds[i].fd == fd)
            break;
    }

    if (i == pfds.size()) {
        // error case
        return 1;
    }

    std::cout << "user: " << i << " closes the connection\n";

    close(pfds[i].fd);

    pfds.erase(pfds.begin() + i);
    return 0;
}
