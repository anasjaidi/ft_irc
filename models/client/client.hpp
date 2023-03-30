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
public:
    const std::string &getUser() const;

    void setUser(const std::string &user);

    const std::string &getNick() const;

    void setNick(const std::string &nick);

    client(const std::string &user, const std::string &nick);

public:
    virtual ~client();
};


#endif //FT_IRC_CLIENT_HPP
