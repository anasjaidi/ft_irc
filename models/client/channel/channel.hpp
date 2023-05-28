
//
// Created by mustapha ouarsass on 4/9/23.
//

#ifndef FT_IRC_CHANNEL_H
#define FT_IRC_CHANNEL_H


# include "../../../FT_IRC.h"
#include <sstream>



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

    const std::vector<int>& get_memebers_fds();
    const std::vector<int>& get_all_fds();
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

    void send_msg_to_all_members(int client_fd, std::string kicked, std::string channel);

    bool itIsInChannel(int client_fd);
    std::vector<int> operator_friends;

    std::string clientInformationsForChannel(struct join_client_info infos);

    bool adminChannel(int fd);

    void broadcast_message(std::string &);

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

    int ban_mode_handler(bool, int);

    /**
     * MODE #channel +o nick
     * @mode o
     *
     * @arg <channel, user nick>
     *
     * @details add user as admin
     * */
    int operator_mode_handler(bool, int);

    /**
     *
     * @mode v
     *
     * @arg user nick
     *
     * @details permit user to talk besides the admins when chat is blocked
     * */
    int operator_friend_mode_handler(bool, int);


    /**
     *
     * @mode k
     *
     * @arg control password
     * */

    std::string pass_mode_handler(bool, std::string &);

    /**
     *
     * @mode l
     *
     * @arg new limit
     *
     * @details control channel limit
     * */

    int limit_mode_handler(bool, int);

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
