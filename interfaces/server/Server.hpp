//
// Created by anas jaidi on 27/3/2023.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

# include "../../FT_IRC.h"

class Server {

private:
    const char *port;
    struct addrinfo hints;
    struct addrinfo *res;

public:
    /**
     * Start default constructor
     * */
    Server();

    /**
     * start parametrised constructor
     * @arg family, socket_type
     *
     * @param family : IPV4, IPV6
     * @param socket_type: SOCK_STREAM, SOCK_DGRAM
     * @param port: Port number
     * */
    Server(int family, int socket_type, const char *port);

    /**
     * start parametrised constructor
    * @arg family, socket_type
    *
    * @param family : IPV4, IPV6
    * @param socket_type: SOCK_STREAM, SOCK_DGRAM
    * @param port: Port number
    * @param flags: other flags
    * */
    Server(int family, int socket_type, const char* port, int flags);

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
};

#endif //FT_IRC_SERVER_HPP