//
// Created by mustapha ouarsass on 4/9/23.
//

#include "channel_manager.hpp"

#include <sstream>




std::string clientInformations(struct join_client_info infos) {
    struct sockaddr_in *i  = (struct sockaddr_in*)(infos.info);
    std::string ip;
    std::stringstream ss;
    ss << i->sin_addr.s_addr;
    ss >> ip;
    return (infos.nick + "!" + infos.user + "@" + ip);
}

int channel_manager::create_channel(int client_fd, std::string ch,  t_join_client infos, std::string key)
{
    if (key != "@notfound@")
    {
        std::cout << "first" << std::endl;

    channels.push_back(channel(ch, key, 42, client_fd));

    }
    else
    {
        std::cout << "second" << std::endl;
    channels.push_back(channel(ch, 42, client_fd));

    }

    std::string msg;
    msg = ":" + clientInformations(infos) + " JOIN " + ch + "\r\n"
          + ":loclahost" + " MODE " + ch + " +nt\r\n"
          + ":localhost" + " 353 " + infos.nick + " = " + ch + " :@" + infos.nick + "\r\n"
          + ":localhost" + " 366 " + infos.nick + " " +  ch + " :End of /NAMES list\r\n";
    send(client_fd, msg.c_str(), msg.size(), 0);

    return (EXIT_SUCCESS);


}

std::vector<channel>::iterator channel_manager::get_channel_by_name(std::string &name) {
    std::vector<channel>::iterator  it = channels.begin();

    for (; it != channels.end(); it++) {
        if (it->getName() == name) {
            return it;
        }
    }
    return channels.end();
}

int channel_manager::add_to_exist(channel &ch, std::string key, int client_fd, t_join_client infos) {
    std::cout<< "adding to channel: " << ch.getName() << std::endl;

    if (ch.check_if_already_memebr(client_fd)) {
        std::string msg = "443 * " + infos.nick + " " + ch.getName() + " :is already on channel\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return 1;
    }
    if (ch.check_if_banned(client_fd))
    {
       std::string msgError = ":localhost 474 " + ch.getName() + " " + infos.nick + ":Cannot join channel (+b)\r\n";
        send(client_fd, msgError.c_str(), msgError.size(), 0);
        return 1;
    }
    ch.add_to_channel(client_fd);
    send(client_fd, "anas jaidi", 10, 0);
}

void channel_manager::render_channels() {
    std::cout<< "channels length: " << channels.size() << std::endl;
    for (channel ch : channels) {
        std::cout  << "name: " << ch.getName() << " | " << "pass: " << ch.getPassword() << std::endl;
    }
}
