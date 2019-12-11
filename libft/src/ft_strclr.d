src/ft_strclr.o: src/ft_strclr.c includes/libft.h includes/ft_printf.h \
  includes/ft_defs.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o src/ft_strclr.o src/ft_strclr.c