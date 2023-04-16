//
// Created by anas jaidi on 30/3/2023.
//

#include "IrcServer.hpp"

IrcServer::IrcServer()  : Server() {
    std::cout << "Default constructor called \n";
}

IrcServer::~IrcServer() {
    std::cout << "Destructor called \n";
}

IrcServer::IrcServer(int family, int socket_type, const char *service, std::string server_pass) : Server(family, socket_type, service), server_password(server_pass) {
}

IrcServer::IrcServer(const char *node, int family, int socket_type, const char *service , std::string server_pass) : Server(node, family, socket_type, service), server_password(server_pass) {
}


const char *IrcServer::SeverErrors::what() const throw() {

    static std::string error; // use a static variable to avoid returning a stack address
    error.clear(); // clear the string before each use

    switch (errorCode) {
        case SeverErrors::AddrInfoError :
            error = "GetAddrInfo Function Failed: ";
            break;
        case SeverErrors::SocketFdError :
            error = "GetSocketFd Function Failed: ";
            break;
        case SeverErrors::BindFdError :
            error = "BindSocketFd Function Failed: ";
            break;
        case SeverErrors::ListenError :
            error = "Listen Function Failed: ";
            break;
        default:
            return "Undefined Error!";
    }

    error += std::strerror(errno); // append the system error message to the error string
    return error.c_str();
}

IrcServer::SeverErrors::SeverErrors(ErrorCode _errorCode) : errorCode(_errorCode) {}



int IrcServer::handle(std::string req, int client_fd) throw() {

    req = req.substr(req.find_first_not_of(" \t\r\n\v\f"));
    req = req.substr(0, req.find_last_not_of(" \t\r\n\v\f") + 1);
    std::pair<OptionCommands, std::string> command = get_command(req);

    if (command.first == OptionCommands::UNDEFINED) {
        std::cerr << command.second << std::endl;
        return -1;
    }
    std::vector<client>::iterator cl = this->get_client(client_fd);

    switch (command.first) {
        case OptionCommands::PASS:
            pass(command.second, client_fd, server_password);
            break;
        case OptionCommands::NICK:
            nick(command.second, client_fd);
            break;
        case OptionCommands::KICK:
            kick(command.second, client_fd);
            break;
        case OptionCommands::USER:
            user(command.second, client_fd);
            break;
        case OptionCommands::PART:
            part(command.second, client_fd);
            break;
        case OptionCommands::BOT:
            bot(command.second, client_fd);
            break;
        case OptionCommands::MODE:
            mode(command.second, client_fd, (t_join_client){.nick=cl->getNick(), .user=cl->getUser(), .info=(struct sockaddr_in*)&cl->getTheirAddr()});
            break;
        case OptionCommands::INVITE:
            invite(command.second, client_fd);
            break;
        case OptionCommands::JOIN:
            struct sockaddr_storage addrInfos = cl->getTheirAddr();
            join(command.second, client_fd, (t_join_client){.nick=cl->getNick(), .user=cl->getUser(), .info=(struct sockaddr_in*)&cl->getTheirAddr()});
            render_channels();
            break;
    }
    clients[0].getTheirAddr();
}

int IrcServer::signup(std::pair<struct sockaddr_storage, int> &new_client) {
    add_client(new_client.second, new_client.first);
}