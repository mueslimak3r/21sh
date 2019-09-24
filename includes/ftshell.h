#ifndef FTSHELL_H
# define FTSHELL_H

# include <signal.h>
# include <sys/ioctl.h>
# include <termcap.h>
# include <termios.h>
# include "../libft/libft.h"
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>
# include <errno.h>

# define PATH_MAX 255


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

# define TYPES

int				g_routes[5000];

typedef struct s_token      t_token;
typedef struct s_term       t_term;
typedef struct s_ast        t_ast;
typedef struct s_shellconf	t_shellconf;
typedef struct s_stats		t_stats;

struct winsize			g_window_size;
struct s_term			g_term;

enum			e_tokentype
{
	NONE,
	WORD,
	AND,
	RDLESS,
	RDGREAT,
	OR,
	PIPE,
	SEMI,
	LESS,
	GREAT,
	REDIRECT,
	IO_NUMBER,
	NEWLINE
};

typedef union
{
	unsigned long	long_form;
	char			arr_form[4];
} u_input;

struct s_token
{
    char                *name;
    char                **args;
	enum e_tokentype	set;
    int                 in;
    int                 out;
    int                 err;
    struct s_token	*next;
};

typedef struct	s_env
{
	char		**envp;
	int			size;
}				t_env;

struct s_term
{
	struct s_env		env;
	char				**symbls;
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

int			run_dispatch(char **args, t_env *env);
int			find_env(char **envp, char *name);
int					make_env(t_env *env);
int					check_path(char **name, char **args, char **envp);
#endif
