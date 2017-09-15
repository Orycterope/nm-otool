# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/12/18 19:13:31 by tvermeil          #+#    #+#              #
#    Updated: 2017/09/15 14:40:30 by tvermeil         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += --no-print-directory

NAME = nm-otool
NM_NAME = ft_nm
OTOOL_NAME = ft_otool

SRC_PATH = src
HEADER_PATH = include/nm-otool
PRIVATE_HEADER_PATH = $(SRC_PATH)
ROOT = .
OBJ_FOLDER = obj/$(NAME)
INC_PATH =	$(ROOT)/include/libft \
			$(HEADER_PATH) \
			$(PRIVATE_HEADER_PATH) \

OBJ_PATH = $(ROOT)/$(OBJ_FOLDER)
DST_PATH = $(ROOT)
LFT_PATH = $(ROOT)

CC = gcc -g
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = $(addprefix -I ,$(INC_PATH))
LDFLAGS = -L$(LFT_PATH)
LDLIBS = -lft 

NM_SRC_NAME = 	nm.c \
				fat.c \
				map.c \
				ar.c \
				macho.c \
				nlist.c \
				print.c \

OTOOL_SRC_NAME =	otool.c \
					fat.c \
					map.c \
					ar.c \
					macho.c \

HEADER_NAME =	\

PRIVATE_HEADER_NAME =	nm_otool.h \

NM_OBJ_NAME = 		$(NM_SRC_NAME:.c=.o)
OTOOL_OBJ_NAME = 	$(OTOOL_SRC_NAME:.c=.o)
OBJ_NAME = $(NM_OBJ_NAME) $(OTOOL_OBJ_NAME)

NM_SRC = 	$(addprefix $(SRC_PATH)/,$(NM_SRC_NAME))
OTOOL_SRC =	$(addprefix $(SRC_PATH)/,$(OTOOL_SRC_NAME))

NM_OBJ =	$(addprefix $(OBJ_PATH)/,$(NM_OBJ_NAME))
OTOOL_OBJ =	$(addprefix $(OBJ_PATH)/,$(OTOOL_OBJ_NAME))
OBJ = $(NM_OBJ) $(OTOOL_OBJ)

#DST = $(addprefix $(DST_PATH)/,$(NAME))
HEADER_FILES = $(addprefix $(HEADER_PATH)/,$(HEADER_NAME)) \
			   $(addprefix $(SRC_PATH)/,$(PRIVATE_HEADER_NAME))

all: $(ROOT)/libft.a nm otool #$(DST)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(HEADER_FILES) libft.a
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@echo -n .

nm: $(NM_NAME)

otool: $(OTOOL_NAME)

$(NM_NAME): $(NM_OBJ) $(HEADER_FILES)
	@$(CC) $(NM_OBJ) $(LDFLAGS) $(LDLIBS) -o $(NM_NAME)
	@echo " . \033[32m$(NM_NAME)\033[0m done"

$(OTOOL_NAME): $(OTOOL_OBJ) $(HEADER_FILES)
	@$(CC) $(OTOOL_OBJ) $(LDFLAGS) $(LDLIBS) -o $(OTOOL_NAME)
	@echo " . \033[32m$(OTOOL_NAME)\033[0m done"


#$(DST): $(OBJ) $(HEADER_FILES)
#	@$(CC) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $(DST)
#	@echo " . \033[32m$(NAME)\033[0m done"

libft.a:
	@make -C libft all

clean:
	@-cd $(ROOT) && rm -fv $(addprefix $(OBJ_FOLDER)/,$(OBJ_NAME))
	@rmdir $(OBJ_PATH) $(ROOT)/obj 2> /dev/null || true
	@make -C libft clean

fclean: clean
	@-cd $(DST_PATH) && rm -fv $(NM_NAME) $(OTOOL_NAME)
	@make -C libft fclean

re: fclean all

ac: all clean

norme:
	@norminette $(SRC_PATH) $(HEADER_FILES) | grep --color=always -B 1 'Error' || echo "\033[32m$(NAME)\033[0m norme OK"
	@make -C libft norme

.PHONY: all clean fclean re
