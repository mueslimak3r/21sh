# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: calamber <calamber@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/17 11:20:37 by calamber          #+#    #+#              #
#    Updated: 2019/10/25 07:03:01 by calamber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#NAME = ../libft.a

SRC := ft_printf.c \
		tools.c \
		print_buffer.c \
		parse_u.c \
		flags.c \
		parse_chars.c \
		parse_d.c \
		parse_wchar.c

SRC_PATH := ft_printf/src/

SRC_LIST += $(addprefix $(SRC_PATH),$(SRC))

#OBJ = $(SRC:.c=.o)

INCLUDE += -Ift_printf/includes

#CC = gcc

#FLAGS = -Wall -Wextra -Werror

# all: $(NAME)

# $(NAME): $(OBJ)
# 	@ar rc $(NAME) $(OBJ) ../*.o
# 	@ranlib $(NAME)

# $(OBJ):
# 	@$(CC) $(FLAGS) $(INC) -c $(SRC_POS)

# clean:
# 	@rm -rf *.h.gch
# 	@rm -f $(OBJ)

# fclean: clean
# 	@rm -f $(NAME)

# re: fclean all

# compile: re
# 	@$(CC) $(FLAGS) $(INC) mymain.c $(NAME)
# 	@make clean

# cleanup: fclean
# 	@rm -rf *.out*

# .PHONY : all, re, clean, fclean, cleanup
