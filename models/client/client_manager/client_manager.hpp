/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_manager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmaziane <zmaziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:55:37 by zmaziane          #+#    #+#             */
/*   Updated: 2023/05/30 18:55:38 by zmaziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_CLIENT_MANAGER_HPP
#define FT_IRC_CLIENT_MANAGER_HPP

# include "../../../FT_IRC.h"
# include "../client.hpp"

class client_manager {
protected:
    std::vector<client> clients;

public:
    std::vector<client>::iterator get_client(int ID);

    int remove_client(int ID);

    int add_client(int fd, struct sockaddr_storage infos);

    int get_client_id(int fd);
    int get_client_id_by_nick(std::string &);
    std::string get_client_Nick_by_Id(int);



    enum update_action {
        UpdateNick,
        UpdateUserName,
    };


    int update_client_info(update_action action, std::string &payload, int ID);
};


#endif //FT_IRC_CLIENT_MANAGER_HPP
