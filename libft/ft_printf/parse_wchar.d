ft_printf/parse_wchar.o: ft_printf/parse_wchar.c includes/ft_printf.h \
  includes/ft_defs.h includes/libft.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o ft_printf/parse_wchar.o ft_printf/parse_wchar.c