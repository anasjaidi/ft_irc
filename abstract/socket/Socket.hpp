//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_SOCKET_HPP
#define FT_IRC_SOCKET_HPP

# include "../../FT_IRC.h"


class Socket {
    std::string service; // port
    const std::string node; // ip
    const int sock_type; // udp, tcp
    const int family; // ipv4, ipv6
    struct addrinfo hints;
    struct addrinfo *res;
    int socket_fd;
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
    Socket(int family, int socket_type, const char *service);

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
    Socket(const char *node, int family, int socket_type, const char* port);

    /**
     * start prototype of destructor
     * */
     ~Socket();

    class SeverErrors : public std::exception {
    public:
        enum ErrorCode {
            AddrInfoError,
            SocketFdError,
            BindFdError,
            ReadError,
            WriteError,
            UndefinedError
        };
        SeverErrors(ErrorCode _errorCode = UndefinedError);

        virtual const char * what() const throw();
    private:
        int errorCode;
    };

    virtual void get_socket_fd() throw(SeverErrors);

    virtual void bind_socket_fd() throw(SeverErrors);
};


#endif //FT_IRC_SOCKET_HPP
