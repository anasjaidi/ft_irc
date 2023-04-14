
//
// Created by mustapha ouarsass on 4/9/23.
//

#ifndef FT_IRC_CHANNEL_H
#define FT_IRC_CHANNEL_H


# include "../../../FT_IRC.h"
#include <sstream>

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
    std::vector<std::string> invited;

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

    void delete_client(int client_fd, char z);

    bool itIsInChannel(int client_fd);

    std::string clientInformationsForChannel(struct join_client_info infos);


    /**
     * @mode i
     *
     * @details control channel privacy
     * */

    int privacy_mode_handler(bool);

    /**
     *
     * @mode m
     *
     * @details control message blocking
     * */
    int message_blocking_mode_handler(bool);
    /**
     *
     * @modes p, s
     *
     * @details control channel visibility
     * */
    int channel_visibility_mode_handler(bool);
    /**
     *
     * @mode t
     *
     * @details control topic enabling
     * */
    int channel_topic_mode_handler(bool);


    /**
     *
     * @mode b
     *
     * @arg user nick
     *
     * @details  ban a user
     * */

    /**
     * MODE #channel +o nick
     * @mode o
     *
     * @arg <channel, user nick>
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

    int public_mode_handler(bool);
    void AddToinvited(std::string nick_name);
};


#endif //FT_IRC_CHANNEL_H
