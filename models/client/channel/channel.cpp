//
// Created by mustapha ouarsass on 4/9/23.
//

#include "channel.hpp"
# include "../../../FT_IRC.h"


const std::vector<int>& channel::get_memebers_fds() {
    return this->members;
}
const std::vector<int>& channel::get_admins_fds() {
    return this->operators;
}
const std::vector<int>& channel::get_all_fds() {
    return this->fdsChannel;
}

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

channel::channel(const std::string &name, const std::string &password, int memberLimit, int client_fd) : name(name),
                                                                                                         password(
                                                                                                                 password),
                                                                                                         memberLimit(
                                                                                                                 memberLimit) {
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
    std::vector<int>::iterator it = members.begin();

    for (; it != members.end(); it++) {
        if (*it == client_fd) {
            return true;
        }
    }
    return false;
}

bool channel::check_if_banned(int client_fd) {
    std::vector<int>::iterator it = bans.begin();

    for (; it != bans.end(); it++) {
        if (*it == client_fd) {
            return true;
        }
    }
    return false;
}

std::string channel::clientInformationsForChannel(struct join_client_info infos) {
    struct sockaddr_in *i = (struct sockaddr_in *) (infos.info);
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
    for (int i = 0; i < this->members.size(); i++){
        if(this->members[i] == client_fd)
            this->members.erase(this->members.begin() + i);
    }
    std::vector<int>::iterator operate = this->operators.begin();
    for (int i = 0; i < this->operators.size(); i++) {
        if (this->operators[i] == client_fd)
            this->operators.erase(this->operators.begin() + i);
    }
    std::vector<int>::iterator allFds = this->fdsChannel.begin();
    for (int i = 0; i < this->fdsChannel.size(); i++) {
        if (this->fdsChannel[i] == client_fd)
            this->fdsChannel.erase(this->fdsChannel.begin() + i);
    }
    if (z == 'k') {
        /// send message for one;
        msg = ":" + clientInformationsForChannel(infos) + " PART " + this->name + "\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
    ///// here i send message to all mumbers is this dude kicked in the roome
    msg.clear();
    msg = clientInformationsForChannel(infos) + " PART " + this->name + "\r\n";
    allFds = this->fdsChannel.begin();
        for(; allFds != this->fdsChannel.end(); allFds++)
        {
            send(*allFds, msg.c_str(),msg.size(), 0);
        }
    }
    if (z == 'b') {
        /// send message for all mumbers in this channel
        msg = ":localehost " + this->name + " +b " + clientInformationsForChannel(infos) +
              " has been banned from this channel\r\n";
        allFds = this->fdsChannel.begin();
        for (; allFds != this->fdsChannel.end(); allFds++) {
            send(*allFds, msg.c_str(), msg.size(), 0);
        }
    }

}

void channel::send_msg_to_all_members( int client_fd, std::string kicked, std::string channel) {
    std::string msg;
    msg = ":" + kicked + " is  KICKED FROM " + channel + "\r\n";
    std::vector<int>::iterator allFds = this->fdsChannel.begin();
    allFds = this->fdsChannel.begin();
        for (; allFds != this->fdsChannel.end(); allFds++) {
            send(*allFds, msg.c_str(), msg.size(), 0);
            }
}


bool channel::itIsInChannel(int client_fd) {
    std::vector<int>::iterator it = this->fdsChannel.begin();

    for(; it != this->fdsChannel.end(); it++) {
        if(*it == client_fd)
            return true;
    }
    return false;
}


bool channel::adminChannel(int fd) {
    std::vector<int>::iterator it = this->operators.begin();
    for (; it != this->operators.end(); it++) {
        if(*it == fd)
            return true;
    }
    return false;
}
void channel::AddToinvited(std::string nick_name)
{
    invited.push_back(nick_name);
}

int channel::privacy_mode_handler(bool on) {
    if (on)
        modes |= PRIVACY_ENABLED;
    else
        modes &= ~PRIVACY_ENABLED;
    std::cout << "privacy mode: is" << (modes & PRIVACY_ENABLED ? "exists" : "not exists")<< std::endl;
    return 0;
}

int channel::message_blocking_mode_handler(bool on) {
    if (on)
        modes |= MESSAGE_BLOCKING;
    else
        modes &= ~MESSAGE_BLOCKING;
    std::cout << "message blocking mode: is" << (modes & MESSAGE_BLOCKING ? "exists" : "not exists")<< std::endl;
    return 0;
}

