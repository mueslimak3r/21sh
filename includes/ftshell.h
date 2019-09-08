#ifndef FTSHELL_H
# define FTSHELL_H

# include <signal.h>
# include <sys/ioctl.h>
# include <termcap.h>
# include <termios.h>
# include "../libft/libft.h"
# include <sys/stat.h>

# define REDIR_L '<'
# define REDIR_R '>'
# define REDIR_LL "<<"
# define REDIR_RR ">>"
# define PIPE '|'
# define AND '&'
# define WAIT ';'

# define SPACE 0x20
# define ENTER 0xA
# define ESCAPE 0x1B
# define LEFT 0x445B1B
# define RIGHT 0x435B1B
# define DELETE 0x7F
# define DELETE2 0x7E335B1B
# define CLEAR_SCREEN ft_putstr_fd(tgetstr("cl", NULL), STDERR_FILENO);
# define GET_SCREENSIZE ioctl(STDERR_FILENO, TIOCGWINSZ, &g_window_size);

typedef struct s_token      t_token;
typedef struct s_term       t_term;
typedef struct s_ast        t_ast;

struct winsize			g_window_size;
struct s_term			g_term;

struct s_token
{
    char                *name;
    char                **args;
    int                 type;
    int                 in;
    int                 out;
    int                 err;
};

struct s_term
{
	struct termios		old_term;
	struct termios		new_term;
	int					rows;
};

struct s_ast
{
    struct s_token      *token;
    struct s_ast        *root;
    struct s_ast        *left;
    struct s_ast        *right;
};

enum			e_tokentype
{
	WORD,
	OPERATOR,
	REDIRECT,
	IO_NUMBER,
	NEWLINE
};

enum			e_operatorlst
{
	NONE,
	DSEMI,
	SEMI,
	AND_IF,
	AND,
	OR_IF,
	PIPE,
	DLESS_DASH,
	DLESS,
	LESS_AND,
	LESS_GREAT,
	LESS,
	DGREAT,
	GREAT_AND,
	CLOBBER,
	GREAT
};

void			set_sighandle(void);
void		    reset_term(void);
void            init_term(void);
void	parse_input(char *input);

#endif