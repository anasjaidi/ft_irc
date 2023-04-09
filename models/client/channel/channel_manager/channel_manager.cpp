//
// Created by mustapha ouarsass on 4/9/23.
//

#include "channel_manager.hpp"

int channel_manager::create_channel(int client_fd, std::string channel, std::string key)
{
    if (key != "@notfound@")
    {
        channels. = channel; //TODO here is not completed
        channels.password = key;
        channels.setKey(key) = true;
        channels.mumberLimited = 42;
        channels.fds_channel.push_back(fd);
        channels.operators.push_back(fd);
        server->clientMap[fd].idChannels.push_back(server->serIdChannel);
        server->clientMap[fd].nameChannels.push_back(cmd[1]);

    }
    else
    {
        set.name = cmd[1];
        set.key = false;
        set.operators.push_back(fd);
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