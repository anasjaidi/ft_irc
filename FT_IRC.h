//
// Created by Mustapha on 12/3/2023.
//

#ifndef FT_IRC_FT_IRC_H
#define FT_IRC_FT_IRC_H
#include <iostream>
#include <string>
# include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
# include <unistd.h>
# include <libc.h>
# include <poll.h>
# include <vector>

typedef struct join_client_info {
    std::string nick;
    std::string user;
    struct sockaddr_in *info;
}   t_join_client;
void trim_fun(std::string &str);
#endif //FT_IRC_FT_IRC_H
