NAME = minishell

CC = cc
CFLAGS = -g -Wall -Werror -Wextra -I$(INC)

INC = ./inc/
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

REMOVE = rm -f

#Commented out files for executor implementation
#./srcs/executor/mockup_parser.c DELETE BEFORE RELEASE	!!!
#


SRCS = \
./srcs/main.c \
./srcs/parser/parser.c \
./srcs/parser/tokenizer.c \
./srcs/parser/tokenizer_helpers.c \
./srcs/parser/parse_helpers.c \
./srcs/parser/ast.c \
./srcs/utils/utils.c \
./srcs/utils/debug_functions.c \
./srcs/utils/str_append.c \
./srcs/utils/quote_utils.c \
./srcs/utils/expand_functions.c \
./srcs/builtins/cd.c \
./srcs/builtins/echo.c \
./srcs/builtins/env.c \
./srcs/builtins/exit.c \
./srcs/builtins/export.c \
./srcs/builtins/pwd.c \
./srcs/builtins/unset.c \
./srcs/executor/executor.c \
./srcs/error.c

OBJS = $(SRCS:.c=.o)

# Colors
GREEN = \033[0;32m
YELLOW = \033[0;33m
CYAN = \033[0;36m
RESET = \033[0m

all: $(NAME)
	@echo "$(GREEN)Compilation successfully done!!$(RESET)"


$(NAME): $(OBJS) $(LIBFT)
	@echo "$(CYAN)Linking minishell...$(RESET)"
	$(CC) $(CFLAGS) $(OBJS) ./libft/libft.a -o $(NAME) -lreadline
	@echo "$(GREEN)Minishell executable created successfully!$(RESET)"

$(LIBFT):
	make -C $(LIBFT_DIR)

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