int channel::channel_visibility_mode_handler(bool on) {
    if (on)
        modes |= VISIBILITY_ENABLED;
    else
        modes &= ~VISIBILITY_ENABLED;
    std::cout << "visibility mode: is" << (modes & VISIBILITY_ENABLED ? "exists" : "not exists")<< std::endl;
    return 0;
}

int channel::channel_topic_mode_handler(bool on) {
    if (on)
        modes |= TOPIC_ENABLED;
    else
        modes &= ~TOPIC_ENABLED;
    std::cout << "topic mode: is" << (modes & TOPIC_ENABLED ? "exists" : "not exists")<< std::endl;
    return 0;
}

int channel::public_mode_handler(bool on) {
    if (on)
        modes |= PUBLIC_ENABLED;
    else
        modes &= ~PUBLIC_ENABLED;
    std::cout << "public mode: is" << (modes & PUBLIC_ENABLED ? "exists" : "not exists") << std::endl;
    return 0;
}

int channel::limit_mode_handler(bool on, int new_limit) {
    if (on)
        this->memberLimit = new_limit;
    else
        this->memberLimit = -1;
    std::cout << "limit mode: " << this->memberLimit << std::endl;
    return this->memberLimit;
}

std::string channel::pass_mode_handler(bool on, std::string &newPass) {
    if (on)
        this->password = newPass;
    else
        this->password = "";
    std::cout << "pass mode: " << this->password << std::endl;
    return this->password;
}

int channel::operator_friend_mode_handler(bool on, int client_fd) {
    if (on) {
        if (std::find(operator_friends.begin(), operator_friends.end(), client_fd) != operator_friends.end()) return 1;
        operator_friends.push_back(client_fd);
    } else {
        if (std::find(operator_friends.begin(), operator_friends.end(), client_fd) == operator_friends.end()) return 1;
        operator_friends.erase(std::remove(operator_friends.begin(), operator_friends.end(), client_fd));
    }
    std::cout << "friend mode: " << (
            std::find(operator_friends.begin(), operator_friends.end(), client_fd) == operator_friends.end() ?
            "not exits" : "exists"
    ) << std::endl;
    return 0;
}

int channel::operator_mode_handler(bool on, int client_fd) {
    if (on) {
        if (std::find(operators.begin(), operators.end(), client_fd) != operators.end()) return 1;
        operators.push_back(client_fd);
    } else {
        if (std::find(operators.begin(), operators.end(), client_fd) == operators.end()) return 1;
        operators.erase(std::remove(operators.begin(), operators.end(), client_fd));
    }
    std::cout << "operator mode: " << (
            std::find(operators.begin(), operators.end(), client_fd) == operators.end() ?
            "not exits" : "exists"
    ) << std::endl;
    return 0;
}

int channel::ban_mode_handler(bool on, int client_fd) {
    std::cout << "client_fd : " << bans.size() << std::endl;

    std::cout << "founded: " << ((bool)(std::find(bans.begin(), bans.end(), client_fd) == bans.end()));
    if (on) {
        if (std::find(bans.begin(), bans.end(), client_fd) != bans.end()) {
            std::cout << "yes founded\n";
            return 1;
        }
        std::cout << "yes pushed\n";
        bans.push_back(client_fd);
        std::cout << bans[0] << std::endl;
        return 0;
    } else {
        if (std::find(bans.begin(), bans.end(), client_fd) == bans.end()) {
            std::cout << "yes not founded\n";
            return 1;
        }
        bans.erase(std::remove(bans.begin(), bans.end(), client_fd));
    }
    std::cout << "ban mode: " << (
            std::find(bans.begin(), bans.end(), client_fd) == bans.end() ?
            "not exits" : "exists"
    ) << std::endl;
    return 0;
}


void channel::broadcast_message(std::string &msg) {
    if (modes & MESSAGE_BLOCKING) {
        for (int i = 0; i < this->operators.size(); ++i) {
            send(operators[i] , msg.c_str(), msg.length(), 0);
        }for (int i = 0; i < this->operator_friends.size(); ++i) {
            send(operator_friends[i] , msg.c_str(), msg.length(), 0);
        }
    } else {
        for (int i = 0; i < this->fdsChannel.size(); ++i) {
            send(fdsChannel[i],  msg.c_str(), msg.length(), 0);
        }
    }
}