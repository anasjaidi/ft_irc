//
// Created by anas jaidi on 30/3/2023.
//

#include "Commands.hpp"


void Commands::nick(std::string) {
    std::cout << "start nickname.\n";
}
void Commands::user(std::string) {
    std::cout << "start user\n";
}



void Commands::pass(std::string pass, int new_client_fd, std::string server_pass) {
    if (pass != server_pass) {
        int  ID = remove_client(new_client_fd);

        if (ID != -1) {
            std::cout << "client Nº " <<  ID << " removed: " \
        << "server password error " << "password mismatched" << std::endl;
        } else {
            std::cerr << "User Not Found: Internal Server Error" << std::endl;
        }
    }
}
