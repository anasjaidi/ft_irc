//
// Created by Zakaria Maziane on 4/13/23.
//
#include "../Commands.hpp"

void Commands::kick(std::string payload, int client_fd) {

    int fdcl = -1;
    std::string msg;
    std::vector<std::string> cmd = split(payload, '|');
    std::vector<std::string> users = split(cmd[1], '$');
    std::vector<channel>::iterator itChan = channels.begin();
    std::vector<client>::iterator itclient = clients.begin();
    for (; itChan != channels.end(); itChan++) {



        if(itChan->getName() == cmd[0]){

            for(; itclient != clients.end(); itclient++) {
                int i= 0;
                while(i < users.size()){
                    if(itclient->getNick() == users[i]){

                        fdcl = itclient->getFd();
                    std::cout << "fdClient :" << fdcl;
                    }
                    i++;
                }
                std::cout << "fdClient :" << fdcl << std::endl;

                if(fdcl == -1){
                    msg = "-> :localhost 401 " + users[i] + " : No such nick/channel\r\n";
                    send(client_fd,msg.c_str(),msg.size(),0);
                }
                if(!this->isOperator(client_fd ,itChan)){
                    msg = ":localhost 482 " + itclient->getNick() + ":You're not channel operator\r\n";
                    send(client_fd, msg.c_str(),msg.size(), 0);
                }
                if(itChan->itIsInChannel(fdcl))
                    itChan->delete_client(fdcl, 'k');
                else{
                    msg = ":localhost 441 " + users[i]  + " :isn't on that channel\r\n";
                    send(client_fd, msg.c_str(),msg.size(), 0);
                    return;
                }
                    // error

            }

        }

    }
    msg = ":localhost 401 " + cmd[0] + " : No such nick/channel\r\n";
    send(client_fd, msg.c_str(),msg.size(), 0);
    return;



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