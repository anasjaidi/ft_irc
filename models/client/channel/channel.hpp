
//
// Created by mustapha ouarsass on 4/9/23.
//

#ifndef FT_IRC_CHANNEL_H
#define FT_IRC_CHANNEL_H


# include "../../../FT_IRC.h"


class channel {
private:
    std::string name;
    std::string password;

    std::vector<int> members;
    std::vector<int> operators;
    int memberLimit;
    bool key;

    std::vector<int> fdsChannel;
    long long modes;
    std::vector<int> bans;

public:
    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getPassword() const;

    channel(const std::string &name, const std::string &password, int memberLimit,  int client_fd);

    channel(const std::string &name, int memberLimit, int client_fd);

    void setPassword(const std::string &password);

    int getMemberLimit() const;

    void setMemberLimit(int memberLimit);

    bool isKey() const;

    void setKey(bool key);

    long long int getModes() const;

    void setModes(long long int modes);


    void add_to_channel(int client_fd);

    bool check_if_banned(int client_fd);

    bool check_if_already_memebr(int client_fd);

};


#endif //FT_IRC_CHANNEL_H
