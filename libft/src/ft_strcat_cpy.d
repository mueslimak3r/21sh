src/ft_strcat_cpy.o: src/ft_strcat_cpy.c includes/libft.h \
  includes/ft_printf.h includes/ft_defs.h
	gcc -Wall -Werror -Wextra -Iincludes  -c -o src/ft_strcat_cpy.o src/ft_strcat_cpy.c