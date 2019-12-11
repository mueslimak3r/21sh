src/ft_readstdin_line.o: src/ft_readstdin_line.c includes/libft.h \
  includes/ft_printf.h includes/ft_defs.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o src/ft_readstdin_line.o src/ft_readstdin_line.c