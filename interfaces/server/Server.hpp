//
// Created by anas jaidi on 27/3/2023.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

# include "../../FT_IRC.h"

class Server {

private:
    std::string service; // port
    const std::string node; // ip
    const int sock_type; // udp, tcp
    const int family; // ipv4, ipv6
    struct addrinfo hints;
    struct addrinfo *res;
    int socket_fd;
    std::vector<struct pollfd> pfds;
public:
    const addrinfo &getHints() const;

    addrinfo *getRes() const;

    void setRes(addrinfo *res);

    int getSocketFd() const;

    void setSocketFd(int socketFd);

    void setHints(const addrinfo &hints);

    Server();
public:
    std::string getService() const;

    void setService(std::string &service);

public:
    /**
     * Start default constructor
     * */

    /**
     * start parametrised constructor
     * @arg family, socket_type
     *
     * @param family : IPV4, IPV6
     * @param socket_type: SOCK_STREAM, SOCK_DGRAM
     * @param service: Port number
     * */
    Server(int family, int socket_type, const char *service);

    /**
     * start parametrised constructor
    * @arg node, family, socket_type, port, flags
    *
    * @details
    *
    * @param family : IPV4, IPV6
    * @param socket_type: SOCK_STREAM, SOCK_DGRAM
    * @param port: Port number
    * @param flags: other flags
    * */
    Server(const char *node, int family, int socket_type, const char* port);

    /**
     * start prototype of destructor
     * */
    ~Server();

    /*
     * start Exceptions
    **/

class SeverErrors : public std::exception {
public:
    enum ErrorCode {
        AddrInfoError,
        SocketFdError,
        BindFdError,
        ListenError,
        AcceptError,
        ReadError,
        WriteError,
        UndefinedError
    };
    SeverErrors(ErrorCode _errorCode = UndefinedError);

    const char * what() const throw();
private:
    int errorCode;
};


    /**
     * */
    virtual void get_socket_fd() throw(SeverErrors);

    virtual void bind_socket_fd() throw(SeverErrors);

    virtual void listen_to_socket() throw(SeverErrors);

    virtual void accept_incoming_requests() throw(SeverErrors);

};

#endif //FT_IRC_SERVER_HPP