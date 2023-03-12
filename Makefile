name = Fixed
flags = -Wall -Wextra -Werror -std=c++98
src =

obj = $(src:%.cpp=%.o)

all : $(name)

$(name): $(obj) main.cpp
	c++ $(flags) main.cpp  $(obj) -o $(name)

%.o: %.cpp
	c++ $(flags) -c $< -o $@

clean:
	rm -rf $(obj)

fclean: clean
	rm -f $(name)

re:	fclean all