#include "ftshell.h"

static void pretty_print(t_complete_commands *node, unsigned int depth)
{
    static char pad[] = "                                                                         ";

	printf("%.*sCOMPLETE_COMMANDS\n", depth * 2, pad);
	handle_complete_commands(node->complete_commands, COMPLETE_COMMANDS, PRINT, depth + 1);
    handle_newline_list(node->newline_list, NEWLINE_LIST, PRINT, depth + 1);
	handle_complete_command(node->complete_command, COMPLETE_COMMAND, PRINT, depth + 1);
}

void		*handle_complete_commands(void *node, unsigned int type, unsigned int method, unsigned int depth)
{
	if (type || method || depth)
	{
		;
	}
    if (method == 0)
    {
        // CREATE
        ;
    }
    else if (method == 1)
    {
        // VISIT
        ;
    }
    else if (method == 2)
    {
        pretty_print(node, depth);
    }
	return (0);
}