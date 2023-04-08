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
    if (!priv)
    {
        FinalCmd = ":localhost 461 " + server->clientMap[client_fd].getNick() + ": Not enough parameters \r\n";
        send(client_fd, FinalCmd.c_str(), FinalCmd.size(), 0);
    }
    // reach of success
    std::string payload = joinByMe(OneChannel, '*') + std::string("&") + joinByMe(OneKey, '*');
    return (payload);
}

bool    IsChannelBanned(Server *server, int client_fd, std::string NameChannel)
{
    unsigned int ip = server->clientMap[client_fd].getFd();
    std::vector<unsigned int> ban = server->mapChannel[NameChannel].mumbers_banned;
    for (size_t i = 0; i < ban.size(); i++)
    {
        if(ban[i] == ip)
            return (true);
    }
    return false;
}

int joinChannel(Server *server,std::vector<std::string> cmd, int client_fd) {
    channel myChannel = server->mapChannel[cmd[1]];

    if(myChannel.key)
    {
        if(myChannel.password != cmd[2]) {
            std::string msg = "475 * " + myChannel.name + " :Cannot join channel (+k)\r\n";
            send(client_fd, msg.c_str(), msg.size(), 0);
            return EXIT_FAILURE;
        }
    }
    for(size_t i = 0; i < myChannel.fds_channel.size(); i++) {
        if(myChannel.fds_channel[i] == client_fd)
        {
            std::string msg = "443 * " + server->clientMap[client_fd].getNick() + " " + myChannel.name + " :is already on channel\r\n";
            send(client_fd, msg.c_str(), msg.size(), 0);
            return (EXIT_FAILURE);
        }
    }
    //check if number of mumberslimits is full
    if (myChannel.fds_channel.size() > (size_t)myChannel.mumberLimited)
    {
        std::string msg = "471 * " + myChannel.name + " :Cannot join channel (+l)\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return (EXIT_FAILURE);
    }
    // added fd client to the channel
    std::string msgSend = ":" + server->clientMap[client_fd].clientInformations()+ " JOIN " + cmd[1] + "\r\n"
      ":localhost 332 " + server->clientMap[client_fd].getNick() + " " + cmd[1] + " :This is my cool channel! https://irc.com\r\n"
      ":localhost 333 " + server->clientMap[client_fd].getNick() + " " + cmd[1] + " " + server->clientMap[client_fd].getNick() +"!" +server->clientMap[client_fd].getChannel() +"@localhost"  " 1547691506\r\n"
      ":localhost 353 " + server->clientMap[client_fd].getNick() + " @ " + cmd[1] + " :" + server->clientMap[client_fd].getNick() + " @"+ server->clientMap[client_fd].getNick() + "\r\n";
     send(client_fd, msgSend.c_str(), msgSend.size(), 0);
    for (IteratorForChannel it = server->mapChannel.begin(); it != server->mapChannel.end(); it++)
    {
        if (it->second.name == cmd[1])
        {
            server->clientMap[client_fd].nameChannels.push_back(cmd[1]);
            it->second.fds_channel.push_back(client_fd);
            it->second.mumbers.push_back(client_fd);
            msgSend = ":" + server->clientMap[ client_fd].clientInformations() + " JOIN " + cmd[1] + "\r\n";
            for (size_t i = 0; i < it->second.fds_channel.size(); i++)
            {
                if (it->second.fds_channel[i] != client_fd)
                    send(it->second.fds_channel[i], msgSend.c_str(), msgSend.size(), 0);
            }
        }
    }
    return (EXIT_SUCCESS);
}

void Commands::join(std::string payload, int client_fd) {
    std::string msg;
    Server *server;
    bool isExistChannel = true;
    //deserialize
    int pos = payload.find("&");
    if (pos == -1)
        msg = "error";/////////////////TODO print error whit  std::cout<< "";
    std::string channel = payload.substr(0, pos);
    std::string key = payload.substr(pos+1, payload.length());
    std::vector<std::string> channels = split(channel, '*');
    std::vector<std::string> keys = split(key, '*');

    std::vector<std::string>::iterator it;
//    size_t len = 0;
    size_t len = 0;

    for(size_t j = 0; j > channels.size();j++) {
//    for (it = channels.begin(); it != channels.end(); ++it) {
        for (int index = 0; index < server->channelNames[index].size(); index++)
        {
            std::string buff;
            if (server->channelNames[index] == channels[j])
            {
                if (IsChannelBanned(server, client_fd, channels[j])) // true
                {
                    msg = ":localhost 474 " + channels[j] + " " + server->clientMap[client_fd].getNick() + ":Cannot join channel (+b)\r\n";
                    send(client_fd, msg.c_str(), msg.size(), 0);
                }
                // TODO check mode channel

                else if(!IsChannelBanned(server, client_fd, channels[j])) // false
                {
                    if (keys.size() > j)
                         buff = "JOIN " + channels[j] + " " + keys[j];
                    else
                         buff = "JOIN " + channels[j];
                    std::vector<std::string> cmd = split(buff, ' ');
                    joinChannel(server, cmd, client_fd);
                }

            }
            else // check existing pass in channel and creat it
            {
                    if (keys.size() > j)
                         buff = "JOIN " + channels[j] + " " + keys[j];
                    else
                         buff = "JOIN " + channels[j];
                    std::vector<std::string> cmd = split(buff, ' ');
                createChannel(server, cmd, client_fd);
            }

        }
    }

    for(int i = 0; i < channels.size(); i++){

    }
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