/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftshell.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:45:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/20 16:07:14 by alkozma          ###   ########.fr       */
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
# include "keycode_linux.h"

# define LEAF_SIZE 5
# define PROMPT "@>"
# define HDPROMPT " >"
# define HT_OVERHEAD 5000

typedef struct s_term		t_term;
typedef struct s_term_sigs	t_term_sigs;
typedef struct s_shellconf	t_shellconf;
typedef struct s_stats		t_stats;
typedef struct s_tbuff		t_tbuff;

typedef struct s_lexeme		t_lexeme;
typedef struct s_node		t_node;

typedef struct s_ht			t_ht;

typedef struct s_redir		t_redir;
typedef struct s_child		t_child;

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
	char					arr_form[sizeof(unsigned long)];
}							t_input;

struct						s_ht
{
	void					*content;
	void					*content_name;
	size_t					content_size;
	struct s_ht				*next;
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
	int						termsize[2];
	int						cursor[2];
	int						curlines;
};

struct						s_term_sigs
{
	bool					restart;
	bool					sigstop;
	bool					sigint;
};

struct						s_term
{
	struct s_env			env;
	char					**symbls;
	t_tbuff					*buff;
	t_shellconf				conf;
	t_term_sigs				sigs;
	struct termios			old_term;
	struct termios			new_term;
	int						rows;
	t_child					*children;
};

struct						s_stats
{
	int						f_d[2];
	int						fd_err[2];
	int						ret;
	int						exit;
};

struct						s_child
{
	int						pid;
	struct s_child			*next;
};

struct						s_st
{
	struct s_st				*children[128];
	uint64_t				access[2];
	char					*str;
};

/*
** TERMCAPS
*/

void						jump_by_row(t_tbuff *buff, unsigned long code);
int							ft_charput(int c);
void						reset_term(void);
void						init_term(void);
int							term_write(char *str, int fd, int len);
bool						validate_term(void);
int							zero_cursor(int hd);

/*
** SIGNALS
*/
int							check_fd(int fd);
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
void						free_redir(t_redir **list);
void						child_wait(void);
int							child_pop(t_child **stack);
void						child_push(t_child **stack, int pid);

void						exec_heredoc(t_node *node, int *in, int *out);
int							handle_redirs(t_redir *list);
int							get_input(void);
void						shell_reset_stuff(t_stats *stats);
int							has_hd(char *thing, char *hd);
void						shell_loop(void);
int							ft_readstdin_line(t_tbuff **tbuff, int hd);
int							read_rcfile(void);

/*
** INPUT BUFFER
*/
void						tbuff_rm_node(t_tbuff *node);
void						tbuff_rm_edits(t_tbuff **buff);
void						tbuff_free_hd(t_tbuff **buff);
void						tbuff_replicate(t_tbuff **buff);
void						tbuff_cleanup(t_tbuff **buff);
int							tbuff_choose(t_tbuff **buff, int hd);
void						tbuff_new(t_tbuff **buff);
int							reprint_buffer(t_tbuff *buff, int pos,
												int move_amt);
void						tbuff_line_insert(t_tbuff **buff,
										char *in, int pos);
void						t_buff_line_rm(t_tbuff *buff,
										int pos, int size);
void						tbuff_free(t_tbuff **buff);
void						tbuff_move_cursor(t_tbuff *buff,
								unsigned long code, char *str);
int							move_cursor(int amt, int affect_tc,
										t_tbuff *buff, int pos);
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
int							quote_parser(char *data, char **ret,
											int *i, int *cur);
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

void						exec_handle_redir(t_node *node, int *in, int *out);
int							find_pipe(t_node *head);
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
int							jump_by_word_amt(t_tbuff *buff, char *str,
													int pos, int dir);
int							termcap_reset_cursor(int pos, int len);
void						add_redir(int src, int dst, t_redir **list);
void						redo_prompt(int hd, int print);
char						*convert_tabs_spaces(char *data);
int							free_arr(char **arr);
int							print_alias(void);
void						st_clean(struct s_st *tree);
char						*st_match(char *sub, struct s_st *tree);
void						st_insert_str(char *str, struct s_st **tree);
#endif
