NAME = ircserver

SRCS = main.cpp Server.cpp User.cpp Channel.cpp ServerUtils.cpp \
		Commandes/Join.cpp \
		Commandes/Pass.cpp \
		Commandes/Cap.cpp \
		Commandes/User.cpp \
		Commandes/Nick.cpp \
		Commandes/Quit.cpp \
		Commandes/Msg.cpp \
		Commandes/Kick.cpp \
		Commandes/Invite.cpp \
		Commandes/Topic.cpp \

OBJS = $(SRCS:.cpp=.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re