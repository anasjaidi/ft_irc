#include "./IrcServer.hpp"

IrcServer::IrcServer() : Server()
{
  std::cout << "Default constructor called \n";
}

IrcServer::~IrcServer()
{
  std::cout << "Destructor called \n";
}

IrcServer::IrcServer(int family, int socket_type, const char *service, std::string server_pass) : Server(family, socket_type, service), server_password(server_pass)
{
}

IrcServer::IrcServer(const char *node, int family, int socket_type, const char *service, std::string server_pass) : Server(node, family, socket_type, service), server_password(server_pass)
{
}
