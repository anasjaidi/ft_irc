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

void Server::accept_incoming_requests() throw() {


    char buff[1024];

    pfds.push_back((struct pollfd){.fd = socket_fd, .events = POLLIN, .revents = 0});

    while (1) {
        int len = poll(pfds.data(), pfds.size(), -1);

        for (int i = 0; i < pfds.size(); ++i) {

            if (pfds[i].revents & POLLIN) {

                if (pfds[i].fd == socket_fd) {
                    add_new_client();
                } else {

                    std::cout << "already a user\n";

                    std::pair<std::string, int> data = read_from_socket_fd(pfds[i].fd);
                    if (data.second <= 0) {

                        if (data.second < 0)
                            throw SeverErrors(SeverErrors::ErrorCode::ReadError);
                        remove_client(i);
                    }

                    write_to_socket_fd(data.first, pfds[i].fd);
                }
            }
        }

    }
}

int Server::add_new_client() {

    struct sockaddr_storage their_addr = {};
    socklen_t addr_size;

    std::cout << "yes its a new member " << pfds.size() << std::endl;

    const int new_client_fd = accept(socket_fd, (struct sockaddr *) &their_addr, &addr_size);

    if (new_client_fd < 0)
        throw SeverErrors(SeverErrors::ErrorCode::AcceptError);


    pfds.push_back((struct pollfd){.fd = new_client_fd, .events = POLLIN, .revents = 0});

    return 0;
}

int Server::remove_client(int i) {

    std::cout << "user: " << i << " closes the connection\n";

    close(pfds[i].fd);

    pfds.erase(pfds.begin() + i);
    return 0;
}
