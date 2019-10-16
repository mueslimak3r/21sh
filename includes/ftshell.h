/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftshell.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 15:39:28 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/16 15:01:26 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <fcntl.h>

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

# define HT_OVERHEAD 5000

int				g_routes[5000];

typedef struct s_token      t_token;
typedef struct s_term       t_term;
typedef struct s_ast        t_ast;
typedef struct s_shellconf	t_shellconf;
typedef struct s_stats		t_stats;
typedef struct s_tbuff		t_tbuff;

typedef struct s_lexeme		t_lexeme;
typedef struct s_node		t_node;

typedef struct s_ht			t_ht;

struct winsize				g_window_size;
struct s_term				g_term;

t_ht						*g_env[HT_OVERHEAD];
t_ht						*g_alias[HT_OVERHEAD];

enum			e_tokentype
{
	NONE,
	WORD,
	EXPANSION,
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
	MOD,
	ERR
};

typedef union
{
	unsigned long	long_form;
	char			arr_form[4];
} u_input;

struct s_ht
{
	void		*content;
	void		*content_name;
	size_t		content_size;
	struct s_ht	*next;
};

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
	int				evaluated;
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
	int f_d[2];
	int	ret;
	int exit;
};

void			set_sighandle(void);
void		    reset_term(void);
void            init_term(void);
t_ast			*parse_input(void);
void			parse_tokens(t_token *tokens);
void			print_tree(t_ast *tree);
void			print_banner(int fd);
int				term_write(char *str, int fd, int len);
int				handle_controls(unsigned long code, char *str, char *saved);
int				ft_readstdin_line(void);
int				ft_printf_fd(int fd, const char *fmt, ...);
int     parse_tree(t_ast **tree);
int     dispatch_tree(t_ast **tree, t_stats *ret);
void		shell_loop(void);
void	line_lexer(t_lexeme **front, t_lexeme **back, char *line);
int			is_operator(char *op, int pos);


int				sh_count_words(char *line);
char			*sh_next_word(char *line, int *j);

int			run_dispatch(char **args, t_env *env);
int			find_env(char **envp, char *name);
int					make_env(t_env *env);
int					check_path(char **name, char **args, char **envp);

int		parse_error(t_node *head, t_lexeme *error);

t_node	*parser(t_lexeme *lexemes);
t_node	*lexer(char *input);
void	recurse(t_node *head, t_stats *stats);
void	clean_tree(t_node *head);

enum e_nodetype	classify(t_lexeme *lexeme);
int		is_mod(t_lexeme *lexeme);
int		is_arg(t_lexeme *lexeme);
int		is_exec(t_lexeme *lexeme);
char	**concat_node(t_node *node);

int		run_builtins(char **args, t_env *env);

int		empty_buffer(int fd[2]);
int		execute_command(t_node *a, int in, int out);

unsigned long	djb2(char *str);
char			*find_env2(char *name);
int				ft_unsetenv2(char *name);
int				ft_setenv2(char *name, char *val);
int				init_env(void);
int				ft_export(char *str);
int				load_envp(void);

char			*find_alias(char *name);
int				ft_alias(char *str);

int				read_rcfile(void);
#endif
