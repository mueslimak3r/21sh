#ifndef FT_SHELL_H
# define FT_SHELL_H

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
# define GET_SCREENSIZE ioctl(STDERR_FILENO, TIOCGWINSZ, &window_size);

#endif