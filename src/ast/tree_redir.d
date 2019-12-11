src/ast/tree_redir.o: src/ast/tree_redir.c includes/ftshell.h \
  libft/includes/libft.h libft/includes/ft_printf.h \
  libft/includes/ft_defs.h
	gcc -g -Ilibft/includes -Iincludes -Wall -Werror -Wextra  -c -o src/ast/tree_redir.o src/ast/tree_redir.c