#include "ftshell.h"

static bool	validate_term(void)
{
	char	*term_id;
	char	buf[2048];

	term_id = getenv("TERM");
	if (!term_id)
		ft_putstr_fd("error: $TERM variable invalid\n",
		STDERR_FILENO);
	else if (!tgetent(buf, term_id))
		ft_putstr_fd("error: tgetent failed\n",
		STDERR_FILENO);
	else
		return (true);
	return (false);
}

void		init_term(void)
{
	char	buf[30];
	char	*temp;

	temp = buf;
	tcgetattr(STDERR_FILENO, &g_term.old_term);
	tcgetattr(STDERR_FILENO, &g_term.new_term);
	g_term.new_term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDERR_FILENO, TCSANOW, &g_term.new_term);
	if (!(tgetstr("vi", &temp)))
	{
		ft_putstr_fd("error: host terminal attr invalid\n", STDERR_FILENO);
		exit (0);
	}
	ft_putstr_fd(tgetstr("vi", &temp), STDERR_FILENO);
	ft_putstr_fd(tgetstr("ti", &temp), STDERR_FILENO);
	GET_SCREENSIZE;
	g_term.rows = 1;
}

void		reset_term(void)
{
	char	*temp;
	char	buf[30];

	temp = buf;
	tcsetattr(STDERR_FILENO, TCSANOW, &g_term.old_term);
	ft_putstr_fd(tgetstr("ve", &temp), STDERR_FILENO);
	ft_putstr_fd(tgetstr("te", &temp), STDERR_FILENO);
}

char		*ft_readstdin_line(t_shellconf *conf)
{
	char	buf[BUFF_SIZE + 1];
	char	*tmp;
	char	*s;
	int		ret;
	u_input	thing;

	s = NULL;
	ft_memset(buf, 0, BUFF_SIZE + 1);
	ft_putstr_fd(PROMPT, STDERR_FILENO);
	thing.long_form = 0;
	while ((ret = read(0, &thing, 8)) > 0)
	{
		buf[ret] = '\0';
		ft_memcpy(buf, thing.arr_form, 8);
		if (!handle_controls(thing.long_form, s, conf))
		{
			term_write(buf, STDERR_FILENO, conf);
			tmp = s ? ft_strjoin(s, buf) : ft_strdup(buf);
		}
		s = tmp;
		if (ft_strchr(s, '\n'))
		{
			tmp = ft_strndup(s, (ft_strchr(s, '\n') - s));
			free(s);
			return (tmp);
		}
	}
	return (0);
}

void		shell_loop(void)
{
	char		*line;
	t_ast		*tree;
	int			quit;
	t_shellconf	conf;

	quit = 0;
	conf.termsize[0] = g_window_size.ws_col;
	conf.termsize[1] = g_window_size.ws_row;
	conf.cursor[0] = ft_strlen(PROMPT);
	conf.cursor[1] = 0;
	line = NULL;
	tree = NULL;
	while (!quit)
	{
		line = ft_readstdin_line(&conf);
		if (!line)
			continue ;
		else
			tree = parse_input(line);//ft_printf("%s\n", line);
		if (tree)
			//print_tree(tree);
			quit = parse_tree(&tree);
		ft_strdel(&line);
	}
}

void		define_symbols(void)
{
	static char	*symbols[14] = { 
	"NONE",
	"WORD",
	"&&",
	"<<",
	">>",
	"||",
	"|",
	";",
	"<",
	">",
	"REDIRECT",
	"IO_NUMBER",
	"NEWLINE",
	NULL };

	g_term.symbls = symbols;
}
int			main(int ac, char **av)
{
	//t_args	*args;

	if (ac || av)
	{
		;
	}

	if (!(validate_term()))
		return (0);
	if (!make_env(&g_term.env))
	init_term();
	set_sighandle();
	//shell_loop();
	define_symbols();
	CLEAR_SCREEN;
	shell_loop();
	CLEAR_SCREEN;
	reset_term();
	return (0);
}
