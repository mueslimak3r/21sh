#include "ftshell.h"

t_atom				*pop_atom(t_atom **atoms)
{
	t_atom	*ret;

	if (!atoms || !(ret = *atoms))
	{
		ft_printf_fd(STDERR_FILENO, "END OF ATOMS\n");
		return (NULL);
	}
	*atoms = (*atoms)->next;
	return (ret);
}

/*
** please send help
**            --alkozma
*/

t_sequential_sep		*make_sequential_sep(t_atom **atoms)
{
	t_sequential_sep	*ret;

	ret = malloc(sizeof(*ret));
	ret->atom = (*atoms)->type == SEMICOLON ? pop_atom(atoms) : NULL;
	ret->linebreak = ret->atom ? make_linebreak(atoms) : NULL;
	ret->newline_list = ret->atom ? NULL : make_newline_list(atoms);
	return (ret);
}

t_separator				*make_separator(t_atom **atoms)
{
	t_separator	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	if (!(ret->separator_op = make_separator_op(atoms)) && !(ret->linebreak = make_linebreak(atoms)))
		ret->newline_list = make_newline_list(atoms);
	else
		ret->newline_list = NULL;
	return (ret);
}

t_separator_op			*make_separator_op(t_atom **atoms)
{
	t_separator_op	*ret;

	ret = NULL;
	if (!atoms || !*atoms || ((*atoms)->type != AND_TOK && (*atoms)->type != SEMICOLON))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->atom = pop_atom(atoms);
	return (ret);
}

t_linebreak				*make_linebreak(t_atom **atoms)
{
	t_linebreak	*ret;

	ret = malloc(sizeof(*ret));
	ret->list = make_newline_list(atoms);
	return (ret);
}

t_newline_list			*make_newline_list(t_atom **atoms)
{
	t_newline_list	*ret;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != NEWLINE_TOK)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->atom = pop_atom(atoms);
	ret->next = make_newline_list(atoms);
	return (ret);
}

t_here_end				*make_here_end(t_atom **atoms)
{
	t_here_end	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	return (ret);
}

t_io_here				*make_io_here(t_atom **atoms)
{
	t_io_here	*ret;

	ret = NULL;
	if (!atoms || !*atoms || ((*atoms)->type != DLESS && (*atoms)->type != DLESSDASH))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->atom = pop_atom(atoms);
	if (!(ret->end = make_here_end(atoms)))
		ft_printf_fd(STDERR_FILENO, "problem in make io here\bn");
	return (ret);
}

t_filename				*make_filename(t_atom **atoms)
{
	t_filename	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	return (ret);
}

t_io_file				*make_io_file(t_atom **atoms)
{
	t_io_file	*ret;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != REDIR_IN ||
		(*atoms)->type != REDIR_OUT || (*atoms)->type != LESSAND ||
		(*atoms)->type != GREATAND || (*atoms)->type != DGREAT ||
		(*atoms)->type != LESSGREAT || (*atoms)->type != CLOBBER)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->op = pop_atom(atoms);
	ret->filename = make_filename(atoms);
	return (ret);
}

t_io_redirect			*make_io_redirect(t_atom **atoms)
{
	t_io_redirect	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	if (!(ret->io_number = (*atoms)->type == IO_NUMBER ? pop_atom(atoms) : NULL))
	{
		ft_printf_fd(STDERR_FILENO, "error making io redir\n");
		return (NULL);
	}
	ret->io_file = make_io_file(atoms);
	if (!ret->io_file)
		ret->io_here = make_io_here(atoms);
	else
		ret->io_here = NULL;
	return (ret);
}

