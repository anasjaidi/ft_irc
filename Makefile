name = ircserv
flags = -Wall -Wextra -Werror -std=c++98
src = main.cpp

obj = $(src:%.cpp=%.o)

all : $(name)

$(name): $(obj)
	c++ $(flags)  $(obj) -o $(name)

%.o: %.cpp
	c++ $(flags) -c $< -o $@

clean:
	rm -rf $(obj)

fclean: clean
	rm -f $(name)

re:	fclean all