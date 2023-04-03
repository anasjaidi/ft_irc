//
// Created by mustapha ouarsass on 4/3/23.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP

#include <iostream>

class channel {

public:
    std::string name;
    std::string password;
    std::string ChannelMembers;
    channel();
    ~channel();
    void creationChannel(std::string &);
};


#endif //FT_IRC_CHANNEL_H
