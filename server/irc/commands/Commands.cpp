//
// Created by anas jaidi on 30/3/2023.
//

#include "Commands.hpp"
# include <sstream>

# define log(x) std::cout<< x << std::endl;

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

void Commands::nick(std::string payload, int new_client_fd) {
    const int ID = update_client_info(update_action::UpdateNick, payload, new_client_fd);
    payload = payload.substr(payload.find_first_not_of(" \t\r\n\v\f"));
    payload = payload.substr(0, payload.find_first_not_of(" \t\r\n\v\f") + 1);
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

    log(pass);
    log(server_pass);
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
    size_t pos = str.find_first_of(" \r\n\t\v\f\n");
    if (pos == std::string::npos) {
        return str;
    } else {
        return str.substr(0, pos);
    }
}

std::string Commands::parse_pass_command(std::string &req) {

   req = req.substr(4);

    trim_fun(req);

    if (req.empty()) {
        // error case
    }

    return std::string(req);
}

std::string Commands::parse_nick_command(std::string &req) {
    req = req.substr(4);

    trim_fun(req);

    if (req.empty()) {
        // error case
    }

    return std::string(req);
}

std::string Commands::parse_user_command(std::string &req) {

    req = req.substr(4);

    std::vector<std::string> parts = split(req, ':');

    if (parts.size() < 2) {
        // error case
    }

    std::istringstream iss(parts[0]);

    std::string word;

    std::string real_name = std::string(":") + parts[1];

    std::vector<std::string> words;

    while (iss >> word) {
        trim_fun(word);
        words.push_back(word);
    }

    if (words.size() != 3 || words[1] != "*" || words[2] != "0") {
        // error case
    }

    return words[0];
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
    } else if(cmd == "JOIN") {
        payload = parse_join_command(request);
        action = OptionCommands::JOIN;
    } else if (cmd == "BOT") {
        payload = parse_bot_command(request);
        action = OptionCommands::BOT;
    } else if (cmd == "INVITE") {
        payload = parse_invite_command(request);
        action = OptionCommands::INVITE;
    } else if (cmd == "PART") {
        payload = parse_part_command(request);
        action = OptionCommands::PART;
    } else if (cmd == "KICK"){
        payload = parse_kick_command(request);
        action = OptionCommands::KICK;
    }else if (cmd == "MODE") {
        payload = parse_mode_command(request);
        action = OptionCommands::MODE;
    } else {
        return std::make_pair(OptionCommands::UNDEFINED, std::string());
    }

    remove_whitespaces(payload);
    return std::make_pair(action, payload);
}

std::vector<std::string> split(std::string line, char c)
{

    std::vector<std::string> command;
    std::stringstream ss(line);
    std::string str;
    while (getline(ss, str, c))
        command.push_back(str);
    return (command);
}

std::string Commands::parse_part_command(std::string &req)
{
    channel_manager manage;
    std::vector<std::string> cmd = split(req,' ');
    std::string nameChannel = cmd[1];
    trim_fun(nameChannel);
    if (cmd.size() != 2) {
        return "Not enough parameters"; // rpl = ":localhost 441 " + clientname  + " : Not enough parameters"
    }
    if(nameChannel[0] != '#') {
        return "No such nick/channel"; // rpl = ":localhost 401 " + nameChannel + " : No such nick/channel\r\n"
    }
    return (nameChannel);
}

void Commands::part(std::string payload, int client_fd) {
    std::string msg;
    if (payload == "Not enough parameters") {
        msg = ":localhost 441 : Not enough parameters";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return;
    }
    else if(payload == "No such nick/channel"){
        msg = ":localhost 401 : No such nick/channel\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return;
    } else
        delete_from_channel(client_fd, payload);
}

std::string Commands::parse_join_command(std::string &req)
{
    req.erase(0, 5);

    std::vector<std::string> splited_command = split(req, ' ');

    if (splited_command.size() > 2 || !splited_command.size()) {
        //  error case
    }

    std::vector<std::string> channels = split(splited_command[0], ',');

    std::vector<std::string> keys;
    if (splited_command.size() == 2)
        keys = split(splited_command[1], ',');

    for (std::string ch : channels) {
        if (ch[0] != '#' && ch[0] != '&') {
            // case error
        }
    }
    return (
            joinByMe(channels, '*') + std::string("|") + joinByMe(keys, '*')
    );
}

void Commands::join(std::string payload, int client_fd, t_join_client infos) {

    std::vector<std::string> desr = split(payload, '|');

    std::vector<std::string> channels_names = split(desr[0], '*');
    std::vector<std::string> keys;

    if (desr.size() == 2) {
        keys = split(desr[1], '*');
    }

    for (int i = 0; i < channels_names.size(); i++) {
        std::vector<channel>::iterator it = get_channel_by_name(channels_names[i]);

        if (i >= keys.size()) {
            if (it != channels.end()) {
                if (it->getPassword() != "") {
                    std::string msgError = "475 * " + it->getName() + " :Cannot join channel (+k)\r\n";
                    send(client_fd, msgError.c_str(), msgError.size(), 0);
                    return ;
                }

                add_to_exist(*it, "", client_fd, infos);
            } else {
                create_channel(client_fd, channels_names[i], infos);
            }
        } else {
            if (it != channels.end()) {
                if (it->getPassword() != keys[i]) {
                    std::string msgError = "464 * :Password incorrect \r\n";
                    send(client_fd, msgError.c_str(), msgError.size(), 0);
                    return ;
                }
                add_to_exist(*it, "", client_fd, infos);
            } else {
                create_channel(client_fd, channels_names[i], infos, keys[i]);
            }
        }
    }
}

