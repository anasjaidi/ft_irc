//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_CLIENT_HPP
#define FT_IRC_CLIENT_HPP
#include "../../FT_IRC.h"

class client {
private:
    std::string user;
    std::string nick;
    int fd;
    struct sockaddr_storage their_addr;
public:
    int getFd() const;

    void setFd(int fd);

    const sockaddr_storage &getTheirAddr() const;

    void setTheirAddr(const sockaddr_storage &theirAddr);

    client(int fd, const sockaddr_storage &theirAddr);

public:
    const std::string &getUser() const;

    void setUser(const std::string &user);

    const std::string &getNick() const;

    void setNick(const std::string &nick);

    client(const std::string &user, const std::string &nick);

public:
    virtual ~client();

    std::string client_info();

    client();
    std::vector<int> idChannels;
    std::vector<std::string> nameChannels;
};


#endif //FT_IRC_CLIENT_HPP
