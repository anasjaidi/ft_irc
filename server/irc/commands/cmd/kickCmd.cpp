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

        std::string name = itChan->getName();
        trim_fun(name);

        if(name == cmd[0]){
            std::string clName = itclient->getNick();
            trim_fun(clName);
            int i= 0;
            while(i < users.size()){
                for(; itclient != clients.end(); itclient++) {
                    if(clName == users[i]){
                        fdcl = itclient->getFd();
                        if(this->isOperator(client_fd ,itChan)){
                            if(itChan->itIsInChannel(fdcl)){
                                itChan->delete_client(fdcl, 'k');
                                msg = users[i] + "is kicked\r\n";
                                send(client_fd, msg.c_str(), msg.size(), 0);
                            }
                            msg = ":localhost 441 " + users[i]  + " :isn't on that channel\r\n";
                            send(client_fd, msg.c_str(),msg.size(), 0);
                            return;
                        }
                        msg = ":localhost 482 " + itclient->getNick() + ":You're not channel operator\r\n";
                        send(client_fd, msg.c_str(),msg.size(), 0);
                        return;
                    }
                        std::cout << "fdClient :" << fdcl;
                    }
                }
                i++;
            }
        msg = "-> :localhost 401 " + users[0] + " : No such nick/channel\r\n";
        send(client_fd,msg.c_str(),msg.size(),0);
        }
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



void trim_fun(std::string &str) {
    str = str.substr(str.find_first_not_of(" \t\f\v\n\r"));
    str = str.substr(0, str.find_last_not_of(" \t\f\v\n\r") + 1);
}