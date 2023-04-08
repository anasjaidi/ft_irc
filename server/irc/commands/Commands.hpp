//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_COMMANDS_HPP
#define FT_IRC_COMMANDS_HPP

# include "../../../FT_IRC.h"
#include "../IrcServer.hpp"
# include "../../../models/client/client_manager/client_manager.hpp"
class Server;

class Commands : public client_manager{
public:
    void pass(std::string, int, std::string);
    void nick(std::string, int);
    void user(std::string, int);
    void who(std::string, int);
    void part(std::string, int);
    void join(std::string, int);



    enum OptionCommands  {
        PASS,
        NICK,
        USER,
        QUIT,
        ADMIN,
        WHO,
        PRIVATE_MSG,
        NOTICE,
        PART,
        MODE,
        INVITE,
        KICK,
        BOT,
        JOIN,
        UNDEFINED
    };

    std::pair<OptionCommands, std::string > get_command(std::string &request, int client_fd);
    std::vector<std::string> split(std::string line, char c);

    std::string parse_pass_command(std::string &);
    std::string parse_nick_command(std::string &);
    std::string parse_user_command(std::string &);
    std::string parse_who_command(std::string &);
    std::string parse_part_command(std::string &);
    std::string parse_join_command(std::string &, int);

};


#endif //FT_IRC_COMMANDS_HPP
