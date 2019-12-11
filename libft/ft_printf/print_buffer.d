ft_printf/print_buffer.o: ft_printf/print_buffer.c includes/ft_printf.h \
  includes/ft_defs.h includes/libft.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o ft_printf/print_buffer.o ft_printf/print_buffer.c