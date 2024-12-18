

CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -I/includes/WebServ.hpp

SRCS_CGI =		cgi_handler.cpp

SRCS_REQUEST =	request_parser.cpp \
				Request.cpp	

SRCS_RESPONSE =	DeleteMethod.cpp \
				GetMethod.cpp \
				PostMethod.cpp \
				Response.cpp \
				Worker.cpp

SRCS_SERVER =	config_parser.cpp \
				Connection.cpp \
				Server.cpp \
				Socket.cpp

SRCS_UTILS =	gen_webpage.cpp
		
SRCS =			main.cpp

OBJ_PATH = obj/
SRCS_PATH = src/
CGI_PATH = src/cgi
REQUEST_PATH = src/request
RESPONSE_PATH = src/response
SERVER_PATH = src/server

SRCS_FILES = $(addprefix $(SRCS_PATH), $(SRCS))
CGI_FILES = $(addprefix $(CGI_PATH), $(SRCS_CGI))
REQUEST_FILES = $(addprefix $(REQUEST_PATH), $(SRCS_REQUEST))
RESPONSE_FILES = $(addprefix $(RESPONSE_PATH), $(SRCS_RESPONSE))
SERVER_FILES = $(addprefix $(SERVER_PATH), $(SRCS_SERVER))
UTILS_FILES = $(addprefix $(UTILS_PATH), $(SRCS_UTILS))

ALL_SRCS = $(SRCS_FILES) $(PARSE_FILES) $(UTILS_FILES)
OBJS = $(ALL_SRCS:$(SRCS_PATH)%.c=$(OBJ_PATH)%.o)

NAME = WebServ

all: $(OBJ_PATH) $(NAME)


${OBJ_PATH}:
		mkdir -p ${OBJ_PATH}

${NAME}: ${OBJS}
			${CC} ${CFLAGS} -o $@ $^
	@echo "\r\033[K\033[38;2;0;170;0;1mSuccessfully compiled to \033[38;2;255;145;5;3m./$(NAME)\033[0m\033[38;2;0;170;0;1m!\033[0m"

${OBJ_PATH}%.o: %.cpp
			${CC} ${CFLAGS} -c $< -o $@

clean:
			rm -f ${OBJS}
			rm -rf ${OBJ_PATH}

fclean: clean
			rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re