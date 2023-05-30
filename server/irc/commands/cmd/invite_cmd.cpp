/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite_cmd.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmaziane <zmaziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:56:09 by zmaziane          #+#    #+#             */
/*   Updated: 2023/05/30 19:08:34 by zmaziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by mustapha ouarsass on 4/13/23.
//

# include "../Commands.hpp"



#include "../../../../FT_IRC.h"


std::string Commands::parse_invite_command(std::string &req, std::vector<client>::iterator )
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

void Commands::invite(std::string payload, int client_fd, std::vector<client>::iterator )
{
    std::string msg;
    if (payload == "enough")
    {
        msg = ":localhost 461 " + clients[client_fd].getNick() + ": Not enough parameters \r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return ;
    }
    std::vector<std::string> desirlize = split(payload, '|');
    std::vector<channel>::iterator ch_it = get_channel_by_name(desirlize[1]);

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
        msg = ":" + clients[client_fd].getNick() + " INVITE " + desirlize[0] + " " + desirlize[1] + " :End of Channel Invite Exception List\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return ;
    }
}