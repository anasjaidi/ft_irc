//
// Created by anas jaidi on 27/3/2023.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

# include "../../FT_IRC.h"

class Server {

private:
    std::vector<struct pollfd> pfds;

public:

    ~Server();

    /*
     * start Exceptions
    **/

class SeverErrors : public std::exception {
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

    const char * what() const throw();
private:
    int errorCode;
};


    /**
     * */
    virtual void get_socket_fd() throw(SeverErrors);

    virtual void bind_socket_fd() throw(SeverErrors);

    virtual void listen_to_socket() throw(SeverErrors);

    virtual void accept_incoming_requests() throw(SeverErrors);

};

#endif //FT_IRC_SERVER_HPP