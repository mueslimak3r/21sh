ft_printf/flags.o: ft_printf/flags.c includes/ft_printf.h \
  includes/ft_defs.h includes/libft.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o ft_printf/flags.o ft_printf/flags.c