t_redirect_list			*make_redirect_list(t_atom **atoms)
{
	t_redirect_list	*ret;
	t_io_redirect	*io_redir;

	ret = NULL;
	if (!atoms || !*atoms || !(io_redir = make_io_redirect(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->io_redirect = io_redir;
	ret->next = make_redirect_list(atoms);
	return (ret);
}

t_cmd_suffix			*make_cmd_suffix(t_atom **atoms)
{
	t_cmd_suffix	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	if (!(ret->io_redir = make_io_redirect(atoms)) &&
		!(ret->word = (*atoms)->type == TYPE_WORD ? pop_atom(atoms) : NULL))
		return (NULL);
	ret->next = make_cmd_suffix(atoms);
	return (ret);
}

t_cmd_prefix			*make_cmd_prefix(t_atom **atoms)
{
	t_cmd_prefix	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	if (!(ret->io_redir = make_io_redirect(atoms)) &&
		!(ret->assignment_word = (*atoms)->type == ASSIGNMENT_WORD ? pop_atom(atoms) : NULL))
		return (NULL);
	ret->next = make_cmd_prefix(atoms);
	return (ret);
}

t_cmd_word				*make_cmd_word(t_atom **atoms)
{
	t_cmd_word	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	return (ret);
}

t_cmd_name				*make_cmd_name(t_atom **atoms)
{
	t_cmd_name	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->name = pop_atom(atoms);
	return (ret);
}

t_simple_command		*make_simple_command(t_atom **atoms)
{
	t_simple_command	*ret;
	t_cmd_prefix		*pre;
	t_cmd_name			*name;

	ret = NULL;
	if (!atoms || !*atoms || (!(pre = make_cmd_prefix(atoms)) && !(name = make_cmd_name(atoms))))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->cmd_prefix = pre;
	ret->cmd_name = name;
	if (name)
		ret->cmd_suffix = make_cmd_suffix(atoms);
	else
	{
		ret->cmd_word = make_cmd_word(atoms);
		ret->cmd_suffix = make_cmd_suffix(atoms);
	}
	return (ret);
}

t_do_group				*make_do_group(t_atom **atoms)
{
	t_do_group		*ret;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != DO)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	ret->compound_list = make_compound_list(atoms);
	if (!(ret->done = (*atoms)->type == DONE ? pop_atom(atoms) : NULL))
		ft_printf_fd(STDERR_FILENO, "theres a problem making your do group\n");
	return (ret);
}

t_brace_group			*make_brace_group(t_atom **atoms)
{
	t_brace_group	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	if (!(ret->lbrace = (*atoms)->type == LEFT_BRACE ? pop_atom(atoms) : NULL) ||
		!(ret->compound_list = make_compound_list(atoms)) ||
		!(ret->rbrace = (*atoms)->type == RIGHT_BRACE ? pop_atom(atoms) : NULL))
		return (NULL);
	return (ret);
}

t_fname					*make_fname(t_atom **atoms)
{
	t_fname	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->fname = pop_atom(atoms);
	return (ret);
}

t_function_body			*make_function_body(t_atom **atoms)
{
	t_function_body	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	if (!(ret->command = make_compound_command(atoms)))
	{
		ft_printf_fd(STDERR_FILENO, "error making function body\n");
		return (NULL);
	}
	ret->redirect_list = make_redirect_list(atoms);
	return (ret);
}

t_function_definition	*make_function_definition(t_atom **atoms)
{
	t_function_definition	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	if (!(ret->fname = make_fname(atoms)) ||
		!(ret->lparen = ((*atoms)->type == LEFT_PAREN ? pop_atom(atoms) : NULL)) ||
		!(ret->rparen = ((*atoms)->type == RIGHT_PAREN ? pop_atom(atoms) : NULL)) ||
		!(ret->linebreak = make_linebreak(atoms)) || 
		!(ret->function_body = make_function_body(atoms)))
		return (NULL);
	return (ret);
}

t_until_clause		*make_until_clause(t_atom **atoms)
{
	t_until_clause	*ret;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != UNTIL)
		return (NULL);
	ret->word = pop_atom(atoms);
	ret->compound_list = make_compound_list(atoms);
	ret->do_group = make_do_group(atoms);
	if (!ret->compound_list || !ret->do_group)
		ft_printf_fd(STDERR_FILENO, "hey making until clause fucked up\n");
	return (ret);
}

t_while_clause		*make_while_clause(t_atom **atoms)
{
	t_while_clause	*ret;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != WHILE)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	ret->compound_list = make_compound_list(atoms);
	ret->do_group = make_do_group(atoms);
	if (!ret->compound_list || !ret->do_group)
		ft_printf_fd(STDERR_FILENO, "make while clause error\n");
	return (ret);
}

