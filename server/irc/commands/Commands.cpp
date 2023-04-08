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
void Commands::user(std::string payload, int new_client_fd) {
    const int ID = update_client_info(update_action::UpdateUserName, payload, new_client_fd);

    if (ID == -1) {
        std::cerr << "User Not Found: Internal Server Error." << std::endl;
        close(new_client_fd);
    } else {
        std::cout << "User Nº " << ID << ": update hes username." << std::endl;
        std::vector<client>::iterator updated_client = get_client(new_client_fd);

        if (updated_client == clients.end()) {
            std::cerr << "Error." << std::endl;
        } else {
            std::cout << "user: " << updated_client->getUser() << "\n" << "nick: " << updated_client->getNick() << std::endl;
        }


    }
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
    } else {
        int ID = get_client_id(new_client_fd);
        std::cout << "client Nº " << ID << " authenticated with password." << std::endl;
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

std::string Commands::parse_who_command(std::string &req) { //////// not yet finished

    remove_whitespaces(req);

    int start = req.find_first_of(" \r\n");


    if (start == std::string::npos) return std::string("Error: parse password command.");


    std::string who = req.substr(start + 1, req.length());

    return who;
}
std::vector<std::string> Commands::split(std::string line, char c)
{

    std::vector<std::string> command;
    std::stringstream ss(line);
    std::string str;
    while (getline(ss, str, c))
        command.push_back(str);
    return (command);
}

std::string joinByMe(std::vector<std::string> &vec, char c)
{
    std::string result = "";
    for (int i = 0; i < vec.size(); i++) {
        result += vec[i];
        if (i != vec.size() - 1) {
            result += c;
        }
    }
    return result;
}

std::string Commands::parse_join_command(std::string &req, int client_fd)
{
    Server *server;
    std::string FinalCmd;
    req.erase(0, 5);
    int v;
    bool priv = true;
    std::vector<std::string> command = split(req, ' ');
    std::string AllChannels = command[0], AllKeys = command[1];
    for (int i = 0; req[i] != ' '; i++)
        if (req[i] == ',')v++;
    std::vector<std::string> OneChannel = split(AllChannels, ',');
    std::vector<std::string> OneKey = split(AllKeys, ',');
    if (OneChannel.size() < OneKey.size())
        std::cerr << "Error: arguments is not exist !" << std::endl;
    else
    {
        for(int j=0; j < v; j++)
        {
            if (OneChannel[j][0] != '&' && OneChannel[j][0] != '#'){
                priv = false;
                break;}
        }
    }
    if (priv == false)
    {
        FinalCmd = ":localhost 461 " + server->clientMap[client_fd].getNick() + ": Not enough parameters \r\n";
        send(client_fd, FinalCmd.c_str(), FinalCmd.size(), 0);
    }
    // reach of success
    std::string payload = joinByMe(OneChannel, '*') + std::string("&") + joinByMe(OneKey, '*');
    return (payload);
}


void Commands::join(std::string payload, int client_fd) {
    std::string msg;

    //deserialize

    int pos = payload.find("&");
    if (pos == -1)
        msg = "error";
    std::string channel = payload.substr(0, pos);
    std::string key = payload.substr(pos+1, payload.length());
    std::vector<std::string> channels = split(channel, '*');
    std::vector<std::string> keys = split(key, '*');

}

std::pair<Commands::OptionCommands, std::string> Commands::get_command(std::string &request, int client_fd)
{
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
    } else if(cmd == "JOIN") {
        payload = parse_join_command(request, client_fd);
        action = OptionCommands::JOIN;
    } else if (cmd == "WHO") {
        payload = parse_who_command(request);
        action = OptionCommands::WHO;
    } else if (cmd == "PART") {
        payload = parse_part_command(request);
        action = OptionCommands::PART;
    } else {
        return std::make_pair(OptionCommands::UNDEFINED, std::string());
    }


    remove_whitespaces(payload);
    return std::make_pair(action, payload);
}


void Commands::who(std::string payload, int new_client_fd) {

}

// "anas*12*13"