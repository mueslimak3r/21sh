src/shell/signals2.o: src/shell/signals2.c includes/ftshell.h \
  libft/includes/libft.h libft/includes/ft_printf.h \
  libft/includes/ft_defs.h
	gcc -g -Ilibft/includes -Iincludes -Wall -Werror -Wextra  -c -o src/shell/signals2.o src/shell/signals2.c