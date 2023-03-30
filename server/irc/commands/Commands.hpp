//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_COMMANDS_HPP
#define FT_IRC_COMMANDS_HPP

# include "../../../FT_IRC.h"


class Commands {
public:
    void pass(std::string);
    void nick(std::string);
    void user(std::string);
};


#endif //FT_IRC_COMMANDS_HPP
