#include "ftshell.h"

t_atom				*pop_atom(t_atom **atoms)
{
	t_atom	*ret;

	if (!atoms || !(ret = *atoms))
		return (NULL);
	*atoms = (*atoms)->next;
	return (ret);
}

/*
** please send help
**            --alkozma
*/

t_case_item_ns		*make_case_item_ns(t_atom **atoms)
{
	t_case_item_ns	*ret;
	t_pattern	*pat;

	ret = NULL;
	if (!atoms || !*atoms || (*atoms)->type != LPAREN ||
		!(pat = make_pattern(atoms)))
		return (NULL);
	ret->lparen = (*atoms)->type == LPAREN ? pop_atom(atoms) : NULL;
	ret->pattern = pat;
	ret->rparen = pop_atom(atoms);
	ret->compound_list = make_compound_list(atoms);
	ret->linebreak = make_linebreak(atoms);
	return (ret);
}

t_case_list		*make_case_list(t_atom **atoms)
{
	t_case_list_ns	*ret;
	t_case_item	*item;
	t_case_list	*list;

	ret = NULL;
	if (!atoms || !*atoms || (!(item = make_case_item(atoms)) &&
		!(list = make_case_list(atoms))))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->case_list = list;
	ret->case_item = item;
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
	ret->case_list = list;
	ret->case_item_ns = item;
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
		ret->case_list_ns = make_case_list_ns(atoms)
	else
		ret->case_list_ns = NULL;
	ret->esac = pop_atom(atoms);
	return (ret);
}

t_wordlist		*make_wordlist(t_atom **atoms)
{
	t_wordlist	*ret;
	t_word		*word;

	ret = NULL;
	if (!atoms || !*atoms || !(word = make_word(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = word;
	ret->list = make_wordlist(atoms);
	return (ret);
}

t_in			*make_in(t_atoms **atoms)
{
	t_in	*ret;

	ret = NULL;
	if (!atoms || !*atoms)
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
	return (ret);
}

t_name			*make_name(t_atoms **atoms)
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
		if (!ft_isnumber((*atoms)->str[i]) &&
			ft_isnumber((*atoms)->str[i]) &&
			(*atoms)->str[i] != '_')
			return (NULL);
		i++;
	}
	ret = malloc(sizeof(*ret));
	ret->word = pop_atom(atoms);
}

t_for_clause		*make_for_clause(t_atoms **atoms)
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

t_term_node		*make_term_node(t_atoms **atoms)
{
	t_term_node	*ret;
	t_and_or	*and_or;

	ret = NULL;
	if (!atoms || !*atoms || !(and_or = make_and_or(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->and_or = and_or;
	ret->term_separator = make_term_separator(atoms);
	return (ret); 
}

t_compound_list		*make_compound_list(t_atoms **atoms)
{
	t_term_node	*term;
	t_compound_list	*ret;
	t_newline_list	*list;

	ret = NULL;
	if (!atoms || !*atoms || (
		!(term = make_term_node(atoms)) && !(list = make_newline_list(atoms))))
		return (NULL);
	ret = malloc(sizeof(*ret));
	if (!(ret->term = term ? term : make_term(atoms)))
	{
		ft_printf_fd(STDERR_FILENO, "compound list parse error\n");
		return (NULL);
	}
	ret->separator = make_separator(atoms);
	ret->list = list;
	return (ret);
}

t_subshell			*make_subshell(t_atoms **atoms)
{
	t_subshell		*ret;
	t_compound_list	*list;
	t_atom			*atom;

	ret = NULL;
	if (!atoms || !*atoms || !((*atoms)->type == LEFT_PAREN))
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
	if (!(*atoms)->type == RIGHT_PAREN)
	{
		ft_printf_fd(STDERR_FILENO, "subshell parse error\n");
		return (NULL);
	}
	ret->rparen = pop_atom(atoms);
	return (ret);
}

t_compound_command	*make_compound_command(t_atoms **atoms)
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
		free(ret);
		return (NULL);
	}
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
	if ((res = (void*)(make_simple_command(atoms))))
		ret->simple_command = (t_simple_command*)res;
	else if ((res = (void*)(make_compound_command(atoms))))
	{
		ret->compound_command = (t_compound_command*)res;
		ret->redirect_list = make_redirect_list(atoms);
	}
	else if ((res = (void*)(make_function_definition(atoms))))
		ret->function_definition = (t_function_definition*)res;
	else
	{
		free(ret);
		return (NULL);
	}
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
	ret->pipe = (*atoms)->type == REDIR_PIPE ? pop_atom(atoms) : NULL;
	ret->linebreak = make_linebreak(atoms);
	ret->next = make_pipe_sequence(atoms);
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
	return (ret);
}

t_and_or			*make_and_or(t_atom **atoms)
{
	t_pipeline	*pipeline;
	t_and_or	*ret;

	ret = NULL;
	if (!atoms || !*atoms || !(pipeline == make_pipeline(atoms)))
		return (NULL);
	ret = malloc(sizeof(*ret));
	ret->pipeline = pipeline;
	ret->and_if = (*atoms)->type == AND_IF ? pop_atom(atoms) : NULL;
	ret->or_if = (*atoms)->type == OR_IF ? pop_atom(atoms) : NULL;
	ret->next = make_and_or(atoms);
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
	ret->next = make_list_node(atoms);
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
	return (ret);
}

t_molecule	*moleculizer(t_atom *atoms)
{
	if (!atom)
	{
		//
	}
	return (NULL);
}
