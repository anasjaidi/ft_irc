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

int client_manager::remove_client(int ID) {
    std::vector<client>::iterator to_remove = get_client(ID);

    if (to_remove != clients.end()) {
        clients.erase(to_remove);
        return 0;
    }
    return -1;
}