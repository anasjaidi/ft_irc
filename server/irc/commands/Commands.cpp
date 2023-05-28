//
// Created by anas jaidi on 30/3/2023.
//

#include "Commands.hpp"
#include <sstream>

#define log(x) std::cout << x << std::endl;

std::string joinByMe(std::vector<std::string> &vec, char c)
{
    std::string result = "";
    for (int i = 0; i < vec.size(); i++)
    {
        trim_fun(vec[i]);
        result += vec[i];
        if (i != vec.size() - 1)
        {
            result += c;
        }
    }
    return result;
}

int Commands::nick(std::string payload, int new_client_fd, std::vector<client>::iterator client)
{

    const int ID = update_client_info(UpdateNick, payload, new_client_fd);

    trim_fun(payload);

    if (ID == -1)
    {
        std::cerr << "User Not Found: Internal Server Error." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "User Nº " << ID << ": update hes nickname." << std::endl;
    }
    return 0;
}

int Commands::user(std::string payload, int new_client_fd, std::vector<client>::iterator theclient)
{

    std::vector<client>::iterator it = get_client(new_client_fd);

    if (it == clients.end() || it->getNick().empty())
    {
        return 1;
    }
    const int ID = update_client_info(UpdateUserName, payload, new_client_fd);

    if (ID == -1)
    {
        std::cerr << "User Not Found: Internal Server Error." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "User Nº " << ID << ": update hes username." << std::endl;
        std::vector<client>::iterator updated_client = get_client(new_client_fd);

        if (updated_client == clients.end())
        {
            std::cerr << "Error." << std::endl;
            return 1;
        }
        else
        {
            std::cout << "user: " << updated_client->getUser() << "\n"
                      << "nick: " << updated_client->getNick()
                      << std::endl;
            ////////////// replay to limechat ////////////
            char time_str[11];
            std::time_t now = std::time(NULL);
            std::tm *local_time = std::localtime(&now);

            std::strftime(time_str, sizeof(time_str), "%d/%m/%Y", local_time);

            std::string msg = ":irc.1337.ma 001 " + theclient->getNick() + " Welcome to internet chat relay \r\n"
                            + ": 002 " + theclient->getNick() + " the host is: localhost, running version 1.0 \r\n"
                            + ": 003 " + theclient->getNick() + " the server was created on " + std::string(time_str) + "\r\n";
            send(new_client_fd, msg.c_str(), msg.size(), 0);
        }
    }
    return 0;
}

int Commands::pass(std::string pass, int new_client_fd, std::string server_pass, std::vector<client>::iterator theclient)
{

    if (pass != server_pass)
    {
        send(new_client_fd, "464 ERR_PASSWDMISMATCH:Password incorrect\r\n", std::strlen("464 ERR_PASSWDMISMATCH:Password incorrect\r\n"), 0);
        return 0;
    }
    else
    {
        int ID = get_client_id(new_client_fd);

        std::vector<client>::iterator it = get_client(new_client_fd);

        if (it == clients.end())
        {
            std::cout << "password error client not found." << std::endl;
            return 1;
        }

        it->setIsAuth(true);

        std::cout << "client Nº " << ID << " authenticated with password." << std::endl;
    }
    return 0;
}

void remove_whitespaces(std::string &str)
{
    while (std::isspace(str[0]))
        str.erase(0, 1);
    while (std::isspace(str[str.length() - 1]))
        str.erase(str.length() - 1, 1);
}

void UPPER_STRING(std::string &str)
{
    for (int i = 0; i < str.length(); ++i)
    {
        str[i] = std::toupper(str[i]);
    }
}

std::string get_first_word(const std::string &str)
{
    size_t pos = str.find_first_of(" \r\n\t\v\f\n");
    if (pos == std::string::npos)
    {
        return str;
    }
    else
    {
        return str.substr(0, pos);
    }
}

std::string Commands::parse_pass_command(std::string &req, int client_fd, std::vector<client>::iterator theclient)
{

    if (req.length() < 5)
    {
        send(client_fd, "", strlen(""), 0);
        std::string("%%{ERROR}%%");
    }

    req = req.substr(4);
    trim_fun(req);

    if (req.empty())
    {
        return std::string("%%{ERROR}%%");
    }

    return std::string(req);
}

