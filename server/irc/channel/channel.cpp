//
// Created by mustapha ouarsass on 4/5/23.
//

#include "channel.hpp"

channel::channel(){}
channel::~channel(){}

int     channel::createChannel(Server *server, std::vector<std::string> cmd, int fd)
{
    channel set;
    if (cmd.size() > 2)
    {
        set.name = cmd[1];
        set.password = cmd[2];
        set.key = true;
        set.mumberLimited = 42;
        set.fds_channel.push_back(fd);
        server->clientMap[fd].idChannels.push_back(server->serIdChannel);
        server->clientMap[fd].nameChannels.push_back(cmd[1]);

    }
    else
    {
        set.name = cmd[1];
        set.key = false;
        set.fds_channel.push_back(fd);
        set.mumberLimited = 42;
        server->clientMap[fd].idChannels.push_back(server->serIdChannel);
        server->clientMap[fd].nameChannels.push_back(cmd[1]);

    }
    std::string msg;
    server->mapChannel.insert(std::make_pair(cmd[1], set));
    server->channelNames.push_back(set.name);
    server->serIdChannel++;

}