std::vector<std::string> parse_and_get_modes(std::string &modes) {

    std::vector<std::string> returned_modes;

    std::string availble_modes = "impstbovkln";

    if (modes.length() && modes[modes.length() - 1] == '-' && modes[modes.length() - 1] == '+') {
        // case error
    }

    modes = modes.substr(modes.find_first_not_of(" \r\t\n"));

    modes = modes.substr(0, modes.find_last_not_of(" \r\t\n") + 1);


     //  +ispx

    for (int i = 0; i < modes.length(); i++) {
        if (modes[i] && modes[i] == '-' && modes[i + 1] != modes[i]) {

            while ( modes[i + 1] && availble_modes.find((modes[i + 1])) != -1) {
                returned_modes.push_back(std::string("-") + modes.substr(i + 1, 1));
                i++;
            }
            if (availble_modes.find((modes[i + 1])) == -1 && modes[i + 1] != '-' && modes[i + 1] != '+'  && modes[i + 1] != '\0') {
                // error case

            }

        } else if (modes[i] && modes[i] == '+' && modes[i + 1] != modes[i]) {
            while ( modes[i + 1] && availble_modes.find((modes[i + 1])) != -1) {
                returned_modes.push_back(std::string("+") + modes.substr(i + 1, 1));
                i++;
            }
            if (availble_modes.find((modes[i + 1])) == -1 && modes[i + 1] != '-' && modes[i + 1] != '+' && modes[i + 1] != '\0' ) {
                // error case

            }

        } else {
            // case error
        }
    }
    return returned_modes;
}


std::string Commands::parse_mode_command(std::string &req) {
    req = req.erase(0, 5);

    std::vector<std::string> parts = split(req, ' ');

    if (parts.size() <= 2) {
        // error case
    }

    std::string target = parts[0];

    std::string modes_str = parts[1];

    std::string args;

    if (parts.size() == 3) {
        args = parts[2];
    }

    std::vector<std::string> modes = parse_and_get_modes(modes_str);

    return (
            target + std::string("|") + joinByMe(modes, '*') + std::string("|") + (
                parts.size() == 3 ? args : ""
            )
    );
}

void Commands::mode(std::string payload, int client_fd, t_join_client infos) {
    std::vector<std::string> parts = split(payload, '|');
    if (parts.size() < 2) {
        // case error
        std::cout << "parse error" << std::endl;
        return ;
    }
    std::string channel_name = parts[0];
    std::vector<std::string> modes = split(parts[1], '*');

    std::string args;

    if (parts.size() >= 3) {
        args = parts[2];
    }

    std::vector<channel>::iterator it = get_channel_by_name(channel_name);

    if (it == channels.end()) {
        // case error
        std::cout << "channel not found" << std::endl;
        return ;
    }
    int new_client_fd;

    channel targeted_channel = *it;

    for (int i = 0; i < modes.size(); i++) {
        switch (modes[i][1]) {
            case 'i':
                if (modes[i][0] == '+') targeted_channel.privacy_mode_handler(1);
                else targeted_channel.privacy_mode_handler(0);
            break;
            case 'm':
                if (modes[i][0] == '+') targeted_channel.message_blocking_mode_handler(1);
                else targeted_channel.message_blocking_mode_handler(0);
            break;
            case 'p':
                if (modes[i][0] == '+') targeted_channel.channel_visibility_mode_handler(1);
                else targeted_channel.channel_visibility_mode_handler(0);
            break;
            case 's':
                if (modes[i][0] == '+') targeted_channel.channel_visibility_mode_handler(1);
                else targeted_channel.channel_visibility_mode_handler(0);
            break;
            case 't':
                if (modes[i][0] == '+') targeted_channel.channel_topic_mode_handler(1);
                else targeted_channel.channel_topic_mode_handler(0);
            break;
            case 'b':
                new_client_fd = get_client_id_by_nick(args);
                if (new_client_fd == -1) {
                    // error case
                    std::cout << "nick  exists." << std::endl;
                } else {
                    std::cout << "nick not exists." << std::endl;
                    exit(0);
                }
                if (modes[i][0] == '+') targeted_channel.ban_mode_handler(1, client_fd);
                else targeted_channel.ban_mode_handler(0, client_fd);
            break;
            case 'o':
                std::cout << clients.size()  << " | " << clients[0].getNick()  << (clients[0].getNick() == args) << std::endl;
                std::cout << "inside operator mode" << std::endl;
                new_client_fd = get_client_id_by_nick(args);
                if (new_client_fd == -1) {
                    std::cout << "nick  exists." << std::endl;
                } else {
                    std::cout << "nick not exists." << std::endl;
                }
                if (modes[i][0] == '+') targeted_channel.operator_mode_handler(1, client_fd);
                else targeted_channel.operator_mode_handler(0, client_fd);
            break;
            case 'v':
                new_client_fd = get_client_id_by_nick(args);
                if (new_client_fd == -1) {
                    std::cout << "nick  exists." << std::endl;
                } else {
                    std::cout << "nick not exists." << std::endl;
                }
                if (modes[i][0] == '+') targeted_channel.operator_friend_mode_handler(1, client_fd);
                else targeted_channel.operator_friend_mode_handler(0, client_fd);
            break;
            case 'k':

                if (modes[i][0] == '+') targeted_channel.pass_mode_handler(1, args);
                else targeted_channel.pass_mode_handler(0, args);
            break;
            case 'l':

                if (modes[i][0] == '+') targeted_channel.limit_mode_handler(1, std::stoi(args));
                else targeted_channel.limit_mode_handler(0, std::stoi(args));
            break;
        }
    }

    
}
