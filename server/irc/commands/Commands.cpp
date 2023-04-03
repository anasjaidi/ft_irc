//
// Created by anas jaidi on 30/3/2023.
//

#include "Commands.hpp"
# include <sstream>

void Commands::nick(std::string payload, int new_client_fd) {
    const int ID = update_client_info(update_action::UpdateNick, payload, new_client_fd);

    if (ID == -1) {
        std::cerr << "User Not Found: Internal Server Error." << std::endl;
        close(new_client_fd);
    } else {
        std::cout << "User Nº " << ID << ": update hes nickname." << std::endl;
    }
}
void Commands::user(std::string, int new_client_fd) {
    std::cout << "start user\n";
}



void Commands::pass(std::string pass, int new_client_fd, std::string server_pass) {
    if (pass != server_pass) {
        int  ID = remove_client(new_client_fd);

        if (ID != -1) {
            std::cout << "client Nº " <<  ID << " removed: " \
        << "server password error, " << "password mismatched" << std::endl;
        } else {
            std::cerr << "User Not Found: Internal Server Error" << std::endl;
            close(new_client_fd);
        }
    }
}

void remove_whitespaces(std::string &str) {
    while (std::isspace(str[0])) str.erase(0, 1);
    while (std::isspace(str[str.length() - 1])) str.erase(str.length() - 1, 1);
}

void UPPER_STRING(std::string &str) {
    for (int i = 0; i < str.length(); ++i) {
        str[i] = std::toupper(str[i]);
    }
}

std::string get_first_word(const std::string& str) {
    size_t pos = str.find_first_of(" \r\n");
    if (pos == std::string::npos) {
        return str;
    } else {
        return str.substr(0, pos);
    }
}

std::string Commands::parse_pass_command(std::string &req) {

    remove_whitespaces(req);

    int start = req.find_first_of(" \r\n");


    if (start == std::string::npos) return std::string("Error: parse password command.");


    std::string password = req.substr(start, req.length());

    return password;
}

std::string Commands::parse_nick_command(std::string &req) {
    remove_whitespaces(req);

    int start = req.find_first_of(" \r\n");


    if (start == std::string::npos) return std::string("Error: parse password command.");


    std::string nick = req.substr(start, req.length());

    return nick;
}

std::string Commands::parse_user_command(std::string &req) {
    std::istringstream iss(req);

    std::string word;

    std::vector<std::string> words;

    while (iss >> word) {
        words.push_back(word);
    }

    if (words.size() < 5) return std::string("Error: NICK parse Error.");

    return words[1];
}

std::pair<Commands::OptionCommands, std::string> Commands::get_command(std::string &request) {


    std::string cmd = get_first_word(request);

    std::string payload;

    OptionCommands action;

    UPPER_STRING(cmd);

    if (cmd == "PASS") {
        payload = parse_pass_command(request);

        action = OptionCommands::PASS;
    } else if (cmd == "NICK") {
        payload = parse_nick_command(request);
        action = OptionCommands::NICK;
    } else if (cmd == "USER") {
        payload = parse_user_command(request);
        action = OptionCommands::USER;
    } else {
        return std::make_pair(OptionCommands::UNDEFINED, std::string());
    }


    remove_whitespaces(payload);
    return std::make_pair(action, payload);
}