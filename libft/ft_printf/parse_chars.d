ft_printf/parse_chars.o: ft_printf/parse_chars.c includes/ft_printf.h \
  includes/ft_defs.h includes/libft.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o ft_printf/parse_chars.o ft_printf/parse_chars.c