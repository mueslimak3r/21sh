src/ft_itoa_base.o: src/ft_itoa_base.c includes/libft.h \
  includes/ft_printf.h includes/ft_defs.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o src/ft_itoa_base.o src/ft_itoa_base.c