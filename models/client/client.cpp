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

const std::string &client::getChannel() const {
    return channel;
}

void client::setChannel(const std::string &channel) {
    client::channel = channel;
}

client::client() {}

client::client(int fd, const sockaddr_storage &theirAddr) : fd(fd), their_addr(theirAddr) {}

int client::getFd() const {
    return fd;
}

void client::setFd(int fd) {
    client::fd = fd;
}

const sockaddr_storage &client::getTheirAddr() const {
    return their_addr;
}

void client::setTheirAddr(const sockaddr_storage &theirAddr) {
    their_addr = theirAddr;
}

std::string client::clientInformations()
{
    std::string ip;
    std::stringstream ss;
    ss << this->ipAddr.sin_addr.s_addr;
    ss >> ip;
    return (nick + "!" + user + "@" + ip);
}