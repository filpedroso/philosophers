# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/16 15:06:56 by fpedroso          #+#    #+#              #
#    Updated: 2025/08/16 15:06:56 by fpedroso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compilation Variables
NAME = philo
CC = cc
CFLAGS = -Wextra -Wall -Werror -g

# Source files
FILES =		general_utils.c		\
			monitor.c			\
			parse_args.c		\
			philosophers.c		\
			prep_table.c		\
			routine.c			\
			routine_utils.c		\
			start_simulation.c	\
			use_forks.c


# Object files
OBJ = $(FILES:.c=.o)

# Verbose flag
VB	= 0
ifeq ($(VB),1)
	AT =
else
	AT = @
endif


# Colors
BLACK	= \033[1;30m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
MAGENTA	= \033[1;35m
CYAN	= \033[1;36m
WHITE	= \033[1;37m
NC		= \033[0m


# Rules
%.o: %.c
	@printf "$(GREEN)O|||"
	$(AT) $(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(AT) $(CC) $(CFLAGS) -o $@ $^
	@printf "\n"
	@printf "$(MAGENTA)philosophers compiled.\n"


all: $(NAME)

clean:
	$(AT) rm -f $(OBJ)
	@printf "$(BLACK)object files deleted.\n"

fclean: clean
	$(AT) rm -f $(NAME)
	@printf "$(RED)executables deleted.\n"

re: fclean all

.PHONY: all clean fclean re
