//
// Created by mustapha ouarsass on 4/9/23.
//

#include "channel.hpp"

const std::string &channel::getName() const {
    return name;
}

void channel::setName(const std::string &name) {
    channel::name = name;
}

const std::string &channel::getPassword() const {
    return password;
}

void channel::setPassword(const std::string &password) {
    channel::password = password;
}

int channel::getMemberLimit() const {
    return memberLimit;
}

void channel::setMemberLimit(int memberLimit) {
    channel::memberLimit = memberLimit;
}

bool channel::isKey() const {
    return key;
}

void channel::setKey(bool key) {
    channel::key = key;
}

long long int channel::getModes() const {
    return modes;
}

void channel::setModes(long long int modes) {
    channel::modes = modes;
}

channel::channel(const std::string &name, const std::string &password, int memberLimit,  int client_fd) : name(name),  password(password),  memberLimit(memberLimit){
    fdsChannel.push_back(client_fd);
    operators.push_back(client_fd);
    key = true;
}

channel::channel(const std::string &name, int memberLimit, int client_fd) : name(name), memberLimit(memberLimit) {
    fdsChannel.push_back(client_fd);
    operators.push_back(client_fd);
    key = false;
}


void channel::add_to_channel(int client_fd) {
    members.push_back(client_fd);
    fdsChannel.push_back(client_fd);
}

bool channel::check_if_already_memebr(int client_fd) {
    std::vector<int>::iterator  it = members.begin();

    for (; it != members.end(); it++) {
        if (*it == client_fd) {
            return true;
        }
    }
    return false;
}

bool channel::check_if_banned(int client_fd) {
    std::vector<int>::iterator  it = bans.begin();

    for (; it != bans.end(); it++) {
        if (*it == client_fd) {
            return true;
        }
    }
    return false;
}