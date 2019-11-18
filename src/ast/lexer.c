/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/25 03:33:40 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

/*
** #define LEX_DEBUG
*/

int			size_redir(char *op)
{
	if (!op)
		return (0);
	if (!(!(ft_strncmp(op, ">&", 2)) || !(ft_strncmp(op, "<&", 2))))
		return (0);
	if (op && op + 1 && op + 2 && *(op + 2) == '-')
		return (3);
	return (2);
}
/*
int			handle_redirect(char *op)
{
	int	i = 0;
	int	found_redir_op = 0;
	int	left_exp_size = 0;
	while (op && op[i])
	{

	}
}

*/

int			handle_redirect(char *op, t_lexeme **head)
{
	int		i = 0;
	char	*io_left = 0;
	char	*io_right = NULL;
	char	*redir = NULL;
	if (!op)
		return (0);
	if (ft_isdigit(*op) || *op == '&')
	{
		io_left = (ft_isdigit(*op)) ? ft_itoa(ft_atoi(op)) : ft_strdup("&");
		new_lex(io_left, IO_NAME, head);
		i += ft_strlen(io_left);
		op += ft_strlen(io_left);
	}
	if (op && (!ft_strncmp(op, "<&", 2) || !ft_strncmp(op, ">&", 2)))
	{
		redir = ft_strdup(!ft_strncmp(op, ">&", 2) ? ">&" : "<&");
		new_lex(redir, (!ft_strncmp(op, ">&", 2) ? R_REDIRECT : L_REDIRECT), head);
		i += 2;
		op += 2;
	}
	if (op && *op && !ft_isspace(*op))
	{
		int j = 0;
		while (op[j] && !ft_isspace(op[j]))
			j++;
		io_right = ft_strndup(op, j + 1);
		new_lex(io_right, IO_NAME, head);
		i += ft_strlen(io_right);
		op += ft_strlen(io_right);
	}
	return (i);
}

int			is_redirect(char *op)
{
	int i = 0;

	
	if (op[i] == '&')
		i++;
	else
		while (op[i] && ft_isdigit(op[i]))
			i++;
	if (op[i] && (op[i] == '<' || op[i] == '>'))
	{
		i++;
		if (op[i] && op[i] == '&')
		{
			i++;
			return (1);
		}
	}
	return (0);
}

/*
int			size_redir(char *op)
{
	if (!op)
		return (0);
	if (!(!(ft_strncmp(op, ">&", 2)) || !(ft_strncmp(op, "<&", 2))))
		return (0);
	if (op && op + 1 && op + 2 && *(op + 2) == '-')
		return (3);
	return (2);
}
*/
int			is_operator(char *op, int pos)
{
	int		i;
	int		ret;

	i = 3;
	ret = 0;
	//if (op && op + pos && (!(ft_strncmp(op + pos, ">&", 2)) || !(ft_strncmp(op + pos, "<&", 2))))
	//	return (11);
	if (is_redirect(op + pos))
		return (12);
	while (g_term.symbls[i] && i < 12)
	{
		if (ft_strncmp(op + pos, g_term.symbls[i],
			ft_strlen(g_term.symbls[i])) == 0)
			return (i);
		i++;
	}
	if (op && ret == 0)
		return (1);
	return (ret);
}

t_lexeme	*new_lex_helper(t_lexeme **head, t_lexeme *new)
{
	t_lexeme	*tmp;

	tmp = *head;
	if (!tmp)
		return ((*head = new));
	while (tmp)
	{
		if (!tmp->next)
		{
			tmp->next = new;
			return (tmp);
		}
		tmp = tmp->next;
	}
	return (tmp);
}

t_lexeme	*new_lex(char *data, enum e_tokentype type, t_lexeme **head)
{
	t_lexeme	*new;
	char		*env;

	new = malloc(sizeof(t_lexeme));
	new->set = type;
	new->data = data;
	new->pos = 0;
	new->designation = BASE;
	new->next = NULL;
	if (data[0] == '$' && data[1] != '(')
	{
		env = find_env(data + 1);
		new->data = env ? ft_strdup(env) : NULL;
		free(data);
	}
	return (new_lex_helper(head, new));
}

int			handle_quote(char *input)
{
	int	i;
	int match_single;
	int	match_double;
	int	ret;

	i = 0;
	match_single = -1;
	match_double = -1;
	while (input[i])
	{
		if ((match_single == 1 && input[i + 1] == '\'') ||
			(match_double == 1 && input[i + 1] == '\"'))
		{
			input[i] = input[i + 2];
			ret = i;
			while (input[i + 1])
			{
				input[i + 1] = input[i + 2];
				i++;
			}
			input[i] = 0;
			return (ft_isspace(input[ret]) ? ret - 1 : ret);
		}
		match_single = input[i] == '\'' ? -match_single : match_single;
		match_double = input[i] == '\"' ? -match_double : match_double;
		if (match_single == 1 && match_double == 1)
			return (-1);
		if (match_single == 1 || match_double == 1)
			input[i] = input[i + 1];
		i++;
	}
	return (1);
}

t_node		*lexer(char *input)
{
	t_lexeme	*ref;
	int			i;
	int			op;
	int			tmp;

	ref = NULL;
	op = 0;
	while (*input)
	{
		i = 0;
		while (*input && ft_isspace(*input))
			input++;
		while (*(input + i) && !ft_isspace(*(input + i)))
		{
			if (*(input + i) == '\'' || *(input + i) == '\"')
				i += handle_quote(input + i);
			op = is_operator(input, i);
			if (op > 1)
				break ;
			if (*(input + i) == '\\')
			{
				tmp = i;
				while (input[i])
				{
					input[i] = input[i + 1];
					i++;
				}
				i = tmp;
			}
			i++;
		}
		i ? new_lex(ft_strndup(input, i), 1, &ref) : 0;
		input += i;
		if (op > 1)
		{
			if (op < L_REDIRECT)
			{
				new_lex(ft_strndup(input, ft_strlen(g_term.symbls[op])), op, &ref);
				input += ft_strlen(g_term.symbls[op]);
			}
			else if (op == L_REDIRECT)
				input += handle_redirect(input, &ref);
			/*
			else if (op == REDIRECT)
			{
				new_lex(ft_strndup(input, size_redir(input)), op, &ref);
				
				input += size_redir(input);
			}
			*/
		}
	}
	ft_printf_fd(STDERR_FILENO, "LEXEMES: ");
	t_lexeme *prnt_tmp = ref;
	while (prnt_tmp)
	{
		ft_printf_fd(STDERR_FILENO, "  |type: %d, str: %s|  ", prnt_tmp->set, prnt_tmp->data);
		prnt_tmp = prnt_tmp->next;
	}
	ft_printf_fd(STDERR_FILENO, "\n");
	return (parser(ref));
}
