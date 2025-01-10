

CC = g++
CFLAGS = -g -Wall -Wextra -std=c++98 -Iincludes/
#CFLAGS = -g -Wall -Wextra -Werror -std=c++98 -Iincludes/

SRCS_CGI =		Cgi.cpp 

SRCS_REQUEST =	Request.cpp	\
				request_parser.cpp

SRCS_RESPONSE =	GetMethod.cpp \
				Response.cpp \
				Worker.cpp
#testmain.cpp


SRCS_SERVER =	Connection.cpp \
				Server.cpp \
				Socket.cpp

SRCS_UTILS =	#gen_webpage.cpp
		
SRCS =			main.cpp
#request_parser_test.cpp
#parsing_test.cpp

OBJ_PATH = obj/
SRCS_PATH = srcs/
CGI_PATH = srcs/cgi/
REQUEST_PATH = srcs/request/
RESPONSE_PATH = srcs/response/
SERVER_PATH = srcs/server/
UTILS_PATH = srcs/utils/

SRCS_FILES = $(addprefix $(SRCS_PATH), $(SRCS))
CGI_FILES = $(addprefix $(CGI_PATH), $(SRCS_CGI))
REQUEST_FILES = $(addprefix $(REQUEST_PATH), $(SRCS_REQUEST))
RESPONSE_FILES = $(addprefix $(RESPONSE_PATH), $(SRCS_RESPONSE))
SERVER_FILES = $(addprefix $(SERVER_PATH), $(SRCS_SERVER))
UTILS_FILES = $(addprefix $(UTILS_PATH), $(SRCS_UTILS))

ALL_SRCS = $(SRCS_FILES) $(CGI_FILES) $(REQUEST_FILES) $(RESPONSE_FILES) $(SERVER_FILES) $(UTILS_FILES)
OBJS = $(ALL_SRCS:srcs/%.cpp=$(OBJ_PATH)srcs/%.o)

NAME = webServ

all: $(OBJ_PATH) $(NAME)

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "\r\033[K\033[38;2;0;170;0;1mSuccessfully compiled to \033[38;2;255;145;5;3m./$(NAME)\033[0m\033[38;2;0;170;0;1m!\033[0m"

$(OBJ_PATH)srcs/%.o: srcs/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re