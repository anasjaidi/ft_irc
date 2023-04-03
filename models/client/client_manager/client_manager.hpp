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
};


#endif //FT_IRC_CLIENT_MANAGER_HPP
