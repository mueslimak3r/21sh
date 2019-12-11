ft_printf/parse_d.o: ft_printf/parse_d.c includes/ft_printf.h \
  includes/ft_defs.h includes/libft.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o ft_printf/parse_d.o ft_printf/parse_d.c