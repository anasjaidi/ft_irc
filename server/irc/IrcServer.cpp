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

    trim_fun(req);
    std::pair<OptionCommands, std::string> command = get_command(req);


    if (command.first == OptionCommands::UNDEFINED) {
        std::cerr << command.second << std::endl;
        return -1;
    }
    std::vector<client>::iterator cl = this->get_client(client_fd);

    if (command.second == "%%{ERROR}%%") {

        remove_client_from_server(client_fd);
        remove_client(client_fd);
        return 1;
    }
    int remove = 0;
    std::vector<client>::iterator it = get_client(client_fd);

    if (it == clients.end() && command.first != OptionCommands::PASS) {
        std::cerr << "internal server error" << std::endl; // client doesn't exist in server
        close(client_fd);
        remove_client_from_server(client_fd);
        return 1;
    } else if (it != clients.end() && it->isAuth1() == false && command.first != OptionCommands::PASS) {
        // user not authenticated
        std::cout << "user not authenticated\n";
        remove_client(client_fd);
        remove_client_from_server(client_fd);
        return 1;
    } else if (it != clients.end() && (it->getNick().empty() || it->getUser().empty()) && command.first != OptionCommands::PASS && command.first != OptionCommands::NICK && command.first != OptionCommands::USER) {
        remove_client(client_fd);
        remove_client_from_server(client_fd);
        return 1;
    }

    switch (command.first) {
        case OptionCommands::PASS:
            remove = pass(command.second, client_fd, server_password);
            break;
        case OptionCommands::NICK:
                remove  = nick(command.second, client_fd);
            break;
        case OptionCommands::USER:
                remove = user(command.second, client_fd);
            break;
//        case OptionCommands::KICK:
//            kick(command.second, client_fd);
//            break;
//        case OptionCommands::PART:
//            part(command.second, client_fd);
//            break;
//        case OptionCommands::BOT:
//            bot(command.second, client_fd);
//            break;
        case OptionCommands::MODE:
            mode(command.second, client_fd, (t_join_client){.nick=cl->getNick(), .user=cl->getUser(), .info=(struct sockaddr_in*)&cl->getTheirAddr()});
            break;
//        case OptionCommands::INVITE:
//            invite(command.second, client_fd);
//            break;
        case OptionCommands::PRIVATE_MSG:
//            struct sockaddr_storage addrInfos = cl->getTheirAddr();
                privmsg(command.second, client_fd);
            break;
        case OptionCommands::JOIN:
            struct sockaddr_storage addrInfos = cl->getTheirAddr();
            join(command.second, client_fd, (t_join_client){.nick=cl->getNick(), .user=cl->getUser(), .info=(struct sockaddr_in*)&cl->getTheirAddr()});
            render_channels();
            break;
    }
    if (remove) {
        remove_client_from_server(client_fd);
        remove_client(client_fd);
    }
    clients[0].getTheirAddr();
}

int IrcServer::signup(std::pair<struct sockaddr_storage, int> &new_client) {
    add_client(new_client.second, new_client.first);
}