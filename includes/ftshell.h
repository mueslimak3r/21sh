#ifndef FTSHELL_H
# define FTSHELL_H

# include <signal.h>
# include <sys/ioctl.h>
# include <termcap.h>
# include <termios.h>
# include "../libft/libft.h"
# include <sys/stat.h>

# define REDIR_L "<"
# define REDIR_R ">"
# define REDIR_LL "<<"
# define REDIR_RR ">>"
# define PIPE "|"
# define AND "&"
# define WAIT ";"

# define IS_REDIR_L(s) ((ft_strncmp(s, REDIR_L, 1)) ? (0) : (1))
# define IS_REDIR_R(s) ((ft_strncmp(s, REDIR_R, 1)) ? (0) : (1))
# define IS_REDIR_LL(s) ((ft_strncmp(s, REDIR_LL, 2)) ? (0) : (1))
# define IS_REDIR_RR(s) ((ft_strncmp(s, REDIR_RR, 2)) ? (0) : (1))
# define IS_PIPE(s) ((ft_strncmp(s, PIPE, 1)) ? (0) : (1))
# define IS_AND(s) ((ft_strncmp(s, AND, 1)) ? (0) : (1))
# define IS_WAIT(s) ((ft_strncmp(s, WAIT, 1)) ? (0) : (1))

# define SPACE 0x20
# define ENTER 0xA
# define ESCAPE 0x1B
# define LEFT 0x445B1B
# define RIGHT 0x435B1B
# define DELETE 0x7F
# define DELETE2 0x7E335B1B
# define CLEAR_SCREEN ft_putstr_fd(tgetstr("cl", NULL), STDERR_FILENO);
# define GET_SCREENSIZE ioctl(STDERR_FILENO, TIOCGWINSZ, &g_window_size);

# define PROMPT "@>"

int				g_routes[5000];

typedef struct s_token      t_token;
typedef struct s_term       t_term;
typedef struct s_ast        t_ast;
typedef struct s_shellconf	t_shellconf;
typedef struct s_stats		t_stats;

struct winsize			g_window_size;
struct s_term			g_term;

typedef union
{
	unsigned long	long_form;
	char			arr_form[4];
} u_input;

struct s_token
{
    char                *name;
    char                **args;
    int                 type;
    int                 in;
    int                 out;
    int                 err;
    struct s_token	*next;
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
	struct s_ast		*parent;
    struct s_ast        *left;
    struct s_ast        *right;
};

struct s_shellconf
{
	int					g_routes[5000];
	int					termsize[2];
	int					cursor[2];
};

struct s_stats
{
	int	ret;
	int exit;
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
	AND_,
	OR_IF,
	PIPE_,
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
t_ast			*parse_input(char *input);
void			parse_tokens(t_token *tokens);
void			print_tree(t_ast *tree);
int				term_write(char *str, int fd, t_shellconf *conf);
int				handle_controls(unsigned long code, char *str, t_shellconf *conf);
char			*ft_readstdin_line(t_shellconf *conf);
int				ft_printf_fd(int fd, const char *fmt, ...);
int     parse_tree(t_ast **tree);
int     dispatch_tree(t_ast **tree, t_stats *ret);
#endif
