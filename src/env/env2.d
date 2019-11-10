src/env/env2.o: src/env/env2.c includes/ftshell.h libft/libft.h \
 libft/ft_printf/includes/ft_printf.h libft/ft_printf/includes/ft_defs.h
	gcc -Wall -Werror -Wextra -Ilibft/ft_printf/includes  -Ilibft -Iincludes  -c -o src/env/env2.o src/env/env2.c
