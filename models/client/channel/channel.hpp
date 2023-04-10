
//
// Created by mustapha ouarsass on 4/9/23.
//

#ifndef FT_IRC_CHANNEL_H
#define FT_IRC_CHANNEL_H


# include "../../../FT_IRC.h"
#include "channel_manager/channel_manager.hpp"


class channel {
private:
    std::string name;
    std::string password;
    std::vector<int> mumbers;
    std::vector<int> operators;
    int mumberLimited;
    bool key;

    std::vector<int> fdsChannel;
    std::vector<char> modes;
    std::vector<unsigned int> bans;

public:
    void setName(const std::string &name);

    const std::string &getName() const;

    void setKey(const bool &key);

    bool getKey() const;

    void setPass(const std::string &pass);

    const std::string &getPass() const;
};


#endif //FT_IRC_CHANNEL_H