std::string Commands::parse_nick_command(std::string &req, int client_fd, std::vector<client>::iterator theclient)
{
    req = req.substr(4);

    trim_fun(req);

    std::vector<std::string> parts = split(req, ' ');

    if (parts.size() != 1)
    {
        return std::string("%%{ERROR}%%");
    }

    if (req.empty())
    {
        return std::string("%%{ERROR}%%");
    }

    return std::string(req);
}

std::string Commands::parse_user_command(std::string &req, int client_fd, std::vector<client>::iterator theclint)
{

    req = req.substr(4);

    std::vector<std::string> parts = split(req, ':');

    if (parts.size() < 2)
    {
        return std::string("%%{ERROR}%%");
    }

    std::istringstream iss(parts[0]);

    std::string word;

    std::string real_name = std::string(":") + parts[1];

    std::vector<std::string> words;

    while (iss >> word)
    {
        trim_fun(word);
        words.push_back(word);
    }

    if (words.size() != 3 || words[1] != "*" || words[2] != "0")
    {
        return std::string("%%{ERROR}%%");
    }

    return words[0];
}

std::pair<Commands::OptionCommands, std::string> Commands::get_command(std::string &request, int client_fd, std::vector<client>::iterator theclient)
{

    std::string cmd = get_first_word(request);

    std::string payload;

    OptionCommands action;

    UPPER_STRING(cmd);

    if (cmd == "PASS")
    {
        payload = parse_pass_command(request, client_fd, theclient);

        action = PASS;
    }
    else if (cmd == "NICK")
    {
        payload = parse_nick_command(request, client_fd, theclient);
        action = NICK;
    }
    else if (cmd == "USER")
    {
        payload = parse_user_command(request, client_fd, theclient);
        action = USER;
    }
    else if (cmd == "JOIN")
    {
        payload = parse_join_command(request, client_fd, theclient);
        action = JOIN;
    }
    else if (cmd == "BOT")
    {
        payload = parse_bot_command(request,  theclient);
        action = BOT;
    }
    else if (cmd == "INVITE")
    {
        payload = parse_invite_command(request, theclient);
        action = INVITE;
    }
    else if (cmd == "PART")
    {
        payload = parse_part_command(request, client_fd, theclient);
        action = PART;
    }
    else if (cmd == "KICK")
    {
        payload = parse_kick_command(request,  theclient);
        action = KICK;
    }
    else if (cmd == "MODE")
    {
        payload = parse_mode_command(request, client_fd, theclient);
        action = MODE;
    }
    else if (cmd == "PRIVMSG")
    {
        payload = parse_privmsg_command(request, client_fd, theclient);
        action = PRIVATE_MSG;
    }
    else
    {
        return std::make_pair(UNDEFINED, std::string());
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

std::string Commands::parse_part_command(std::string &req, int client_fd, std::vector<client>::iterator theclient)
{
    channel_manager manage;
    std::vector<std::string> cmd = split(req, ' ');
    std::string nameChannel = cmd[1];
    trim_fun(nameChannel);
    if (cmd.size() != 2)
    {
        return "Not enough parameters"; // rpl = ":localhost 441 " + clientname  + " : Not enough parameters"
    }
    if (nameChannel[0] != '#')
    {
        return "No such nick/channel"; // rpl = ":localhost 401 " + nameChannel + " : No such nick/channel\r\n"
    }
    return (nameChannel);
}

void Commands::part(std::string payload, int client_fd, std::vector<client>::iterator theclient)
{
    std::string msg;
    if (payload == "Not enough parameters")
    {
        msg = ":localhost 441 : Not enough parameters";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return;
    }
    else if (payload == "No such nick/channel")
    {
        msg = ":localhost 401 : No such nick/channel\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return;
    }
    else
        delete_from_channel(client_fd, payload);
}

std::string Commands::parse_join_command(std::string &req, int client_fd, std::vector<client>::iterator thclient)
{
    req.erase(0, 5);
    trim_fun(req);
    std::vector<std::string> splited_command = split(req, ' ');

    std::cout << "size of splitted: " << splited_command.size() << std::endl;

    std::cout << "channels: "
              << "\"" << splited_command[0] << "\"" << std::endl;
    if (splited_command.size() == 2)
        std::cout << "keys: "
                  << "\"" << splited_command[1] << "\"" << std::endl;

    if (splited_command.size() > 2 || !splited_command.size())
    {

        return ("Not enough param");
    }

    trim_fun(splited_command[0]);
    std::vector<std::string> channels = split(splited_command[0], ',');

    std::vector<std::string> keys;
    if (splited_command.size() == 2)
    {
        trim_fun(splited_command[1]);
        keys = split(splited_command[1], ',');
    }

    for (std::string ch : channels)
    {
        if (ch[0] != '#' && ch[0] != '&')
        {
            return ("Not enough param");
        }
    }
    return (joinByMe(channels, '*') + std::string("|") + joinByMe(keys, '*'));
}

void Commands::join(std::string payload, int client_fd, t_join_client infos, std::vector<client>::iterator theclint)
{
    std::string msg;
    if (payload == "Not enough param")
    {
        msg = ":localhost 461 JOIN :Not enough parameters\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return;
    }
    std::vector<std::string> desr = split(payload, '|');

    std::vector<std::string> channels_names = split(desr[0], '*');
    std::vector<std::string> keys;

    if (desr.size() == 2)
    {
        keys = split(desr[1], '*');
    }

    for (int i = 0; i < channels_names.size(); i++)
    {
        std::vector<channel>::iterator it = get_channel_by_name(channels_names[i]);

        if (i >= keys.size())
        {
            if (it != channels.end())
            {
                if (it->getPassword() != "")
                {
                    std::string msgError = ":localhost 475 " + it->getName() + " :Cannot join channel (+k)\r\n";
                    send(client_fd, msgError.c_str(), msgError.size(), 0);
                    return;
                }

                std::vector<int> members_fds = (*it).get_all_fds();
                std::vector<std::string> members_nicks;


                for (size_t i = 0; i < members_fds.size(); i++)
                {
                    std::vector<client>::iterator client_it = get_client(members_fds[i]);

                    if (client_it != clients.end()) {
                        std::string client_nick = client_it->getNick();

                        members_nicks.push_back(client_nick);
                    }
                }
                

                add_to_exist(*it, "", client_fd, infos, members_nicks);
            }
            else
            {
                create_channel(client_fd, channels_names[i], infos);
            }
        }
        else
        {
            if (it != channels.end())
            {
                if (it->getPassword() != keys[i])
                {
                    std::string msgError = ":localhost 464 :Password incorrect \r\n";
                    send(client_fd, msgError.c_str(), msgError.size(), 0);
                    return;
                }
                std::vector<int> members_fds = (*it).get_all_fds();
                std::vector<std::string> members_nicks;


                for (size_t i = 0; i < members_fds.size(); i++)
                {
                    std::vector<client>::iterator client_it = get_client(members_fds[i]);

                    if (client_it != clients.end()) {
                        std::string client_nick = client_it->getNick();

                        members_nicks.push_back(client_nick);
                    }
                }
                add_to_exist(*it, "", client_fd, infos, members_nicks);
            }
            else
            {
                create_channel(client_fd, channels_names[i], infos, keys[i]);
            }
        }
    }
}

std::vector<std::string> parse_and_get_modes(std::string &modes)
{

    std::vector<std::string> returned_modes;

    std::string availble_modes = "impstbovkln";

    if (modes.length() && modes[modes.length() - 1] == '-' && modes[modes.length() - 1] == '+')
    {
        // case error
    }

    modes = modes.substr(modes.find_first_not_of(" \r\t\n"));

    modes = modes.substr(0, modes.find_last_not_of(" \r\t\n") + 1);

    //  +ispx

    for (int i = 0; i < modes.length(); i++)
    {
        if (modes[i] && modes[i] == '-' && modes[i + 1] != modes[i])
        {

            while (modes[i + 1] && availble_modes.find((modes[i + 1])) != -1)
            {
                returned_modes.push_back(std::string("-") + modes.substr(i + 1, 1));
                i++;
            }
            if (availble_modes.find((modes[i + 1])) == -1 && modes[i + 1] != '-' && modes[i + 1] != '+' &&
                modes[i + 1] != '\0')
            {
                // error case
            }
        }
        else if (modes[i] && modes[i] == '+' && modes[i + 1] != modes[i])
        {
            while (modes[i + 1] && availble_modes.find((modes[i + 1])) != -1)
            {
                returned_modes.push_back(std::string("+") + modes.substr(i + 1, 1));
                i++;
            }
            if (availble_modes.find((modes[i + 1])) == -1 && modes[i + 1] != '-' && modes[i + 1] != '+' &&
                modes[i + 1] != '\0')
            {
                // error case
            }
        }
        else
        {
            // case error
        }
    }
    return returned_modes;
}

std::string Commands::parse_mode_command(std::string &req, int client_fd, std::vector<client>::iterator theclient)
{
    req = req.erase(0, 5);

    std::vector<std::string> parts = split(req, ' ');

    if (parts.size() <= 2)
    {
        return ("enough");
    }

    std::string target = parts[0];

    std::string modes_str = parts[1];

    std::string args;

    if (parts.size() == 3)
    {
        args = parts[2];
    }

    std::vector<std::string> modes = parse_and_get_modes(modes_str);

    return (
        target + std::string("|") + joinByMe(modes, '*') + std::string("|") + (parts.size() == 3 ? args : ""));
}

void Commands::mode(std::string payload, int client_fd, t_join_client infos)
{
    if (payload == "enough")
    {
        std::string msg = ":localhost 461 MODE :Not enough parameters\r\n";
        send(client_fd, msg.c_str(), msg.size(), 0);
        return;
    }
    std::vector<std::string> parts = split(payload, '|');
    if (parts.size() < 2)
    {
        std::string Errormsg = ":localhost 461 MODE :Not enough parameters\r\n";
        send(client_fd, Errormsg.c_str(), Errormsg.size(), 0);
        return;
    }
    std::string channel_name = parts[0];
    std::vector<std::string> modes = split(parts[1], '*');

    std::string args;

    if (parts.size() >= 3)
    {
        args = parts[2];
    }

    std::vector<channel>::iterator it = get_channel_by_name(channel_name);

    if (it == channels.end())
    {
        std::string Errormsg = ":localhost 401 " + channel_name + " : No such nick/channel\r\n";
        send(client_fd, Errormsg.c_str(), Errormsg.size(), 0);
        return;
    }
    int new_client_fd;

    channel targeted_channel = *it;

    for (int i = 0; i < modes.size(); i++)
    {
        switch (modes[i][1])
        {
        case 'i':
            if (modes[i][0] == '+')
                targeted_channel.privacy_mode_handler(1);
            else
                targeted_channel.privacy_mode_handler(0);
            break;
        case 'm':
            if (modes[i][0] == '+')
                targeted_channel.message_blocking_mode_handler(1);
            else
                targeted_channel.message_blocking_mode_handler(0);
            break;
        case 'p':
            if (modes[i][0] == '+')
                targeted_channel.channel_visibility_mode_handler(1);
            else
                targeted_channel.channel_visibility_mode_handler(0);
            break;
        case 's':
            if (modes[i][0] == '+')
                targeted_channel.channel_visibility_mode_handler(1);
            else
                targeted_channel.channel_visibility_mode_handler(0);
            break;
        case 't':
            if (modes[i][0] == '+')
                targeted_channel.channel_topic_mode_handler(1);
            else
                targeted_channel.channel_topic_mode_handler(0);
            break;
        case 'b':
            new_client_fd = get_client_id_by_nick(args);
            if (new_client_fd == -1)
            {
                std::string Errormsg = ":localhost 401 " + channel_name + " : No such nick/channel\r\n";
                send(client_fd, Errormsg.c_str(), Errormsg.size(), 0);
                //// return here
            }
            else
            {
                std::string Errormsg = ":localhost 401 " + channel_name + " : No such nick/channel\r\n";
                send(client_fd, Errormsg.c_str(), Errormsg.size(), 0);
                return;
            }
            if (modes[i][0] == '+')
                targeted_channel.ban_mode_handler(1, client_fd);
            else
                targeted_channel.ban_mode_handler(0, client_fd);
            break;
        case 'o':
            std::cout << clients.size() << " | " << clients[0].getNick() << (clients[0].getNick() == args)
                      << std::endl;
            std::cout << "inside operator mode" << std::endl;
            new_client_fd = get_client_id_by_nick(args);
            if (new_client_fd == -1)
            {
                std::string Errormsg = ":localhost 401 " + channel_name + " : No such nick/channel\r\n";
                send(client_fd, Errormsg.c_str(), Errormsg.size(), 0);
                return;
            }
            else
            {
                std::string Errormsg = ":localhost 401 " + channel_name + " : No such nick/channel\r\n";
                send(client_fd, Errormsg.c_str(), Errormsg.size(), 0);
                /// return here
            }
            if (modes[i][0] == '+')
                targeted_channel.operator_mode_handler(1, client_fd);
            else
                targeted_channel.operator_mode_handler(0, client_fd);
            break;
        case 'v':
            new_client_fd = get_client_id_by_nick(args);
            if (new_client_fd == -1)
            {
                std::string Errormsg = ":localhost 401 " + channel_name + " : No such nick/channel\r\n";
                send(client_fd, Errormsg.c_str(), Errormsg.size(), 0);
                return;
            }
            else
            {
                std::string Errormsg = ":localhost 401 " + channel_name + " : No such nick/channel\r\n";
                send(client_fd, Errormsg.c_str(), Errormsg.size(), 0);
                /// return here
            }
            if (modes[i][0] == '+')
                targeted_channel.operator_friend_mode_handler(1, client_fd);
            else
                targeted_channel.operator_friend_mode_handler(0, client_fd);
            break;
        case 'k':
            if (modes[i][0] == '+')
                targeted_channel.pass_mode_handler(1, args);
            else
                targeted_channel.pass_mode_handler(0, args);
            break;
        case 'l':
            if (modes[i][0] == '+')
                targeted_channel.limit_mode_handler(1, std::stoi(args));
            else
                targeted_channel.limit_mode_handler(0, std::stoi(args));
            break;
        }
    }
}

std::string Commands::parse_privmsg_command(std::string &req, int client_fd, std::vector<client>::iterator theclient)
{
    std::string payload;
    std::string msg;

    req = req.substr(7);
    trim_fun(req);

    int pos = req.find(' ');

    if (pos == -1)
    {
        msg = ":localhost 461 PRIVMSG :Not enough parameters\n\r";
        send(client_fd,msg.c_str(), msg.size(), 0);
        std::cout << "no space\n";
        return "%%{ERROR}%%";
    }

    std::string chs = req.substr(0, pos);

    std::string message = req.substr(pos + 1, req.length());

    if (message.empty())
    {
        msg = ":localhost 461 PRIVMSG :Not enough parameters\n\r";
        send(client_fd,msg.c_str(), msg.size(), 0);
        std::cout << "empty message\n";
        return "%%{ERROR}%%";
    }

    std::vector<std::string> targets = split(chs, ',');

    std::cout << chs << std::endl;

    if (targets.size() == 0)
    {
         msg = ":localhost 461 PRIVMSG :Not enough parameters\n\r";
        send(client_fd,msg.c_str(), msg.size(), 0);
        std::cout << "empty targets\n";
        return "%%{ERROR}%%";
    }
    payload = joinByMe(targets, '*') + std::string("|") + message;

    std::cout << payload << std::endl;

    return payload;
}

void Commands::privmsg(std::string payload, int client_fd, std::vector<client>::iterator thclient)
{
    std::vector<std::string> parts = split(payload, '|');

    if (parts.size() != 2)
    {
        return;
    }

    std::vector<std::string> targets = split(parts[0], '*');

    std::string message = parts[1];

    for (int i = 0; i < targets.size(); ++i)
    {
        if (targets[i][0] == '#' || targets[i][0] == '&')
        {
            std::vector<channel>::iterator it = get_channel_by_name(targets[i]);
            if (it != channels.end())
            {
                std::cout << "send to channel\n";
                it->broadcast_message(message);
            }
        }
        else
        {
            int it = get_client_id_by_nick(targets[i]);
            if (it != -1)
            {
                std::cout << "send to user\n";
                send(it, message.c_str(), message.length(), 0);
            }
        }
    }
}