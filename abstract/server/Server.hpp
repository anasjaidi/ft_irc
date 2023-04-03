//
// Created by anas jaidi on 27/3/2023.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

# include "../../FT_IRC.h"
# include "../socket/Socket.hpp"

class Server : public Socket {

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

    virtual std::pair<struct sockaddr_storage, int> accept_and_add_new_client() throw(SeverErrors);
    virtual int remove_client(int i);

    virtual int handle(std::string , int) throw(SeverErrors) = 0;
    virtual int signup(std::pair<struct sockaddr_storage, int> &) = 0 ;
};

#endif //FT_IRC_SERVER_HPP