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

std::string channel::clientInformationsForChannel(struct join_client_info infos) {
    struct sockaddr_in *i  = (struct sockaddr_in*)(infos.info);
    std::string ip;
    std::stringstream ss;
    ss << i->sin_addr.s_addr;
    ss >> ip;
    return (infos.nick + "!" + infos.user + "@" + ip);
}


void channel::delete_client(int client_fd, char z) {
    t_join_client infos;
    std::string msg;
    std::vector<int>::iterator mumber = this->members.begin();
    for (; mumber != this->members.end(); mumber++){
        if(*mumber == this->members[client_fd])
            this->members.erase(mumber);
    }
    std::vector<int>::iterator operate = this->operators.begin();
    for (; operate != this->operators.end(); operate++) {
        if (*operate == this->operators[client_fd])
            this->operators.erase(operate);
    }
    std::vector<int>::iterator allFds = this->fdsChannel.begin();
    for (; allFds != this->fdsChannel.end(); allFds++) {
        if (*allFds == this->fdsChannel[client_fd])
            this->fdsChannel.erase(allFds);
    }
    if(z == 'k') {
        /// send message for one;
        msg = ":" + clientInformationsForChannel(infos) + " PART " + this->name + "\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
    ///// here i send message to all mumbers is this dude kicked in the roome
    msg.clear();
    msg = clientInformationsForChannel(infos) + " PART " + this->name + "\\r\n";
    allFds = this->fdsChannel.begin();
        for(; allFds != this->fdsChannel.end(); allFds++)
        {
            send(*allFds, msg.c_str(),msg.size(), 0);
        }
    }
    if(z == 'b')
    {
        /// send message for all mumbers in this channel
        msg = ":localehost "+ this->name + " +b " + clientInformationsForChannel(infos) + " has been banned from this channel\r\n";
        allFds = this->fdsChannel.begin();
        for(; allFds != this->fdsChannel.end(); allFds++)
        {
            send(*allFds, msg.c_str(),msg.size(), 0);
        }
    }

}

bool channel::itIsInChannel(int client_fd) {
    std::vector<int>::iterator it = this->fdsChannel.begin();
    for(; it != this->fdsChannel.end(); it++) {
        if(this->fdsChannel[client_fd] == client_fd)
            return true;
    }
    return false;
}

void channel::AddToinvited(int client_fd)
{
    invited.push_back(client_fd);
}