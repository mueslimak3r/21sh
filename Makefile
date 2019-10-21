NAME = 21sh

SRC = main.c signals.c write.c utils.c \
	parser.c lexer.c exec.c error.c builtin.c \
	flavor.c env.c rc.c alias.c input_buffer.c

OBJ = $(SRC:.c=.o)

SRC_PATH = src/

SRC_POS = $(addprefix $(SRC_PATH),$(SRC))

INC = -I includes

CC = gcc

FLAGS = -g #-fsanitize=address #-Wall -Wextra -Werror #-g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft
	@gcc $(FLAGS) $(OBJ) libft/libft.a -lncurses -o $(NAME)

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
