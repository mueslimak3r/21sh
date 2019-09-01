# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: calamber <calamber@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/29 23:01:53 by calamber          #+#    #+#              #
#    Updated: 2019/07/08 04:39:28 by calamber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
INCLUDE = libft.h
FLAGS = -Wall -Wextra -Werror -g #-fsanitize=address

SRC = ft_memset.c ft_bzero.c ft_memcpy.c ft_memccpy.c ft_memmove.c \
ft_memchr.c ft_memcmp.c ft_strlen.c ft_strdup.c ft_strcpy.c \
ft_strncpy.c ft_strcat.c ft_strncat.c ft_strlcat.c ft_strchr.c \
ft_strrchr.c ft_strstr.c ft_strnstr.c ft_strcmp.c ft_strncmp.c \
ft_atoi.c ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
ft_isprint.c ft_toupper.c ft_tolower.c ft_memalloc.c ft_memdel.c \
ft_strnew.c ft_strdel.c ft_strclr.c ft_striter.c ft_striteri.c \
ft_strmap.c ft_strmapi.c ft_strequ.c ft_strnequ.c ft_strsub.c \
ft_strjoin.c ft_strtrim.c ft_strsplit.c ft_itoa.c ft_putchar.c \
ft_putstr.c ft_putendl.c ft_putnbr.c ft_putchar_fd.c ft_putstr_fd.c \
ft_putendl_fd.c ft_putnbr_fd.c ft_lstnew.c ft_lstdelone.c ft_lstdel.c \
ft_lstadd.c ft_lstiter.c ft_lstmap.c ft_cntwords.c ft_isupper.c \
ft_strcase.c ft_itoa_base.c get_next_line.c ft_arraydel.c ft_makepath.c \
ft_strndup.c ft_returnfree.c ft_readstdin_line.c ft_checkifc.c ft_realloc.c ft_atoi_base.c \
ft_isspace.c

OBJ = $(SRC:.c=.o)

all:
	@$(CC) $(FLAGS) -c $(SRC)
	@make -C ft_printf

clean:
	@rm -f *.o

fclean:
	@rm -f *.o
	@make fclean -C ft_printf

re: fclean all

.PHONY: all clean fclean re
