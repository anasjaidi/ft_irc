//
// Created by anas jaidi on 27/3/2023.
//


# include "./Server.hpp"

/**
 * @brief Default Constructor
 * */
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
        throw SeverErrors(Server::SeverErrors::AddrInfoError);
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
        throw SeverErrors(SeverErrors::AddrInfoError);
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







void Server::listen_to_socket() throw(SeverErrors) {
    int status = listen(this->socket_fd, 12);

    if (status < 0)
        throw SeverErrors(SeverErrors::ErrorCode::ListenError);
    else
        std::cout <<  "Listen Success " << status << std::endl;
}

void Server::accept_incoming_requests() throw(SeverErrors) {

    char buff[1024];

    struct sockaddr_storage their_addr = {};
    socklen_t addr_size;



    pfds.push_back((struct pollfd){.fd = socket_fd, .events = POLLIN, .revents = 0});

    while (1) {
        int len = poll(pfds.data(), pfds.size(), -1);

        for (int i = 0; i < pfds.size(); ++i) {

            if (pfds[i].revents & POLLIN) {

                if (pfds[i].fd == socket_fd) {

                    std::cout << "yes its a new member " << pfds.size() << std::endl;

                    const int new_client_fd = accept(socket_fd, (struct sockaddr *) &their_addr, &addr_size);

                    if (new_client_fd < 0)
                        throw SeverErrors(SeverErrors::ErrorCode::AcceptError);


                    pfds.push_back((struct pollfd){.fd = new_client_fd, .events = POLLIN, .revents = 0});

                } else {

                    std::cout << "already a user\n";

                    const int bytes_len = recv(pfds[i].fd, buff, 1024, 0);

                    if (bytes_len <= 0) {

                        if (bytes_len < 0)
                            throw SeverErrors(SeverErrors::ErrorCode::ReadError);

                        std::cout << "user: " << i << " closes the connection\n";
                        close(pfds[i].fd);
                        pfds.erase(pfds.begin() + i);

                    }

                    buff[bytes_len] = 0;
//                    send(pfds[i].fd, buff, strlen(buff), 0);
                    // TODO: handle the request
                }
            }
        }

    }
}

