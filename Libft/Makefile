# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abendrih <abendrih@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/30 20:38:49 by abendrih          #+#    #+#              #
#    Updated: 2025/10/29 09:15:30 by abendrih         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#                                     ,-="""=.
#                                   .'        `.
#                                  (            `.
#                                   `.            `..
#                                    ,'             .'
#                                    `.            '.
#                                      `-.           `-.
#                                         )             `=-.
#                                       .'              `=-.
#                                     .`               .`-.
#                       _            (                \ `-.
#                    ,'   `.          `.        /`.    \
#                   /        `.         \      |   `.   `.
#                 ,'            `.       )    /      \    \
#                /     .'`.        `.    )    |       `.   \
#              ,'    .'    `.         `./     \         `.  \
#            ,'    .'        `.                \          \  \
#          ,'    .'            `.               \          `. \
#        ,'   .'                 `.              )          ) (__.
#      ,'   (                      `.            )          `."""'
#  _.-'    __)                       `.         .  
# `""'""                               `"""""""

# ============================= CONFIGURATION ============================= #

EXEC        = main
NAME        = libft.a

SRC_LIBFT = \
	ft_lstnew.c ft_lstadd_front.c ft_lstsize.c \
	ft_lstlast.c ft_lstadd_back.c ft_lstclear.c \
	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c \
	ft_strlcat.c ft_tolower.c ft_toupper.c ft_strchr.c ft_strrchr.c \
	ft_strncmp.c ft_strcmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_strdup.c \
	ft_calloc.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
	ft_strmapi.c ft_striteri.c ft_putnbr_fd.c ft_putchar_fd.c ft_putstr_fd.c \
	ft_putendl_fd.c ft_strndup.c ft_atol.c ft_free.c ft_error.c

SRC_LIBFT   := $(addprefix ft_lib/, $(SRC_LIBFT))

SRC_PRINTF  := $(addprefix ft_printf/, \
	ft_printf.c ft_convertf.c ft_put_unsigned_nbrf.c ft_putnbrf.c \
	ft_putstrf.c ft_puthexaf.c ft_putcharf.c)

SRC_GNL     := $(addprefix get_next_line/, \
	get_next_line.c)

SRC         = $(SRC_LIBFT) $(SRC_PRINTF) $(SRC_GNL)
OBJ         = $(SRC:.c=.o)

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror
RM          = rm -f

# ============================== COLORS =================================== #

RESET       = \033[0m
GREEN       = \033[1;32m
RED         = \033[1;31m
CYAN        = \033[1;36m

# ============================== RULES ==================================== #

all: $(NAME)
	@printf "\n$(CYAN)🔧 Building $(NAME)...$(RESET)\n"
	@for i in 10 20 30 40 50 60 70 80 90 100; do \
		printf "\r$(GREEN)[%-50s] %d%%$(RESET)" "$$(printf '#%.0s' $$(seq 1 $$((i/2))))" $$i; \
		sleep 0.03; \
	done; \
	printf "\n\n✅ $(NAME) compiled successfully.$(RESET)\n\n"

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ)

fclean: clean
	@printf "\n$(RED)🔥 Purging $(NAME)...$(RESET)\n"
	@for i in 100 90 80 70 60 50 40 30 20 10 0; do \
		len=$$((i / 2)); \
		printf "\r$(RED)[%-50s] %d%%$(RESET)" "$$(printf '#%.0s' $$(seq 1 $$len))" $$i; \
		sleep 0.03; \
	done; \
	$(RM) $(NAME) $(EXEC); \
	printf "\n"

re: fclean all

run: all
	@printf "$(CYAN)⚙️  Compiling $(EXEC) with $(NAME)...$(RESET)\n"
	@$(CC) $(CFLAGS) $(EXEC).c -L. -lft -o $(EXEC)
	@printf "$(GREEN)👾 Launching $(EXEC)...$(RESET)\n"
	@./$(EXEC)

# ============================== META ===================================== #

.PHONY: all clean fclean re run
