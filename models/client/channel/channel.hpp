
//
// Created by mustapha ouarsass on 4/9/23.
//

#ifndef FT_IRC_CHANNEL_H
#define FT_IRC_CHANNEL_H


# include "../../../FT_IRC.h"


enum MODES : char {
    PRIVACY_ENABLED = 0b00000001,
    MESSAGE_BLOCKING = 0b00000010,
    VISIBILITY_ENABLED = 0b00000100,
    TOPIC_ENABLED = 0b00001000,
    PUBLIC_ENABLED = 0b00010000,
};

class channel {
private:
    std::string name;
    std::string password;

    std::vector<int> members;
    std::vector<int> operators;
    int memberLimit;
    bool key;

    std::vector<int> fdsChannel;
    char modes;
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



    /**
     * @mode i
     *
     * @details control channel privacy
     * */

    /**
     *
     * @mode m
     *
     * @details control message blocking
     * */

    /**
     *
     * @modes p, s
     *
     * @details control channel visibility
     * */

    /**
     *
     * @mode t
     *
     * @details control topic enabling
     * */

    /**
     *
     * @mode b
     *
     * @arg user nick
     *
     * @details  ban a user
     * */

    /**
     *
     * @mode o
     *
     * @arg user nick
     *
     * @details add user as admin
     * */

    /**
     *
     * @mode v
     *
     * @arg user nick
     *
     * @details permit user to talk besides the admins when chat is blocked
     * */

    /**
     *
     * @mode k
     *
     * @arg control password
     * */

    /**
     *
     * @mode l
     *
     * @arg new limit
     *
     * @details control channel limit
     * */

    /**
     *
     * @mode n
     *
     * @details make channel public
     *
     * */



};


#endif //FT_IRC_CHANNEL_H
