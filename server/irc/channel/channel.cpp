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
        set.fds_channel = push_back(fd);
    }
    else
    {

    }
}