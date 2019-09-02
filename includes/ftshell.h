#ifndef FTSHELL_H
# define FTSHELL_H

# include <signal.h>
# include <sys/ioctl.h>
# include <termcap.h>
# include <termios.h>
# include "../libft/libft.h"
# include <sys/stat.h>

# define SPACE 0x20
# define ENTER 0xA
# define ESCAPE 0x1B
# define LEFT 0x445B1B
# define RIGHT 0x435B1B
# define DELETE 0x7F
# define DELETE2 0x7E335B1B
# define CLEAR_SCREEN ft_putstr_fd(tgetstr("cl", NULL), STDERR_FILENO);
# define GET_SCREENSIZE ioctl(STDERR_FILENO, TIOCGWINSZ, &g_window_size);

typedef struct s_args       t_args;
typedef struct s_arg_node   t_arg_node;
typedef struct s_term       t_term;

struct winsize			g_window_size;
struct s_term			g_term;

struct s_arg_node
{
    char                *value;
    int                 len;
    struct s_arg_node   *next;
    struct s_arg_node   *prev;
};

struct s_args
{
    char                *args;
    int                 ac;
    struct s_arg_node   *stack;
    struct s_args       *next;
    struct s_args       *prev;
};

struct s_term
{
	struct termios		old_term;
	struct termios		new_term;
	int					rows;
};

void			set_sighandle(void);
void		    reset_term(void);
void            init_term(void);

#endif