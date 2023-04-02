//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_IRCSERVER_HPP
#define FT_IRC_IRCSERVER_HPP

# include "../../abstract/server/Server.hpp"

# include "./commands/Commands.hpp"

# include "../../models/client/client_manager/client_manager.hpp"

class IrcServer: public Server, public Commands, public client_manager{
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

    int handle(std::string) throw() override;
    std::vector<std::string> splitCmd(std::string buff);
};


#endif //FT_IRC_IRCSERVER_HPP
