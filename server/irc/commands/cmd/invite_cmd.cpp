//
// Created by mustapha ouarsass on 4/13/23.
//

# include "../Commands.hpp"



#include "../../../../FT_IRC.h"


std::string Commands::parse_invite_command(std::string &req)
{
    req.erase(0, 6);

    std::string payload;
    std::vector<std::string> cmd = split(req, ' ');
    std::cout << cmd[0] << " | " << cmd[1] << std::endl;
    if (cmd.size() != 2 && cmd[1][0] != '#')
        return "enough"; // not enough param
    else {
        payload = joinByMe(cmd, '|');
    }
    return (payload);
}

void Commands::invite(std::string payload, int client_fd)
{
    std::string msg;
    if (payload == "enough")
    {
        msg = ":localhost 461 " + clients[client_fd].getNick() + ": Not enough parameters \r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    std::vector<std::string> desirlize = split(payload, '|');
    // desirlize[0] = nickname
    // desirlize[1] = #channel
    std::vector<channel>::iterator ch_it = get_channel_by_name(desirlize[1]);
    //TODO here i have to check this channel if have it mode "i"=invite in my modes

    if (ch_it == channels.end())
    {
        msg.clear();
        msg =  ":localhost 401 " + desirlize[1] + " : No such nick/channel\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    else if (ch_it->getModes() & (PRIVACY_ENABLED))
    {

    }
    else if (!(ch_it->getModes() & (PRIVACY_ENABLED)))
    {
        // case error
        std::cout << "dont have invite mode" << std::endl;
    }
    else
    {
        if (ch_it->getName() == desirlize[0])
        {
            std::string nick_name = desirlize[0];
            ch_it->AddToinvited(nick_name);
        }
        msg.clear();
        msg = ":" + clients[client_fd].getNick() + " INVITE ========>" + desirlize[0] + " " + desirlize[1] + " :End of Channel Invite Exception List\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return ;
    }
}