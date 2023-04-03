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



    enum OptionCommands  {
        PASS,
        NICK,
        USER,
        QUIT,
        ADMIN,
        TIME,
        WHO,
        PRIVATE_MSG,
        NOTICE,
        NAMES,
        PART,
        MODE,
        INVITE,
        KICK,
        BOT,
        JOIN,
        UNDEFINED
    };

    std::pair<OptionCommands, std::string > get_command(std::string &request);

    std::string parse_pass_command(std::string &);
    std::string parse_nick_command(std::string &);
    std::string parse_user_command(std::string &);
};


#endif //FT_IRC_COMMANDS_HPP
