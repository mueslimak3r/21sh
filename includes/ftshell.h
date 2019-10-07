#ifndef FTSHELL_H
# define FTSHELL_H

# include <signal.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include "../libft/libft.h"
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>
# include <errno.h>

//# define PATHMAX 255

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
typedef struct s_tbuff		t_tbuff;

typedef struct s_lexeme		t_lexeme;
typedef struct s_node		t_node;

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

enum			e_nodetype
{
	BASE,
	EXPR,
	EXEC,
	ARG,
	MOD
};

typedef union
{
	unsigned long	long_form;
	char			arr_form[4];
} u_input;

// USE THIS FOR TOKENIZER (LEXER)
struct s_lexeme
{
	enum e_tokentype	set;
	char				*data;
	int					pos;
	enum e_nodetype		designation;
	struct s_lexeme		*next;
};

struct s_node
{
	enum e_nodetype	set;
	struct s_lexeme	*lexeme;
	struct s_node	*parent;
	struct s_node	*children;
	struct s_node	*next;
};

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

struct s_ast
{
    struct s_token      *token;
	struct s_ast		*parent;
    struct s_ast        *left;
    struct s_ast        *right;
};

struct s_tbuff_buff
{
	char				buff[200];
	size_t				size;
	size_t				pos;
	struct s_tbuff_buff	*next;
};

struct s_tbuff
{
	struct s_tfuff_buff	*buff;
	size_t				size;
	struct s_tbuff		*next;
};

struct s_shellconf
{

	int					g_routes[5000];
	int					termsize[2];
	int					cursor[2];
	int					curr_c;
	t_tbuff				*buff_first;
	t_tbuff				*buff_last;
	int					curlines;
};

struct s_term
{
	struct s_env		env;
	char				*line_in;
	char				**symbls;
	t_shellconf			conf;
	struct termios		old_term;
	struct termios		new_term;
	int					rows;
};

struct s_stats
{
	int	ret;
	int exit;
};

void			set_sighandle(void);
void		    reset_term(void);
void            init_term(void);
t_ast			*parse_input(void);
void			parse_tokens(t_token *tokens);
void			print_tree(t_ast *tree);
int				term_write(char *str, int fd, int len);
int				handle_controls(unsigned long code, char *str, char *saved);
int				ft_readstdin_line(void);
int				ft_printf_fd(int fd, const char *fmt, ...);
int     parse_tree(t_ast **tree);
int     dispatch_tree(t_ast **tree, t_stats *ret);
void		shell_loop(void);


int			run_dispatch(char **args, t_env *env);
int			find_env(char **envp, char *name);
int					make_env(t_env *env);
int					check_path(char **name, char **args, char **envp);

void	lexer(void);
enum e_nodetype	classify(t_lexeme *lexeme);
int		is_mod(t_lexeme *lexeme);
int		is_arg(t_lexeme *lexeme);
int		is_exec(t_lexeme *lexeme);

#endif
