# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/29 16:23:58 by maborges          #+#    #+#              #
#    Updated: 2024/11/29 17:48:17 by maborges         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# My program name
NAME = libft.a

#Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

#Source files
SRCS =	ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
	ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
	ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_putstr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_strndup.c ft_striteri.c ft_strjoin.c ft_strjoin_free.c \
	ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strcmp.c ft_strncmp.c \
	ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \

# Object files
OBJS = $(SRCS:.c=.o)

#Default rule to compile the program
all: $(NAME)

#Rule to link the object files into executable, create static lib and update the index of static lib
$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)
	ranlib $(NAME)

#Rule to compile the source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#For make main
main: $(NAME)
	$(CC) $(CFLAGS) main.c $(NAME) -o main

#Clean rule to remove object files
clean:
	rm -f $(OBJS)

#Fclean rule to remove object files and executable
fclean: clean
	rm -f $(NAME)

#Rule to clean and recompile everything
re: fclean all

#Phony targets to prevent conflicts with files
.PHONY: clean fclean re
