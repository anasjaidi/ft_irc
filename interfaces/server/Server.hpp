//
// Created by anas jaidi on 27/3/2023.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

# include "../../FT_IRC.h"

class Server {

private:
    std::string port;

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
     * */
    Server(int family, int socket_type);

    /**
     * start parametrised constructor
    * @arg family, socket_type
    *
    * @param family : IPV4, IPV6
    * @param socket_type: SOCK_STREAM, SOCK_DGRAM
    * @param flags: other flags
    * */
    Server(int family, int socket_type, int flags);

    ~Server();
};

#endif //FT_IRC_SERVER_HPP