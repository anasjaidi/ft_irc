//
// Created by mustapha ouarsass on 4/9/23.
//

#include "channel.hpp"

void channel::setName(const std::string &name) {
    channel::name = name;
}

const std::string &channel::getName() const {
    return name;
}
void channel::setKey(const bool &key) {
    channel::key = key;
}
bool channel::getKey() const {
    return key;
}

void channel::setPass(const std::string &pass) {
    channel::password = pass;
}

const std::string &channel::getPass() const {
    return password;
}