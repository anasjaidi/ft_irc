//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_IRCSERVER_HPP
#define FT_IRC_IRCSERVER_HPP

# include "../../abstract/server/Server.hpp"


class IrcServer: public Server {
protected:
    IrcServer();
public:

    ~IrcServer();
    IrcServer(int family, int socket_type, const char *service);
    IrcServer(const char *node, int family, int socket_type, const char *service);
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
};


#endif //FT_IRC_IRCSERVER_HPP
