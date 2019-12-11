src/get_next_line.o: src/get_next_line.c includes/libft.h \
  includes/ft_printf.h includes/ft_defs.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o src/get_next_line.o src/get_next_line.c