ft_printf/ft_printf.o: ft_printf/ft_printf.c includes/ft_printf.h \
  includes/ft_defs.h includes/libft.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o ft_printf/ft_printf.o ft_printf/ft_printf.c