/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_manager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmaziane <zmaziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:55:19 by zmaziane          #+#    #+#             */
/*   Updated: 2023/05/30 18:55:20 by zmaziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by mustapha ouarsass on 4/9/23.
//

#ifndef FT_IRC_CHANNEL_MANAGER_HPP
#define FT_IRC_CHANNEL_MANAGER_HPP



# include "../../../../FT_IRC.h"
#include "../channel.hpp"
class channel_manager {
protected:
    std::vector<channel> channels;
public:
    int create_channel(int client_fd, std::string channel,  t_join_client infos, std::string key = std::string("@notfound@"));
    int add_to_exist(channel &channel, std::string key,int client_fd, t_join_client infos, std::vector<std::string> members_nicks, std::vector<std::string> admins_nicks);
    std::vector<channel>::iterator  get_channel_by_name(std::string &name);
    void render_channels();
    void delete_from_channel(int client_fd, std::string &channel_name);
    bool isOperator(int fdOperator, std::vector<channel>::iterator itch);

};


#endif //FT_IRC_CHANNEL_MANAGER_HPP
