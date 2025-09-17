NAME = minishell

CC = cc
CFLAGS = -g -Wall -Werror -Wextra -I$(INC) -I$(LIBFT_INC) -I$(PRINTF_INC)

INC = ./inc/
LIBFT_INC = ./libraries/
PRINTF_INC = ./libraries/
LIBFT = ./libraries/libft.a
PRINTF = ./libraries/libftprintf.a

REMOVE = rm -f

SRCS = \
./srcs/main.c \
./srcs/utils.c \
./srcs/builtins/cd.c \
./srcs/builtins/echo.c \
./srcs/builtins/env.c \
./srcs/builtins/exit.c \
./srcs/builtins/export.c \
./srcs/builtins/pwd.c \
./srcs/builtins/unset.c \
./srcs/builtins/parse_command.c \

OBJS = $(SRCS:.c=.o)

# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
CYAN = \033[0;36m
RESET = \033[0m

all: $(NAME)
	@echo "$(GREEN)Compilation successfully done!!$(RESET)"


$(NAME): $(OBJS)
	@echo "$(CYAN)Linking minishell...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT) $(PRINTF) -lreadline
	@echo "$(GREEN)Minishell executable created successfully!$(RESET)"

%.o: %.c
	@echo "$(CYAN)Compiling $<...$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW)Cleaning object files...$(RESET)"
	$(REMOVE) $(OBJS)

fclean: clean
	@echo "$(YELLOW)Removing executables...$(RESET)"
	$(REMOVE) $(NAME)

re: fclean all

.PHONY: all clean fclean re
