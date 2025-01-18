CC = g++

# Compilation flags
CFLAGS = -g -Wall -Wextra -Werror -std=c++98 -Iincludes/

# Source files
SRCS_CGI =		Cgi.cpp

SRCS_REQUEST =	Request.cpp \
				request_parser.cpp

SRCS_RESPONSE =	GetMethod.cpp \
				PostMethod.cpp \
				DeleteMethod.cpp \
				Response.cpp \
				Worker.cpp

SRCS_SERVER =	Connection.cpp \
				Server.cpp \
				Socket.cpp

SRCS_UTILS =	run.cpp

SRCS =			main.cpp


# Paths
OBJ_PATH = obj/
SRCS_PATH = srcs/
CGI_PATH = srcs/cgi/
REQUEST_PATH = srcs/request/
RESPONSE_PATH = srcs/response/
SERVER_PATH = srcs/server/
UTILS_PATH = srcs/utils/
UPLOADS_PATH = ./html_page/www/uploads/

# Full paths to source files
SRCS_FILES = $(addprefix $(SRCS_PATH), $(SRCS))
CGI_FILES = $(addprefix $(CGI_PATH), $(SRCS_CGI))
REQUEST_FILES = $(addprefix $(REQUEST_PATH), $(SRCS_REQUEST))
RESPONSE_FILES = $(addprefix $(RESPONSE_PATH), $(SRCS_RESPONSE))
SERVER_FILES = $(addprefix $(SERVER_PATH), $(SRCS_SERVER))
UTILS_FILES = $(addprefix $(UTILS_PATH), $(SRCS_UTILS))

ALL_SRCS = $(SRCS_FILES) $(CGI_FILES) $(REQUEST_FILES) $(RESPONSE_FILES) $(SERVER_FILES) $(UTILS_FILES)
OBJS = $(ALL_SRCS:srcs/%.cpp=$(OBJ_PATH)srcs/%.o)

# Executable name
NAME = webServ

# Image URLs and destinations
IMAGE_URL = https://upload.wikimedia.org/wikipedia/commons/thumb/d/d6/El_oriegen_2010.png/800px-El_oriegen_2010.png?20150804032438
IMAGE_DEST = $(UPLOADS_PATH)inception.png
IMAGE_URL2 = https://e7.pngegg.com/pngimages/902/338/png-clipart-poke-ball-open-wv-computer-wallpaper-desktop-wallpaper-thumbnail.png
IMAGE_DEST2 = $(UPLOADS_PATH)pokeball.png

# Default target
all: $(OBJ_PATH) $(NAME) destructor free download_images

# Target to compile destructor utility
destructor: 
	$(CC) $(CFLAGS) -o destructor $(UTILS_PATH)destructor.cpp

# Target to compile free utility
free: 
	$(CC) $(CFLAGS) -o free $(UTILS_PATH)free.cpp

# Create object directory and touch test files
$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

# Ensure the uploads directory exists
$(UPLOADS_PATH):
	mkdir -p $(UPLOADS_PATH)

# Download images
download_images: $(UPLOADS_PATH) $(IMAGE_DEST) $(IMAGE_DEST2)

$(IMAGE_DEST): $(UPLOADS_PATH)
	@wget -q $(IMAGE_URL) -O $(IMAGE_DEST)
	@echo "This is a test file." > $(UPLOADS_PATH)test.txt
	@echo "This is a test copy file." > $(UPLOADS_PATH)test-copy.txt
	@echo "This is a youpi.bla file." > $(UPLOADS_PATH)youpi.bla

$(IMAGE_DEST2): $(UPLOADS_PATH)
	@wget -q $(IMAGE_URL2) -O $(IMAGE_DEST2)

# Compile the main executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "\r\033[K\033[38;2;0;170;0;1mSuccessfully compiled to \033[38;2;255;145;5;3m./$(NAME)\033[0m\033[38;2;0;170;0;1m!\033[0m"

# Compile object files
$(OBJ_PATH)srcs/%.o: srcs/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files and directories
clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_PATH)

# Full clean including images and executables
fclean: clean
	rm -f $(IMAGE_DEST) $(IMAGE_DEST2)
	rm -f $(UPLOADS_PATH)test-copy.txt
	rm -f $(UPLOADS_PATH)test.txt
	rm -f $(UPLOADS_PATH)youpi.bla
	rm -f $(NAME) destructor free

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re download_images