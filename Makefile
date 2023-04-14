name = ircserv
flags = -Wall -Wextra -Werror -std=c++98
src = main.cpp abstract/server/Server.cpp abstract/socket/Socket.cpp server/irc/IrcServer.cpp models/client/client.cpp server/irc/commands/Commands.cpp models/client/client_manager/client_manager.cpp models/client/channel/channel_manager/channel_manager.cpp models/client/channel/channel.cpp server/irc/commands/cmd/kickCmd.cpp server/irc/commands/cmd/invite_cmd.cpp
G = '\033[92m'
R = '\033[91m'
obj = $(src:%.cpp=%.o)
all : $(name)
$(name): $(obj)
	c++ $(flags)  $(obj) -o $(name)
%.o: %.cpp  models/client/client_manager/client_manager.hpp FT_IRC.h  abstract/server/Server.hpp  abstract/socket/Socket.hpp  server/irc/IrcServer.hpp  models/client/client.hpp  server/irc/commands/Commands.hpp
	c++ $(flags) -c $< -o $@
clean:
	rm -rf $(obj)
	@echo $(R) "deleted !" $(NONE)
fclean: clean
	rm -f $(name)
	@echo $(R) "$(name) has ben deleted !" $(NONE)
re:	fclean all
.PHONY: all clean fclean re