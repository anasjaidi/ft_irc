/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmaziane <zmaziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:54:19 by zmaziane          #+#    #+#             */
/*   Updated: 2023/05/30 18:54:20 by zmaziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_SOCKET_HPP
#define FT_IRC_SOCKET_HPP

# include "../../FT_IRC.h"


class Socket {
protected:
    std::string service; // port
    const std::string node; // ip
    const int sock_type; // udp, tcp
    const int family; // ipv4, ipv6
    struct addrinfo hints;
    struct addrinfo *res;
    int socket_fd;
    Socket();
public:
    /**
     * Start default constructor
     * */


    /**
     * start parametrised constructor
     * @arg family, socket_type
     *
     * @param family : IPV4, IPV6
     * @param socket_type: SOCK_STREAM, SOCK_DGRAM
     * @param service: Port number
     * */
    Socket(int family, int socket_type, const char *service);

    /**
     * start paramtrised constructor
    * @arg node, family, socket_type, port, flags
    *
    * @details
    *
    * @param family : IPV4, IPV6
    * @param socket_type: SOCK_STREAM, SOCK_DGRAM
    * @param port: Port number
    * @param flags: other flags
    * */
    Socket(const char *node, int family, int socket_type, const char* port);

    /**
     * start prototype of destructor
     * */
     ~Socket();

    class SeverErrors : public std::exception {
    public:
        enum ErrorCode {
            AddrInfoError,
            SocketFdError,
            BindFdError,
            ListenError,
            ReadError,
            WriteError,
            UndefinedError
        };
        SeverErrors(ErrorCode _errorCode = UndefinedError);

        virtual const char * what() const throw();

    protected:
        int errorCode;
    };

    virtual void get_socket_fd() throw(SeverErrors);

    virtual void bind_socket_fd() throw(SeverErrors);

    virtual void listen_to_socket() throw(SeverErrors);

    virtual void accept_incoming_requests() = 0;
};


#endif //FT_IRC_SOCKET_HPP
