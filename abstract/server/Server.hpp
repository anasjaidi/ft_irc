//
// Created by anas jaidi on 27/3/2023.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

# include "../../FT_IRC.h"
# include "../socket/Socket.hpp"

class Server : public Socket {

private:
    std::vector<struct pollfd> pfds;

public:

    ~Server();

    /*
     * start Exceptions
    **/

    class SeverErrors : public Socket::SeverErrors {
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
    virtual const char * what() const throw();
};

    virtual void accept_incoming_requests() throw(SeverErrors);

};

#endif //FT_IRC_SERVER_HPP