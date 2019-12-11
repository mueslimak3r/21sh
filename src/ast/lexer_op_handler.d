src/ast/lexer_op_handler.o: src/ast/lexer_op_handler.c includes/ftshell.h \
  libft/includes/libft.h libft/includes/ft_printf.h \
  libft/includes/ft_defs.h
	gcc -g -Ilibft/includes -Iincludes -Wall -Werror -Wextra  -c -o src/ast/lexer_op_handler.o src/ast/lexer_op_handler.c