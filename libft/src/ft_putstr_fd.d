src/ft_putstr_fd.o: src/ft_putstr_fd.c includes/libft.h \
  includes/ft_printf.h includes/ft_defs.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o src/ft_putstr_fd.o src/ft_putstr_fd.c