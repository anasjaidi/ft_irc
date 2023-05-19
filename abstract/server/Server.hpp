//
// Created by anas jaidi on 27/3/2023.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "../../FT_IRC.h"
#include "../socket/Socket.hpp"

class Server : public Socket
{

protected:
    std::vector<struct pollfd> pfds;
    Server();

public:
    ~Server();

    Server(int family, int socket_type, const char *service);

    Server(const char *node, int family, int socket_type, const char *service);
    /*
     * start Exceptions
     **/

    virtual void accept_incoming_requests() throw(ServerErrors);

    virtual std::pair<struct sockaddr_storage, int> accept_and_add_new_client() throw(ServerErrors);
    virtual int remove_client_from_server(int i);

    // virtual int handle(std::string, int) throw(SeverErrors) = 0;
    // virtual int signup(std::pair<struct sockaddr_storage, int> &) = 0;
};

#endif // FT_IRC_SERVER_HPP