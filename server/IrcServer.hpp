//
// Created by anas jaidi on 30/3/2023.
//

#ifndef FT_IRC_IRCSERVER_HPP
#define FT_IRC_IRCSERVER_HPP

#include "../FT_IRC.h"

# include "../abstract/server/Server.hpp"

class IrcServer : public Server
{
protected:
  IrcServer();

  std::string server_password;

  std::string server_name;

public:
  ~IrcServer();
  IrcServer(int family, int socket_type, const char *service, std::string server_pass);
  IrcServer(const char *node, int family, int socket_type, const char *service, std::string server_pass);
  /*
   * start Exceptions
   **/

  virtual int handle(std::string, int) throw(ServerErrors);
  int signup(std::pair<struct sockaddr_storage, int> &);
};

#endif // FT_IRC_IRCSERVER_HPP
