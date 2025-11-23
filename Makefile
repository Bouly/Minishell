# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/19 21:13:48 by abendrih          #+#    #+#              #
#    Updated: 2025/11/23 13:56:03 by abendrih         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ============================= CONFIGURATION ============================= #

NAME        = minishell
LIBFT_DIR   = Libft
LIBFT       = $(LIBFT_DIR)/libft.a

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -g3 -Iincludes -I$(LIBFT_DIR)
RM          = rm -rf

SRC_DIR     = src
OBJ_DIR     = obj
SRC         = $(shell find $(SRC_DIR) -name "*.c")
OBJ         = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# ============================== COLORS =================================== #

RESET       = \033[0m
GREEN       = \033[1;32m
RED         = \033[1;31m
CYAN        = \033[1;36m
GRAY        = \033[0;37m

# ============================== RULES ==================================== #

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	@printf "\n$(CYAN)🔧 Building $(NAME)...$(RESET)\n"
	@for i in 10 20 30 40 50 60 70 80 90 100; do \
		printf "\r$(GREEN)[%-50s] %d%%$(RESET)" "$$(printf '#%.0s' $$(seq 1 $$((i/2))))" $$i; \
		sleep 0.03; \
	done
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline
	@printf "\n\n✅ $(NAME) compiled successfully.$(RESET)\n\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -sC $(LIBFT_DIR) --no-print-directory

clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		$(RM) $(OBJ_DIR); \
		$(MAKE) -sC $(LIBFT_DIR) clean --no-print-directory; \
	else \
		printf "$(GRAY)🧼 Everything already clean.$(RESET)\n"; \
	fi

fclean:
	@if [ -f "$(NAME)" ] || [ -d "$(OBJ_DIR)" ]; then \
		printf "\n$(RED)🔥 Purging $(NAME)...$(RESET)\n"; \
		for i in 100 90 80 70 60 50 40 30 20 10 0; do \
			len=$$((i / 2)); \
			printf "\r$(RED)[%-50s] %d%%$(RESET)" "$$(printf '#%.0s' $$(seq 1 $$len))" $$i; \
			sleep 0.03; \
		done; \
		$(RM) $(NAME) $(OBJ_DIR); \
		$(MAKE) -sC $(LIBFT_DIR) fclean --no-print-directory; \
		printf "\n"; \
	else \
		printf "$(GRAY)🧼 Everything already clean.$(RESET)\n"; \
	fi

re: fclean all

run: all
	@printf "$(CYAN)🚀 Running $(NAME)...$(RESET)\n"
	@./$(NAME)

# ============================== META ===================================== #
valgrind: $(NAME)
	@echo "$(YELLOW)🔍 Lancement de Valgrind sur ./minishell..."
	valgrind -q --suppressions=./ignore --trace-children=yes \
		--leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes \
		./minishell
.PHONY: all clean fclean re run
