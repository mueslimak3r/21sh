NAME = 21sh

SRC = src/main.c \
	src/ast/lexer.c src/ast/parser.c \
	src/env/alias.c src/env/auto.c src/env/env.c src/env/env2.c src/env/rc.c \
	src/exec/exec.c src/exec/exec2.c \
	src/history_rope/history_buffer.c src/history_rope/history_rope.c \
	src/shell/builtin.c src/shell/error.c src/shell/flavor.c src/shell/input.c src/shell/signals.c src/shell/utils.c

#SRC_POS = $(addprefix $(SRC_PATH),$(SRC))

OBJ := $(notdir $(SRC)) #$(subst .c,.o,$(filter %.c, $(SRC))) #$(SRC:.c=.o)# $(subst .c,.o,$(SRC))#$(SRC:.c=.o)

#SRC_PATH := src/

INC := -I includes

CC = gcc

FLAGS = -g -fsanitize=address #-Wall -Wextra -Werror #-g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft
	@gcc $(FLAGS) $(OBJ) libft/libft.a -lncurses -o $(NAME)

$(OBJ):
	@$(CC) $(INC) $(FLAGS) -c $(SRC)

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
