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
    int pass(std::string, int, std::string, std::vector<client>::iterator);
    int nick(std::string, int, std::vector<client>::iterator);
    int user(std::string, int, std::vector<client>::iterator);
    void part(std::string, int, std::vector<client>::iterator);
    void join(std::string, int, t_join_client infos, std::vector<client>::iterator);
    void mode(std::string,int, t_join_client infos);
    void kick(std::string, int, std::vector<client>::iterator);
    void invite(std::string,int, std::vector<client>::iterator);
    void bot(std::string, int client_fd, std::vector<client>::iterator);
    void privmsg(std::string, int client_fd, std::vector<client>::iterator);

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

    std::pair<OptionCommands, std::string > get_command(std::string &request, int client_fd, std::vector<client>::iterator);

    std::string parse_pass_command(std::string &, int client_fd, std::vector<client>::iterator);
    std::string parse_nick_command(std::string &, int client_fd, std::vector<client>::iterator);
    std::string parse_user_command(std::string &, int client_fd, std::vector<client>::iterator);
    std::string parse_join_command(std::string &req, int client_fd, std::vector<client>::iterator);
    std::string parse_mode_command(std::string &req, int client_fd, std::vector<client>::iterator);
    std::string parse_part_command(std::string &, int client_fd, std::vector<client>::iterator);
    std::string parse_kick_command(std::string &req, std::vector<client>::iterator theclient);
    std::string parse_invite_command(std::string &req, std::vector<client>::iterator theclient);
    std::string parse_bot_command(std::string &req, std::vector<client>::iterator theclient);
    std::string parse_privmsg_command(std::string &req, int client_fd, std::vector<client>::iterator);
};

std::vector<std::string> split(std::string line, char c);
std::string joinByMe(std::vector<std::string> &vec, char c);
void trim_fun(std::string &str);







std::vector<std::string> split(std::string line, char c);
std::string joinByMe(std::vector<std::string> &vec, char c);


#endif //FT_IRC_COMMANDS_HPP
