//
// Created by Zakaria Maziane on 4/13/23.
//
#include "../Commands.hpp"

void Commands::kick(std::string payload, int client_fd, std::vector<client>::iterator theclient) {
//payload = #ch|med
//
    int fdcl;
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

        if(!cmd[0].empty() && name == cmd[0]){


            //check nick client ,check is the current one and get his file descriptor
            if(!cmd[1].empty()){
                for(; itclient != clients.end(); itclient++) {
                    std::string clName = itclient->getNick();
                    trim_fun(clName);
                    if(clName == cmd[1]){
                        fdcl = itclient->getFd();
                        // check if the kicker is operator
                        if(this->isOperator(client_fd ,itChan)){
                            if(itChan->itIsInChannel(fdcl)){
                                std::string kicker = get_client_Nick_by_Id(client_fd);
                                 itChan->send_msg_to_all_members(client_fd, cmd[1], cmd[0], kicker, theclient);
                                itChan->delete_client(fdcl, 'k');
//                                msg = ":" + cmd[1] + " is  KICKED FROM " + cmd[0] + "\r\n";
//                                send(client_fd, msg.c_str(), msg.size(), 0);
                                //->
                                /// here i send msg kiked to all mumbers in channel


                                //->
                                return;
                            }else
                                msg = ":localhost 441 " + cmd[1]  + " :isn't on that channel\r\n";
                                send(client_fd, msg.c_str(),msg.size(), 0);
                                return;
                        }else
                            msg = ":localhost 482 " + itclient->getNick() + ":You're not channel operator\r\n";
                            send(client_fd, msg.c_str(),msg.size(), 0);
                            return;
                    }
                }
            }
        }

    }
    msg = " :localhost 401" + cmd[0] + " : No such nick/channel\r\n";
    send(client_fd,msg.c_str(),msg.size(),0);
    return;
}










std::string Commands::parse_kick_command(std::string &req, std::vector<client>::iterator ) {
    bool Nk = false;
     std::string payload;
     std::vector<std::string> cmd = split(req, ' ');
     std::string channelName = cmd[1];
     std::string user = cmd[2];

     trim_fun(channelName);
     trim_fun(user);

    std::vector<client>::iterator itclient = clients.begin();

    std::cout << cmd[1] << "->" << cmd[2] <<std::endl;
     if (cmd.size() <= 3 || channelName[0] != '#')
         return  "No such nick/channel"; // rpl = ":localhost 401 " + cmd[1] + " : No such nick/channel\r\n";
     if(!user.empty())
     {
         for(;itclient != clients.end(); itclient++)
         {
             std::string clNick = itclient->getNick();
             trim_fun(clNick);
             if(user == clNick){
                 Nk = true;
                 break;
             }
         }
         if(Nk == false)
             return"No such nick/channel"; // rpl = ":localhost 401 " + cmd[1] + " : No such nick/channel\r\n";
     }
     if (Nk == true)
     {
        payload = channelName + "|";
        payload += user;
     }
     std::cout << payload <<std::endl;
     return (payload);
}



void trim_fun(std::string &str) {

    size_t start = str.find_first_not_of(" \t\f\v\n\r");
    size_t end = str.find_last_not_of(" \t\f\v\n\r");
    if (start == std::string::npos || end == std::string::npos) {
        str = std::string("");
        return ;
    }
    str = str.substr(start, end + 1);
}