# include "ftshell.h"

# define REDIR_L '<'
# define REDIR_R '>'
# define REDIR_LL "<<"
# define REDIR_RR ">>"
# define PIPE '|'
# define AND '&'
# define WAIT ';'

typedef struct s_cdata  t_cdata;

struct s_cdata
{
    char *command;
    char **args;

};

int	is_operator(char *data)
{
	if (!data || !*data)
		return (-1);
	if (*data == REDIR_L || *data == REDIR_R || *data == PIPE
			|| *data == AND || *data == WAIT)
	{
		if (*data == REDIR_L && *(data + 1) == REDIR_L)
			return (1);
		if (*data == REDIR_R && *(data + 1) == REDIR_L)
			return (1);
		else
			return (0);
	}
	return (-1);
}

enum e_tokentype	define_flags(char *data)
{
	enum e_tokentype	e;

	if (*data == AND || *data == WAIT)
		return ((e = OPERATOR));
	if (*data == REDIR_L || *data == REDIR_R || *data == PIPE)
		return ((e = REDIRECT));
	return ((e = WORD));
}

void		add_token(t_token **head, t_token *new)
{
	t_token	*old;

	old = *head;
	if (!old)
		*head = new;
	else
	{
		while (old->next)
			old = old->next;
		old->next = new;
	}
}

t_token		*new_token(char *data)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	new->type = (int)define_flags(data);
	new->name = data;
	new->next = NULL;
	return (new);
}

t_token		*parse_input(char *input)
{
	int	cur;
	int	last;
	t_token	*tokens;
	int	res;

	last = 0;
	cur = 0;
	tokens = NULL;
	while (input[cur])
	{
		if ((res = is_operator(input + cur)) >= 0)
		{
			add_token(&tokens, new_token(ft_strsub(input, last, res + cur - last == 0 ? 1 : cur - last)));
			if (last != cur && input[cur + 1])
				add_token(&tokens, new_token(ft_strsub(input, cur, 1 + res)));
			last = cur + 1 + res;
		}
		cur += 1 + (res == 1 ? res : 0);
	}
	add_token(&tokens, new_token(ft_strsub(input, last, cur - last)));
	/*
	while (tokens)
	{
		ft_printf("[%s] (%d)\n", tokens->name, tokens->type);
		tokens = tokens->next;
	}
	*/
	return (tokens);
}