t_else_part			*make_else_part(t_atom **atoms)
{
	t_else_part		*ret;

	ret = NULL;
	if (!atoms || !*atoms || ((*atoms)->type != ELIF && (*atoms)->type != ELSE))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	ret->compound_list = make_compound_list(atoms);
	ret->then = ret->word->type == ELIF && (*atoms)->type == THEN ? pop_atom(atoms) : NULL;
	ret->else_part = ret->then ? make_else_part(atoms) : NULL;
	return (ret);
}

t_if_clause			*make_if_clause(t_atom **atoms)
{
	t_if_clause		*ret;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != IF)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	ret->compound_list = make_compound_list(atoms);
	if (*atoms && (*atoms)->type != THEN)
	{
		ft_printf_fd(STDERR_FILENO, "error making if clause\n");
		return (NULL);
	}
	ret->then = pop_atom(atoms);
	ret->compound_list2 = make_compound_list(atoms);
	ret->else_part = make_else_part(atoms);
	if (*atoms && (*atoms)->type != FI)
		ft_printf_fd(STDERR_FILENO, "error making if clause2\n");
	ret->fi = pop_atom(atoms);
	return (ret);
}

t_pattern			*make_pattern(t_atom **atoms)
{
	t_pattern	*ret;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != TYPE_WORD)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	ret->pipe = (*atoms)->type == REDIR_PIPE ? pop_atom(atoms) : NULL;
	ret->pattern = ret->pipe ? make_pattern(atoms) : NULL;
	return (ret);
}

