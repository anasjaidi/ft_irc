//
// Created by Zakaria Maziane on 4/14/23.
//

#include "../Commands.hpp"

void Commands::bot(std::string payload, int client_fd, std::vector<client>::iterator theclient) {
    std::string msg;
    std::vector<client>::iterator it = clients.begin();
    if(payload == "Error"){
        msg = ":localhost not enough parameters\r\n";
        send(client_fd,msg.c_str(), msg.size(),0);
        return;
    }
    else {
        for (; it != this->clients.end(); it++) {
            msg =  "Nick client: " + it->getNick() + "\r\n";
            send(client_fd,msg.c_str(),msg.size(),0);
        }
    }

}

std::string Commands::parse_bot_command(std::string &req, std::vector<client>::iterator theclient) {
    trim_fun(req);
    std::vector<std::string> cmd = split(req, ' ');
    if(cmd.size() != 1)
        return ("Error");
    return (req);
}