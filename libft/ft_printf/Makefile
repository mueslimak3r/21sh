# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: calamber <calamber@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/17 11:20:37 by calamber          #+#    #+#              #
#    Updated: 2019/07/01 18:29:34 by calamber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ../libft.a

SRC = ft_printf.c tools.c uitoa_base.c print_buffer.c \
parse_u.c flags.c parse_chars.c parse_d.c parse_wchar.c

SRC_PATH = src/

SRC_POS = $(addprefix $(SRC_PATH),$(SRC))

OBJ = $(SRC:.c=.o)

INC = -I includes

CC = gcc

FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	@ar rc $(NAME) $(OBJ) ../*.o
	@ranlib $(NAME)

$(OBJ):
	@$(CC) $(FLAGS) $(INC) -c $(SRC_POS)

clean:
	@make clean -C ../
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

compile: re
	@$(CC) $(FLAGS) -g -fsanitize=address $(INC) mymain.c $(NAME)
	@make clean

cleanup: fclean
	@rm -rf *.out*

.PHONY : all, re, clean, fclean, cleanup
