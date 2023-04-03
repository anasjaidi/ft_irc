//
// Created by anas jaidi on 30/3/2023.
//

#include "client.hpp"

client::~client() {

}

client::client(const std::string &user, const std::string &nick) : user(user), nick(nick) {}

const std::string &client::getUser() const {
    return user;
}

void client::setUser(const std::string &user) {
    client::user = user;
}

const std::string &client::getNick() const {
    return nick;
}

void client::setNick(const std::string &nick) {
    client::nick = nick;
}

client::client() {}

client::client(int fd, const sockaddr_storage &theirAddr) : fd(fd), their_addr(theirAddr) {}
