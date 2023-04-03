#include "FT_IRC.h"
# include "./abstract/server/Server.hpp"
# include "./server/irc/IrcServer.hpp"
#include "./abstract/socket/Socket.hpp"

int main(int argc, char *argv[])
{
    // TODO: MUSTAFA CLEAN CODE
    if (argc != 3)
    {
        std::cout << "ERROR :\nthe argument is not correct please you have to enter lik this :" << std::endl;
        std::cout << "./ircserv <port> <password>" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "welcome in our program\n";


    try {

//        Server *irc = new Server("www.google.com", AF_UNSPEC, SOCK_STREAM, "80" );
        Server *irc = new IrcServer(AF_UNSPEC, SOCK_STREAM, argv[1], argv[2]);

        irc->get_socket_fd();

        irc->bind_socket_fd();

        irc->listen_to_socket();

        irc->accept_incoming_requests();

    } catch (std::exception &err) {
        std::cerr << "error occurred: ";
        std::cerr << err.what() << std::endl;
    }

    (void)argv;
    return (0);
}
