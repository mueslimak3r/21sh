src/ft_cntwords_space.o: src/ft_cntwords_space.c includes/libft.h \
  includes/ft_printf.h includes/ft_defs.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o src/ft_cntwords_space.o src/ft_cntwords_space.c