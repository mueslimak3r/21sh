/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftshell.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:45:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/10 11:51:00 by alkozma          ###   ########.fr       */
/*                                                                            */
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
# include <sys/select.h>

# define SPACE 0x20
# define ENTER 0xA
# define TAB 0x09
# define ESCAPE 0x1B

# define LEFT 0x445B1B
# define RIGHT 0x435B1B
# define UP 0x415B1B
# define DOWN 0x425B1B

# define ALTLEFT 0x435B1B1B
# define ALTRIGHT 0x445B1B1B

# define HOME 0x485B1B
# define END 0x465B1B

# define DELETE 0x7F
# define DELETE2 0x7E335B1B
# define CLEAR_SCREEN ft_putstr_fd(tgetstr("cl", NULL), STDERR_FILENO);
# define GET_SCREENSIZE ioctl(STDERR_FILENO, TIOCGWINSZ, &g_window_size);

# define LEAF_SIZE 5

# define PROMPT "@>"
# define HDPROMPT " >"
# define TYPES
# define HT_OVERHEAD 5000

typedef struct s_term		t_term;
typedef struct s_term_sigs	t_term_sigs;
typedef struct s_shellconf	t_shellconf;
typedef struct s_stats		t_stats;
typedef struct s_tbuff		t_tbuff;

typedef struct s_lexeme		t_lexeme;
typedef struct s_node		t_node;

typedef struct s_ht			t_ht;

typedef struct s_st_node	t_st_node;
typedef struct s_st_leaf	t_st_leaf;
typedef struct s_rope_node	t_rope_node;

typedef struct s_redir		t_redir;

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
	REDIR,
	FD_LIT,
	ERR
};

struct						s_redir
{
	int						src;
	int						dst;
	struct s_redir			*next;
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

struct						s_rope_node
{
	int						length;
	int						removed_length;
	t_rope_node				*parent;
	t_rope_node				*left;
	t_rope_node				*right;
	char					str[LEAF_SIZE + 1];
};

struct						s_tbuff
{
	char					*buff_str;
	int						len;
	struct s_tbuff			*next;
	struct s_tbuff			*prev;
	struct s_tbuff			*original;
	bool					heredocs;
	bool					temp;
};

struct						s_shellconf
{
	int						prompt_size;
	int						g_routes[5000];
	int						termsize[2];
	int						cursor[2];
	int						curlines;
};

struct						s_term_sigs
{
	bool					sigint;
};

struct						s_term
{
	struct s_env			env;
	char					**symbls;
	t_tbuff					*buff;
	t_tbuff					*curr_buff;
	t_tbuff					*hd_buff;
	t_shellconf				conf;
	t_term_sigs				sigs;
	struct termios			old_term;
	struct termios			new_term;
	int						rows;
	int						pid;
};

struct						s_stats
{
	int						f_d[2];
	int						fd_err[2];
	int						ret;
	int						exit;
};

/*
** TERMCAPS
*/
int							ft_charput(int c);
void						reset_term(void);
void						init_term(void);
int							term_write(char *str, int fd, int len);
bool						validate_term(void);
int							zero_cursor(void);

/*
** SIGNALS
*/
void						set_sighandle_child(void);
void						set_sighandle(void);
int							handle_controls(unsigned long code, t_tbuff **buff);
void						sig_int(int nb);
void						sig_stop(int nb);
void						sig_resize(int nb);
void						sig_suspend(int nb);
void						sig_resume(int nb);

/*
** SHELL
*/
int							handle_redirs(t_redir *list);
int							get_input(void);
void						shell_reset_stuff(t_stats *stats);
int							has_hd(char *thing, char *hd);
void						shell_loop(void);
int							ft_readstdin_line(t_tbuff **tbuff, int hd);
int							read_rcfile(void);
int							subshell(int *in, int *out, char **args, t_redir *list);

/*
** INPUT BUFFER
*/
void		tbuff_rm_edits(t_tbuff **buff);
void						tbuff_free_hd(t_tbuff **buff);
void						tbuff_replicate(t_tbuff **buff);
void						tbuff_cleanup(t_tbuff **buff);
void						tbuff_choose(t_tbuff **buff, int hd);
void						tbuff_new(t_tbuff **buff);
int							reprint_buffer(t_tbuff *buff, int pos, int move_amt);
void						tbuff_line_insert(t_tbuff **buff, char *in, int pos);
void						t_buff_line_rm(t_tbuff *buff, int pos, int size);
void						tbuff_free(t_tbuff **buff);
void						tbuff_move_cursor(t_tbuff *buff,
								unsigned long code, char *str);
int							move_cursor(int amt, int affect_tc, t_tbuff *buff);
void						tbuff_line_setsize(t_tbuff *buff, int amt);
/*
** ENVIRONMENT
*/

char						*find_env(char *name);
char						*path_expansions(char *path);
int							ft_unsetenv(char *name);
int							ft_setenv(char *name, char *val);
int							init_env(void);
int							ft_export(char *str);
int							load_envp(void);
int							run_builtins(char **args, t_env *env);
int							check_path(char **name, char **args, char **envp);
int							run_dispatch(char **args, t_env *env);

/*
** LEXER
*/

t_node						*lexer(char *input);
int							is_operator(char *op, int pos);
t_lexeme					*new_lex(char *data, enum e_tokentype type,
										t_lexeme **head);
int							is_nb_before_redir(char *op);
int							is_redirect(char *op);
int							handle_redirect(char *op, t_lexeme **head);
int							handle_quote(char *input);
char						*add_lex_op(t_lexeme **head, char *line, int op);

/*
** PARSER
*/

t_node						*parser(t_lexeme *lexemes);
void						clean_tree(t_node *head);
enum e_nodetype				classify(t_lexeme *lexeme);
int							is_mod(t_lexeme *lexeme);
int							is_arg(t_lexeme *lexeme);
int							is_exec(t_lexeme *lexeme);
int							is_fd_lit(t_lexeme *lexeme);
char						**concat_node(t_node *node, t_redir **list);
t_node						*new_node(enum e_nodetype set, t_lexeme *lexeme,
										t_node *parent, int dir);
t_node						*abstract(t_node *node);
t_node						*new_abstract_node(t_node *node);
int							count_pipes(t_node *node);
/*
** AST
*/

void						exec_node_parse(t_node *node, int *in, int *out);
void						recurse(t_node *head, t_stats *stats);
void						redir_pipes(t_node *node, t_redir **list);

/*
** ALIASING
*/

char						*find_alias(char *name);
int							ft_alias(char *str);

/*
** HELPERS
*/

int							empty_buffer(int fd[2]);
int							print_buffer(int fd[2]);
int							execute_command(int *in, int *out,
								char **args, t_redir *list);
void						print_banner(int fd);
int							ft_printf_fd(int fd, char *fmt, ...);
int							parse_error(t_node *head, t_lexeme *error);
unsigned long				djb2(char *str);
int							readfd(int fd1, int fd2, int cd2);
int							ft_cd(char *path);
int							ft_env(char **envp);
int							last_slash(const char *in);
int							calc_pos(void);
int							resolve_path(char *arg);
int							check_dir(char *name, char *path);
int							jump_by_word_amt(char *str, int pos, int dir);

int							termcap_reset_cursor(int pos, int len);
void						add_redir(int src, int dst, t_redir **list);
void						print_prompt(int hd);
#endif
