# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aybelaou <aybelaou@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/30 16:59:31 by aybelaou          #+#    #+#              #
#    Updated: 2025/05/08 19:10:26 by aybelaou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -----------------------------------Colors------------------------------------

ORANGE				= \033[38;2;255;165;0m
BOLD_ORANGE			= \033[1;38;5;214m
BG_YELLOW			= \033[48;5;214m
MAGENTA				= \033[0;35m
BOLD_MAGENTA		= \033[1;35m
RESET				= \033[0m

# ---------------------------------Compilation---------------------------------

NAME				= philo
CC					= cc
CFLAGS				= -Werror -Wall -Wextra -pthread -I inc
RM					= rm -rf

OBJ_DIR				= obj
SRC_DIR				= src
SRC					= $(SRC_DIR)/main.c \
						$(SRC_DIR)/utils.c \
						$(SRC_DIR)/init.c \
						$(SRC_DIR)/routine.c \
						$(SRC_DIR)/monitor.c
OBJ					= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# -----------------------------------Rules-------------------------------------

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(ORANGE)Building $(NAME)...$(RESET)"
	@$(CC) $(OBJ) $(CFLAGS) -o $(NAME)
	@echo "$(BOLD_ORANGE)$(NAME) is ready!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(ORANGE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

fclean: clean
	@echo "$(MAGENTA)Removing the executable...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(BOLD_MAGENTA)Executable removed!$(RESET)"

clean:
	@echo "$(MAGENTA)Cleaning object files...$(RESET)"
	@$(RM) -r $(OBJ_DIR)
	@echo "$(BOLD_MAGENTA)All object files cleaned!$(RESET)"

re: fclean all
	@echo "$(BG_YELLOW)Cleaned and rebuilt everything!$(RESET)"

.PHONY: all clean fclean re