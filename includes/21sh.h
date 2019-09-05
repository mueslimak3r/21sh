#ifndef FTSH_H
# define FTSH_H

# define REDIR_L '<'
# define REDIR_R '>'
# define REDIR_LL "<<"
# define REDIR_RR ">>"
# define PIPE '|'
# define AND '&'
# define WAIT ';'

# include "../libft/libft.h"

typedef struct s_cdata  t_cdata;

struct s_cdata
{
    char *command;
    char **args;

};

void	parse_input(char *input);

#endif
