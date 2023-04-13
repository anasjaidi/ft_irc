//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_COMMANDS_HPP
#define FT_IRC_COMMANDS_HPP

# include "../../../FT_IRC.h"

# include "../../../models/client/client_manager/client_manager.hpp"
#include "../../../models/client/channel/channel_manager/channel_manager.hpp"


class Commands : public client_manager, public channel_manager{
public:
    void pass(std::string, int, std::string);
    void nick(std::string, int);
    void user(std::string, int);
    void part(std::string, int);
    void join(std::string, int, t_join_client infos);
    void mode(std::string,int, t_join_client infos);
    void kick(std::string, int);

    enum OptionCommands  {
        PASS, // valide
        NICK, // valide
        USER, // valide
        PRIVATE_MSG,
        NOTICE,
        PART,
        MODE,
        INVITE,
        KICK,
        BOT,
        JOIN, // valide
        KILL,
        UNDEFINED
    };

    std::pair<OptionCommands, std::string > get_command(std::string &request);

    std::string parse_pass_command(std::string &);
    std::string parse_nick_command(std::string &);
    std::string parse_user_command(std::string &);
    std::string parse_join_command(std::string &req);
    std::string parse_mode_command(std::string &req);
    std::string parse_part_command(std::string &);
    std::string parse_kick_command(std::string &);
};
std::vector<std::string> split(std::string line, char c);
std::string joinByMe(std::vector<std::string> &vec, char c);


#endif //FT_IRC_COMMANDS_HPP
