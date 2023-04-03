//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_COMMANDS_HPP
#define FT_IRC_COMMANDS_HPP

# include "../../../FT_IRC.h"

# include "../../../models/client/client_manager/client_manager.hpp"


class Commands : public client_manager{
public:
    void pass(std::string, int, std::string);
    void nick(std::string, int);
    void user(std::string, int);
    //////// QUIT - ADMIN - TIME - WHO - PRIVMSG - NOTICE - NAMES - PART - MODE - KICK - INVITE - BOT - JOIN


};


#endif //FT_IRC_COMMANDS_HPP
