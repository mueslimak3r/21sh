/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 ftshell.h											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: calamber <calamber@student.42.fr>			+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2019/10/11 15:39:28 by alkozma		   #+#	  #+#			  */
/*	 Updated: 2019/10/16 17:18:38 by alkozma		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#ifndef FTSHELL_H
# define FTSHELL_H

# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include "libft.h"
# include <sys/stat.h>
# include <unistd.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>

//# define PATHMAX 255

# define SPACE 0x20
# define ENTER 0xA
# define TAB 0x09
# define ESCAPE 0x1B
# define LEFT 0x445B1B
# define RIGHT 0x435B1B

# define UP 0x415B1B
# define DOWN 0x425B1B

# define DELETE 0x7F
# define DELETE2 0x7E335B1B
# define CLEAR_SCREEN ft_putstr_fd(tgetstr("cl", NULL), STDERR_FILENO);
# define GET_SCREENSIZE ioctl(STDERR_FILENO, TIOCGWINSZ, &g_window_size);

# define LEAF_SIZE 5

# define PROMPT "@>"
# define PROMPT_SIZE 2
# define HDPROMPT " >"
# define TYPES
# define HT_OVERHEAD 5000

typedef struct s_term		t_term;
typedef struct s_shellconf	t_shellconf;
typedef struct s_stats		t_stats;
typedef struct s_tbuff		t_tbuff;

typedef struct s_lexeme		t_lexeme;
typedef struct s_node		t_node;

typedef struct s_ht			t_ht;

typedef struct s_st_node	t_st_node;
typedef struct s_st_leaf	t_st_leaf;
typedef struct s_rope_node	t_rope_node;

struct winsize				g_window_size;
struct s_term				g_term;
struct s_ht					*g_env[HT_OVERHEAD];
struct s_ht					*g_alias[HT_OVERHEAD];

enum						e_tokentype
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
	NEWLINE,
	L_REDIRECT,
	R_REDIRECT,
	IO_NAME,
	FD_CLOSE,
};

enum						e_nodetype
{
	BASE,
	EXPR,
	EXEC,
	ARG,
	MOD,
	FD_R,
	FD_W,
	FD_H,
	FD_A,
	ERR
};

typedef union				u_input
{
	unsigned long			long_form;
	char					arr_form[4];
}							t_input;

struct						s_ht
{
	void					*content;
	void					*content_name;
	size_t					content_size;
	struct s_ht				*next;
};

struct						s_st_node
{
	char					*str;
	struct s_st_node		*children[256];
	struct s_st_node		*parent;
};

struct						s_lexeme
{
	enum e_tokentype		set;
	char					*data;
	int						pos;
	enum e_nodetype			designation;
	struct s_lexeme			*next;
};

struct						s_node
{
	enum e_nodetype			set;
	struct s_lexeme			*lexeme;
	struct s_node			*parent;
	struct s_node			*children;
	struct s_node			*next;
	int						evaluated;
};

typedef struct				s_env
{
	char					*envp[HT_OVERHEAD];
	int						size;
}							t_env;

struct s_rope_node
{
	int			length;
	int			removed_length;
	t_rope_node	*parent;
	t_rope_node	*left;
	t_rope_node	*right;
	char		str[LEAF_SIZE + 1];
};

struct s_tbuff
{
	//int						input_line_size[2];
	char				*buff_str;
	int					len;
	struct s_tbuff		*next;
	struct s_tbuff		*prev;
};

struct						s_shellconf
{

	int						g_routes[5000];
	int						termsize[2];
	int						cursor[2];
	int						curlines;
};

struct						s_term
{
	struct s_env			env;
	char					**symbls;
	t_tbuff					*buff;
	t_tbuff					*curr_buff;
	t_shellconf				conf;
	struct termios			old_term;
	struct termios			new_term;
	int						rows;
};

struct						s_stats
{
	int						f_d[3];
	int						ret;
	int						exit;
};

/*
** TERMCAPS
*/
int		ft_charput(int c);
void			reset_term(void);
void			init_term(void);
int				term_write(char *str, int fd, int len);

/*
** SIGNALS
*/

void			set_sighandle(void);
int				handle_controls(unsigned long code, char *str);

/*
** SHELL
*/

void			shell_loop(void);
int				ft_readstdin_line(int hd, char *stop);
int				read_rcfile(void);

/*
** INPUT BUFFER
*/

void			tbuff_new(t_tbuff **buff);
int				reprint_buffer(t_tbuff *buff);
void			tbuff_line_insert(t_tbuff *buff, char *in, int pos);
void        	t_buff_line_rm(t_tbuff *buff, int pos, int size);
void			tbuff_free(t_tbuff **buff);
void			tbuff_move_cursor(t_tbuff *buff, unsigned long code, char *str);
int				move_cursor(int amt, int affect_tc);
void        	tbuff_line_setsize(t_tbuff *buff, int amt);
/*
** ENVIRONMENT
*/

char			*find_env(char *name);
char			*path_expansions(char *path);
int				ft_unsetenv(char *name);
int				ft_setenv(char *name, char *val);
int				init_env(void);
int				ft_export(char *str);
int				load_envp(void);
int				run_builtins(char **args, t_env *env);
int				check_path(char **name, char **args, char **envp);
int				run_dispatch(char **args, t_env *env);

/*
** LEXER
*/

t_node			*lexer(char *input);
int				is_operator(char *op, int pos);

/*
** PARSER
*/

t_node			*parser(t_lexeme *lexemes);
void			recurse(t_node *head, t_stats *stats);
void			clean_tree(t_node *head);
enum e_nodetype	classify(t_lexeme *lexeme);
int				is_mod(t_lexeme *lexeme);
int				is_arg(t_lexeme *lexeme);
int				is_exec(t_lexeme *lexeme);
char			**concat_node(t_node *node, int *in, int *out, int *err);

/*
** ALIASING
*/

char			*find_alias(char *name);
int				ft_alias(char *str);

/*
** HELPERS
*/

int				empty_buffer(int fd[2]);
int		print_buffer(int fd[2]);
int		execute_command(int in, int out, int err, char **args);
void			print_banner(int fd);
int				ft_printf_fd(int fd, const char *fmt, ...);
int				parse_error(t_node *head, t_lexeme *error);
unsigned long	djb2(char *str);
int				readfd(int fd1, int fd2, int cd2);
int				ft_cd(char *path);
int				ft_env(char **envp);

void			auto_complete(void);
int				termcap_reset_cursor(int pos, int len);
#endif
