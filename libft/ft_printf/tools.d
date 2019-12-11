ft_printf/tools.o: ft_printf/tools.c includes/ft_printf.h \
  includes/ft_defs.h includes/libft.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o ft_printf/tools.o ft_printf/tools.c