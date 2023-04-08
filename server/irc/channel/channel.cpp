//
// Created by mustapha ouarsass on 4/5/23.
//

#include "channel.hpp"

channel::channel(){}
channel::~channel(){}

int     createChannel(Server *server, std::vector<std::string> cmd, int fd)
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
    msg = ":" +server->clientMap[fd].clientInformations() + " JOIN " + cmd[1] + "\r\n" // TODO : fix client_info() whith roben
          + ":loclahost" + " MODE " + cmd[1] + " +nt\r\n"
          + ":localhost" + " 353 " + server->clientMap[fd].getNick() + " = " + cmd[1] + " :@" + server->clientMap[fd].getNick() + "\r\n"
          + ":localhost" + " 366 " + server->clientMap[fd].getNick() + " " + cmd[1] + " :End of /NAMES list\r\n";
    send(fd, msg.c_str(), msg.size(), 0);
    return (EXIT_SUCCESS);

}
