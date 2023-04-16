//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_IRCSERVER_HPP
#define FT_IRC_IRCSERVER_HPP

# include "../../abstract/server/Server.hpp"

# include "./commands/Commands.hpp"

# include "../../models/client/client_manager/client_manager.hpp"

# include "../../FT_IRC.h"

class IrcServer: public Server, public Commands{
protected:
    IrcServer();


    std::string server_password;

    std::string server_name;

public:

    ~IrcServer();
    IrcServer(int family, int socket_type, const char *service, std::string server_pass);
    IrcServer(const char *node, int family, int socket_type, const char *service, std::string server_pass);
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

    int handle(std::string, int) throw() override;
    int signup(std::pair<struct sockaddr_storage, int> &) override;
};




#endif //FT_IRC_IRCSERVER_HPP
