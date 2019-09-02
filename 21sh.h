#ifndef FTSH_H
# define FTSH_H

# define REDIR_L '<'
# define REDIR_R '>'
# define REDIR_LL "<<"
# define REDIR_RR ">>"
# define PIPE '|'
# define AND '&'
# define WAIT ';'

# include "../includes/libft.h"
# include "../includes/ftprintf.h"
# include "../includes/ftgnl.h"

void	parse_input(char *input);

#endif
