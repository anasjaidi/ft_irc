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
    std::vector<client>::iterator cl = this->get_client(client_fd);
    std::cout << "==========>" << req << "<==========" << "\n";
    std::pair<OptionCommands, std::string> command = get_command(req, client_fd, cl);

    if (command.first == UNDEFINED) { 
        std::cerr << command.second << std::endl; // TODO: UNDIFINED COMMAND ERROR
        return -1;
    }


    if (command.second == "%%{ERROR}%%") {

        remove_client_from_server(client_fd);
        remove_client(client_fd);
        return 1;
    }

    int remove = 0;
    std::vector<client>::iterator it = get_client(client_fd);

    if (it == clients.end() && command.first != PASS) {
        std::cerr << "internal server error" << std::endl; // client doesn't exist in server
        close(client_fd);
        remove_client_from_server(client_fd);
        return 1;
    } else if (it != clients.end() && it->isAuth1() == false && command.first != PASS) {
        // user not authenticated
        std::cout << "user not authenticated\n"; // TODO: CLIENT IS NOT AUTHENTICATED
        remove_client(client_fd);
        remove_client_from_server(client_fd);
        return 1;
    } else if (it != clients.end() && (it->getNick().empty() || it->getUser().empty()) && command.first != PASS && command.first != NICK && command.first != USER) {
        remove_client(client_fd); // TODO: COMMAND WITHOUT INDETIFICATION
        remove_client_from_server(client_fd);
        return 1;
    }

    switch (command.first) {
        case UNDEFINED:
        break;
        case PASS:
            remove = pass(command.second, client_fd, server_password, cl);
            break;
        case TOPIC:
            topic(command.second, client_fd, cl);
            break;
        case NICK:
                remove  = nick(command.second, client_fd, cl);
            break;
        case USER:
                remove = user(command.second, client_fd, cl);
            break;
       case KICK:
           kick(command.second, client_fd, cl);
           break;
//        case OptionCommands::PART:
//            part(command.second, client_fd);
//            break;
       case BOT:
           bot(command.second, client_fd, cl);
           break;
        case MODE:
            mode(command.second, client_fd, (t_join_client){.nick=cl->getNick(), .user=cl->getUser(), .info=(struct sockaddr_in*)&cl->getTheirAddr()});
            break;
       case INVITE:
           invite(command.second, client_fd, cl);
           break;
        case PRIVATE_MSG:
//            struct sockaddr_storage addrInfos = cl->getTheirAddr();
                privmsg(command.second,client_fd, (t_join_client){.nick=cl->getNick(), .user=cl->getUser(), .info=(struct sockaddr_in*)&cl->getTheirAddr()}, cl);
            break;
        case NOTICE:
                notice(command.second,client_fd, (t_join_client){.nick=cl->getNick(), .user=cl->getUser(), .info=(struct sockaddr_in*)&cl->getTheirAddr()}, cl);
            break;
        case JOIN:
            join(command.second, client_fd, (t_join_client){.nick=cl->getNick(), .user=cl->getUser(), .info=(struct sockaddr_in*)&cl->getTheirAddr()}, cl);
            render_channels();
            break;
    }
    std::cout << "remove: " << remove << std::endl;
    if (remove) {
        remove_client_from_server(client_fd);
        remove_client(client_fd);
    }
    return 0;
}

int IrcServer::signup(std::pair<struct sockaddr_storage, int> &new_client) {
    return add_client(new_client.second, new_client.first);

}