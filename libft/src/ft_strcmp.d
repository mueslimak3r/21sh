src/ft_strcmp.o: src/ft_strcmp.c includes/libft.h includes/ft_printf.h \
  includes/ft_defs.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o src/ft_strcmp.o src/ft_strcmp.c