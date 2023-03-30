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
    Server();

public:

    ~Server();
    Server(int family, int socket_type, const char *service);
    Server(const char *node, int family, int socket_type, const char *service);
    /*
     * start Exceptions
    **/

    class SeverErrors  {
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
    virtual const char * what() const throw();
    protected:
        int errorCode;
};

    virtual void accept_incoming_requests() throw() override;

};

#endif //FT_IRC_SERVER_HPP