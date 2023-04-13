//
// Created by Zakaria Maziane on 4/13/23.
//
#include "../Commands.hpp"

void Commands::kick(std::string payload, int client_fd) {

    std::string msg;
    std::vector<std::string> cmd = split(payload, '|');
    std::vector<std::string> users = split(cmd[1], '$');
    std::vector<channel>::iterator itChan = channels.begin();
    for (; itChan != channels.end(); itChan++) {
        if(itChan->getName() == cmd[0])

    }
    msg = ":localhost 401 " + cmd[0] + " : No such nick/channel\r\n"



}

std::string Commands::parse_kick_command(std::string &req) {
     std::string payload;
     std::vector<std::string> cmd = split(req, ' ');
     std::string channelName = cmd[1];
     std::vector<std::string> users  = split(cmd[2], ',');
     if (cmd.size() != 3 && channelName[0] != '#')
         return  "Error"; // rpl = ":localhost 401 " + cmd[1] + " : No such nick/channel\r\n";

     else{
        payload = channelName + '|';
        payload += joinByMe(users, '$');
     }
     std::cout << payload <<std::endl;
    return (payload);
}