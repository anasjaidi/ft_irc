//
// Created by mustapha ouarsass on 4/5/23.
//

#ifndef FT_IRC_CHANNEL_H
#define FT_IRC_CHANNEL_H

#include <iostream>
#include "../../../abstract/server/Server.hpp"
#include "../../../models/client/client_manager/client_manager.hpp"
#include "../commands/Commands.hpp"
#include <map>
#include "../../../FT_IRC.h"
#include <vector>
class Server;
class client_manager;
class channel
{
public:
    std::string name;
    std::string password;
    std::vector<int> mumbers;
    std::vector<int> operators;
    int mumberLimited;
    bool key;

    std::vector<int> fds_channel;
    std::vector<unsigned int> mumbers_banned;
    channel();
    ~channel();


};
    int     createChannel(Server *server, std::vector<std::string> cmd, int fd);

#endif //FT_IRC_CHANNEL_H