t_case_item			*make_case_item(t_atom **atoms)
{
	t_case_item	*ret;
	t_pattern	*pat;

	ret = NULL;
	if (!atoms || !*atoms || !(pat = make_pattern(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->lparen = (*atoms)->type == LEFT_PAREN ? pop_atom(atoms) : NULL;
	ret->pattern = pat;
	if (!((*atoms)->type == RIGHT_PAREN))
		ft_printf_fd(STDERR_FILENO, "error making case item\n");
	ret->rparen = (*atoms)->type == RIGHT_PAREN ? pop_atom(atoms) : NULL;
	ret->linebreak = make_linebreak(atoms);
	if (!ret->linebreak && !(ret->compound_list = make_compound_list(atoms)))
		ft_printf_fd(STDERR_FILENO, "error making case item 2\n");
	ret->dsemi = (*atoms)->type == DSEMI ? pop_atom(atoms) : NULL;
	ret->linebreak2 = make_linebreak(atoms);
	return (ret);	
}

t_case_item_ns		*make_case_item_ns(t_atom **atoms)
{
	t_case_item_ns	*ret;
	t_pattern	*pat;

	ret = NULL;
	if (!atoms || !*atoms || !(pat = make_pattern(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->lparen = (*atoms)->type == LEFT_PAREN ? pop_atom(atoms) : NULL;
	ret->pattern = pat;
	ret->rparen = pop_atom(atoms);
	ret->compound_list = make_compound_list(atoms);
	ret->linebreak = make_linebreak(atoms);
	return (ret);
}

t_case_list		*make_case_list(t_atom **atoms)
{
	t_case_list	*ret;
	t_case_item	*item;
	t_case_list	*list;

	ret = NULL;
	if (!atoms || !*atoms || (!(item = make_case_item(atoms)) &&
		!(list = make_case_list(atoms))))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->list = list;
	ret->item = item;
	return (ret);
}

t_case_list_ns		*make_case_list_ns(t_atom **atoms)
{
	t_case_list_ns	*ret;
	t_case_item_ns	*item;
	t_case_list	*list;

	ret = NULL;
	if (!atoms || !*atoms || (!(list = make_case_list(atoms)) &&
		!(item = make_case_item_ns(atoms))))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->list = list;
	ret->item = item;
	return (ret);
}

t_case_clause		*make_case_clause(t_atom **atoms)
{
	t_case_clause	*ret;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != CASE)
		return (NULL);
	ret->case_word = pop_atom(atoms);
	ret->word = pop_atom(atoms);
	ret->linebreak = make_linebreak(atoms);
	ret->in = make_in(atoms);
	ret->linebreak2 = make_linebreak(atoms);
	if (!(ret->case_list = make_case_list(atoms)))
		ret->case_list_ns = make_case_list_ns(atoms);
	else
		ret->case_list_ns = NULL;
	ret->esac = pop_atom(atoms);
	return (ret);
}

t_wordlist		*make_wordlist(t_atom **atoms)
{
	t_wordlist	*ret;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != TYPE_WORD)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	ret->list = make_wordlist(atoms);
	return (ret);
}

t_in			*make_in(t_atom **atoms)
{
	t_in	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	return (ret);
}

t_name			*make_name(t_atom **atoms)
{
	t_name	*ret;
	int	i;

	ret = NULL;
	i = 0;
	if (!atoms || !*atoms || !(*atoms)->str ||
		!ft_isalpha((*atoms)->str[0]))
		return (NULL);
	while ((*atoms)->str[i])
	{
		if (!ft_isdigit((*atoms)->str[i]) &&
			ft_isdigit((*atoms)->str[i]) &&
			(*atoms)->str[i] != '_')
			return (NULL);
		i++;
	}
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	return (ret);
}

t_for_clause		*make_for_clause(t_atom **atoms)
{
	t_for_clause	*ret;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != FOR)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->for_word = pop_atom(atoms);
	ret->name = make_name(atoms);
	ret->linebreak = make_linebreak(atoms);
	ret->in = make_in(atoms);
	ret->wordlist = make_wordlist(atoms);
	ret->sequential_sep = make_sequential_sep(atoms);
	ret->do_group = make_do_group(atoms);
	return (ret);
}

t_term_node		*make_term_node(t_atom **atoms)
{
	t_term_node	*ret;
	t_and_or	*and_or;

	ret = NULL;
	if (!atoms || !*atoms || !(and_or = make_and_or(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->and_or = and_or;
	if ((ret->separator = make_separator(atoms)))
		ret->term = make_term_node(atoms);
	else
		ret->term = NULL;
	return (ret); 
}

t_compound_list		*make_compound_list(t_atom **atoms)
{
	t_term_node	*term;
	t_compound_list	*ret;
	t_newline_list	*list;

	ret = NULL;
	if (!atoms || !*atoms || (
		!(term = make_term_node(atoms)) && !(list = make_newline_list(atoms))))
		return (NULL);
	ret = malloc(sizeof(*ret));
	if (!(ret->term = term ? term : make_term_node(atoms)))
	{
		ft_printf_fd(STDERR_FILENO, "compound list parse error\n");
		return (NULL);
	}
	ret->separator = make_separator(atoms);
	ret->list = list;
	return (ret);
}

t_subshell			*make_subshell(t_atom **atoms)
{
	t_subshell		*ret;
	t_compound_list	*list;
	t_atom			*atom;

	ret = NULL;
	if (!atoms || !*atoms || !((*atoms)->type == DOLL_PAREN))
		return (NULL);
	atom = pop_atom(atoms);
	if (!(list = make_compound_list(atoms)))
	{
		*atoms = atom;
		return (NULL);
	}
	ret = malloc(sizeof(*ret));
	ret->lparen = atom;
	ret->compound_list = list;
	if ((*atoms)->type != RIGHT_PAREN)
	{
		ft_printf_fd(STDERR_FILENO, "subshell parse error\n");
		return (NULL);
	}
	ret->rparen = pop_atom(atoms);
	ft_printf_fd(STDERR_FILENO, "made subshell\n");
	return (ret);
}

t_compound_command	*make_compound_command(t_atom **atoms)
{
	t_compound_command	*ret;
	void				*res;

	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->brace_group = NULL;
	ret->subshell = NULL;
	ret->for_clause = NULL;
	ret->case_clause = NULL;
	ret->if_clause = NULL;
	ret->while_clause = NULL;
	ret->until_clause = NULL;
	if ((res = (void*)make_brace_group(atoms)))
		ret->brace_group = (t_brace_group*)res;
	else if ((res = (void*)make_subshell(atoms)))
		ret->subshell = (t_subshell*)res;
	else if ((res = (void*)make_for_clause(atoms)))
		ret->for_clause = (t_for_clause*)res;
	else if ((res = (void*)make_case_clause(atoms)))
		ret->case_clause = (t_case_clause*)res;
	else if ((res = (void*)make_while_clause(atoms)))
		ret->while_clause = (t_while_clause*)res;
	else if ((res = (void*)make_until_clause(atoms)))
		ret->until_clause = (t_until_clause*)res;
	else
	{
		ft_printf_fd(STDERR_FILENO, "error making compound command\n");
		free(ret);
		return (NULL);
	}
	ft_printf_fd(STDERR_FILENO, "made compound command\n");
	return (ret);
}

t_command			*make_command(t_atom **atoms)
{
	t_command	*ret;
	void		*res;

	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->simple_command = NULL;
	ret->compound_command = NULL;
	ret->redirect_list = NULL;
	ret->function_definition = NULL;
	if ((res = (void*)(make_compound_command(atoms))))
	{
		ret->compound_command = (t_compound_command*)res;
		ret->redirect_list = make_redirect_list(atoms);
	}
	else if ((res = (void*)(make_simple_command(atoms))))
		ret->simple_command = (t_simple_command*)res;
	else if ((res = (void*)(make_function_definition(atoms))))
		ret->function_definition = (t_function_definition*)res;
	else
	{
		free(ret);
		return (NULL);
	}
	ft_printf_fd(STDERR_FILENO, "made command\n");
	return (ret);
}

t_pipe_sequence		*make_pipe_sequence(t_atom **atoms)
{
	t_command		*command;
	t_pipe_sequence	*ret;

	ret = NULL;
	if (!atoms || !*atoms || !(command = make_command(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->command = command;
	if (!atoms || !*atoms || !(ret->pipe = (*atoms)->type == REDIR_PIPE ? pop_atom(atoms) : NULL))
		return (NULL);
	ret->linebreak = make_linebreak(atoms);
	ret->next = make_pipe_sequence(atoms);
	ft_printf_fd(STDERR_FILENO, "made pipe sequence\n");
	return (ret);
}

t_pipeline			*make_pipeline(t_atom **atoms)
{
	t_pipe_sequence	*pipe_sequence;
	t_pipeline		*ret;

	ret = NULL;
	if (!atoms || !*atoms || !(pipe_sequence = make_pipe_sequence(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->pipe_sequence = pipe_sequence;
	ret->bang = (*atoms)->type == BANG ? pop_atom(atoms) : NULL;
	ft_printf_fd(STDERR_FILENO, "made pipeline\n");
	return (ret);
}

t_and_or			*make_and_or(t_atom **atoms)
{
	t_pipeline	*pipeline;
	t_and_or	*ret;

	ret = NULL;
	pipeline = NULL;
	if (!atoms || !*atoms || !(pipeline == make_pipeline(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->pipeline = pipeline;
	ret->and_if = *atoms && (*atoms)->type == AND_IF ? pop_atom(atoms) : NULL;
	ret->or_if = *atoms && (*atoms)->type == OR_IF ? pop_atom(atoms) : NULL;
	ret->next = ret->and_if || ret->or_if ? make_and_or(atoms) : NULL;
	ft_printf_fd(STDERR_FILENO, "made and_or\n");
	return (ret);
}

t_list_node			*make_list_node(t_atom **atoms)
{
	t_list_node		*ret;
	t_and_or		*and_or;

	ret = NULL;
	if (!atoms || !*atoms || !(and_or = make_and_or(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->and_or = and_or;
	ret->separator_op = make_separator_op(atoms);
	ret->next = ret->separator_op ? make_list_node(atoms) : NULL;
	ft_printf_fd(STDERR_FILENO, "made list\n");
	return (ret);
}

t_complete_command	*make_complete_command(t_atom **atoms)
{
	t_complete_command	*ret;
	t_list_node			*list;
	t_separator			*separator;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	if (!(list = make_list_node(atoms)))
		return (NULL);
	separator = make_separator(atoms);
	ret = malloc(sizeof(*ret));
	ret->list = list;
	ret->separator = separator;
	ft_printf_fd(STDERR_FILENO, "made complete command\n");
	return (ret);
}

t_molecule	*moleculizer(t_atom *atoms)
{
	t_molecule	*ret;

	ret = NULL;
	if (!atoms)
	{
		return (NULL);
	}
	ret = malloc(sizeof(*ret));
	ret->command = make_complete_command(&atoms);
	return (ret);
}
