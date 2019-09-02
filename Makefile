NAME = 21sh

SRC = main.c signals.c

OBJ = $(SRC:.c=.o)

SRC_PATH = src/

SRC_POS = $(addprefix $(SRC_PATH),$(SRC))

INC = -I includes

CC = gcc

FLAGS = -Wall -Wextra -Werror -g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft
	@gcc $(FLAGS) $(OBJ) libft/libft.a -ltermcap -o $(NAME)

$(OBJ):
	@$(CC) $(INC) $(FLAGS) -c $(SRC_POS)

clean:
	@make clean -C libft
	@rm -f $(OBJ)

fclean: clean
	@make fclean -C libft
	@rm -f $(NAME)

re: fclean all

again: re
	@make clean

cleanup: fclean
	@rm -rf *21sh*

.PHONY : all, re, clean, fclean, again, cleanup