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

void Server::bind_socket_fd() throw(SeverErrors) {
    int status = bind(this->socket_fd, res->ai_addr, res->ai_addrlen);

    if (status < 0)
        throw SeverErrors(SeverErrors::BindFdError);
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

void Server::accept_incoming_requests() throw(SeverErrors) {

    char buff[1024];

    struct sockaddr_storage their_addr = {};
    socklen_t addr_size;

    char *reply =
            "HTTP/1.1 200 OK\n"
            "Server: Apache/2.2.3\n"
            "Content-Type: text/html\n"
            "Content-Length: 15\n"
            "Accept-Ranges: bytes\n"
            "Connection: close\n"
            "\n"
            "Hello, World!";

    struct pollfd hint;

    hint.fd = socket_fd;
    hint.events = POLLIN;

    pfds.push_back(hint);

    while (1) {
        int len = poll(pfds.data(), pfds.size(), 1000);

        std::cout << len << std::endl;

        for (int i = 0; i < pfds.size(); ++i) {
            if (pfds[i].revents & POLLIN) {
                if (pfds[i].fd == socket_fd) {
                    std::cout << "yes its a new member\n" << pfds.size() << std::endl;

                    const int new_client_fd = accept(socket_fd, (struct sockaddr *) &their_addr, &addr_size);

                    if (new_client_fd < 0)
                        throw SeverErrors(SeverErrors::ErrorCode::AcceptError);

                    const struct pollfd hint = {.fd = new_client_fd, .events = POLLIN};


                    pfds.push_back(hint);
                } else {
                    // TODO: handle exist client
                    std::cout << "no its not a new member\n";
                    send(pfds[i].fd, "anas\n", strlen("anas\n"), 0);
                }
            }


//            int key = send(new_client_fd, reply, strlen(reply), 0);
        }

    }








//    if (!fork()) { // this is the child process
//
//        close(this->socket_fd); // child doesn't need the listener
//
//
//        std::cout << "key: " << key << std::endl;
//
//        if ( key == -1)
//            perror("send");
//
//        close(new_client_fd);
//        exit(0);
//    }
//    close(new_client_fd);
//
//    if (new_client_fd < 0)
//        throw SeverErrors(SeverErrors::ErrorCode::AcceptError);
//    else
//        std::cout << "ACCEPT NEW CLIENT IN A FD: " << new_client_fd << std::endl;

}

