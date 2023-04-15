//
// Created by Zakaria Maziane on 4/13/23.
//
#include "../Commands.hpp"

void Commands::kick(std::string payload, int client_fd) {
//payload = #ch|med
//
    int fdcl = -1;
    std::string msg;
    std::vector<std::string> cmd = split(payload, '|');

    //set iterators of channels,clients

    std::vector<channel>::iterator itChan = channels.begin();
    std::vector<client>::iterator itclient = clients.begin();

    // check error parsing

    if(payload == "No such nick/channel"){
        msg = "-> :localhost 401 " + cmd[1] + " : No such nick/channel\r\n";
        send(client_fd, msg.c_str(),msg.size(),0);
        return;
    }

    // loop to find the current channel

    for (; itChan != channels.end(); itChan++) {

        std::string name = itChan->getName();
        trim_fun(name);

        // check name of channel

        if(name == cmd[0]){
            std::string clName = itclient->getNick();
            trim_fun(clName);
            int i= 0;

            //check nick client ,check is the current one and get his file descriptor
            if(!cmd[1].empty()){
                for(; itclient != clients.end(); itclient++) {
                    if(clName == cmd[1]){
                        fdcl = itclient->getFd();

                        // check if kicker is operator

                        if(this->isOperator(client_fd ,itChan)){
                            if(itChan->itIsInChannel(fdcl)){
                                itChan->delete_client(fdcl, 'k');
                                msg = cmd[1] + "is kicked\r\n";
                                send(client_fd, msg.c_str(), msg.size(), 0);
                            }
                            msg = ":localhost 441 " + cmd[1]  + " :isn't on that channel\r\n";
                            send(client_fd, msg.c_str(),msg.size(), 0);
                            return;
                        }
                        msg = ":localhost 482 " + itclient->getNick() + ":You're not channel operator\r\n";
                        send(client_fd, msg.c_str(),msg.size(), 0);
                        return;
                    }
                }
            }
        }
        msg = "-> :localhost 401 " + cmd[1] + " : No such nick/channel\r\n";
        send(client_fd,msg.c_str(),msg.size(),0);
        }
    }










std::string Commands::parse_kick_command(std::string &req) {
    bool Nk = false;
     std::string payload;
     std::vector<std::string> cmd = split(req, ' ');
     std::string channelName = cmd[1];
    trim_fun(channelName);
//     std::vector<std::string> users  = split(cmd[2], ',');
     std::string user = cmd[2];
    trim_fun(user);
    std::vector<client>::iterator itclient = clients.begin();

     if (cmd.size() != 3 && channelName[0] != '#')
         return  "No such nick/channel"; // rpl = ":localhost 401 " + cmd[1] + " : No such nick/channel\r\n";
     else if(!user.empty()){
         for(;itclient != clients.end(); itclient++){
             std::string clNick = itclient->getNick();
             trim_fun(clNick);
             if(user == clNick)
                 Nk = true;
         }
         if(Nk == false)
             return"No such nick/channel"; // rpl = ":localhost 401 " + cmd[1] + " : No such nick/channel\r\n";

         }
     else{
        payload = channelName + '|';
        payload += user;
     }
     std::cout << payload <<std::endl;
    return (payload);
}



void trim_fun(std::string &str) {
    str = str.substr(str.find_first_not_of(" \t\f\v\n\r"));
    str = str.substr(0, str.find_last_not_of(" \t\f\v\n\r") + 1);
}