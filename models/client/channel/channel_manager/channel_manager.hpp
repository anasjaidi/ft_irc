//
// Created by mustapha ouarsass on 4/9/23.
//

#ifndef FT_IRC_CHANNEL_MANAGER_HPP
#define FT_IRC_CHANNEL_MANAGER_HPP


# include "../../../../FT_IRC.h"
#include "../channel.hpp"

class channel_manager {
protected:
    std::vector<channel> channels;
public:
    int create_channel(int client_fd, std::string channel, std::string key = std::string("@notfound@"));
};


#endif //FT_IRC_CHANNEL_MANAGER_HPP
