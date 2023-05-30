/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_manager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmaziane <zmaziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:55:34 by zmaziane          #+#    #+#             */
/*   Updated: 2023/05/30 18:55:35 by zmaziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//
// Created by anas jaidi on 30/3/2023.
//

#include "client_manager.hpp"


int client_manager::add_client(int fd, struct sockaddr_storage infos) {
    clients.push_back(client(fd, infos));
    return 0;
}

std::vector<client>::iterator client_manager::get_client(int ID) {
    for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++) {
        if (it->getFd() == ID) {
            return it;
        }
    }
    return clients.end();
}

int client_manager::get_client_id_by_nick(std::string &nick)  {
    for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++) {
        if (it->getNick() == nick) {
            return it->getFd();
        }
    }
    return -1;
}

std::string client_manager::get_client_Nick_by_Id(int id)  {
    for (std::vector<client>::iterator itId = clients.begin(); itId != clients.end(); itId++) {
        if (itId->getFd() == id) {
            return itId->getNick();
        }
    }
    return ("error");
}

int client_manager::remove_client(int ID) {

    std::vector<client>::iterator to_remove = get_client(ID);

    int client_index = std::distance(clients.begin(), to_remove)  + 1;

    if (to_remove != clients.end()) {

        close(to_remove->getFd());

        clients.erase(to_remove);

        return client_index;
    }
    return -1;
}

int client_manager::update_client_info(update_action action, std::string &payload, int ID) {
    std::vector<client>::iterator to_update = get_client(ID);

    if (to_update == clients.end())
        return -1;

    int client_index = std::distance(clients.begin(), to_update)  + 1;

    switch (action) {
        case UpdateNick:
            to_update->setNick(payload);
            break;
        case UpdateUserName:
            to_update->setUser(payload);
            break;
    }
    return client_index;
}

int client_manager::get_client_id(int fd) {
    std::vector<client>::iterator to_update = get_client(fd);

    if (to_update == clients.end())
        return -1;

    int client_index = std::distance(clients.begin(), to_update)  + 1;

    return  client_index;
}