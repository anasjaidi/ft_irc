//
// Created by anas jaidi on 27/3/2023.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

# include "../../FT_IRC.h"

class Server {

private:
    std::string service;
    const std::string node;
    const int sock_type;
    const int family;
    struct addrinfo hints;
    struct addrinfo *res;
    int socket_fd;
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
     * @param port: Port number
     * */
    Server(int family, int socket_type, const char *service);

    /**
     * start parametrised constructor
    * @arg node, family, socket_type, port, flags
    *
    * @details
    *
    * @param family : IPV4, IPV6
    * @param socket_type: SOCK_S¬TREAM, SOCK_DGRAM
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
    class AddrInfoError : public std::exception {
        virtual const char *what(void) const  throw();
    };
    class SocketFdError : public std::exception {
        virtual const char *what(void) const  throw();
    };

    /**
     * */

    virtual void get_socket_fd() throw(SocketFdError);
};

#endif //FT_IRC_SERVER_HPP