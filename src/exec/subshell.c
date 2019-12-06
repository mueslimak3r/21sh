#include "ftshell.h"

int		subshell(int *in, int *out, char **args, t_redir *list)
{
	char	*name;
	pid_t	pid;

	name = NULL;
	args[0] = find_alias(args[0]);
	if (!check_path(&name, args, g_term.env.envp))
		return (ft_printf_fd(2, "-wtsh: %s: command not found\n", args[0]));
	reset_term();
    if ((pid = fork()) == 0)
	{
		dup2(in[0], 0);
		dup2(out[1], 1);
		if (!handle_redirs(list))
			return (0);
		set_sighandle_child();
		execve(name, args, g_term.env.envp);
		exit(0);
	}
	waitpid(pid, 0, 0);
    init_term();
	set_sighandle();
	//in[0] > 2 ? close(in[0]) : 0;
	//in[1] > 2 ? close(in[1]) : 0;
	//out[0] > 2 ? close(out[1]) : 0;
    //out[1] > 2 ? close(out[1]) : 0;
	name ? free(name) : 0;
	return (1);
}