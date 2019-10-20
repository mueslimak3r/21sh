/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/20 02:49:50 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"
//#define LEX_DEBUG

int			is_operator(char *op, int pos)
{
	int		i;
	int		ret;
	

	i = 3;
	ret = 0;
	while (g_term.symbls[i])
	{
		//if (pos > 0 && ft_strncmp(op + pos - 1, g_term.symbls[i], ft_strlen(g_term.symbls[i])) == 0)
		//	return (2);
		if (ft_strncmp(op + pos, g_term.symbls[i], ft_strlen(g_term.symbls[i])) == 0)
			return (i);
		i++;
	}
	if (op && ret == 0)
		return (1);
	return (ret);
}

void	lexer_queue_push(t_lexeme **front, t_lexeme **back, char *str, enum e_tokentype set)
{
	t_lexeme	*new;
	char		*rep;

	if (!str || !(new = ft_memalloc(sizeof(t_lexeme))))
		return ;
	if (*back)
		(*back)->next = new;
	else
		*front = new;
	*back = new;
	if (ft_strlen(str) >= 2 && str[0] == '$' && str[1] != '(')
		new->data = (rep = find_env(str + 1)) ? ft_strdup(rep) : NULL;
	else
		new->data = str;
	new->set = set;
	new->next = NULL;
}

void	line_lexer(t_lexeme **front, t_lexeme **back, char *line)
{
	int words;
	int j;
	int	i;

	j = 0;
	i = 0;
	words = sh_count_words(line);
	while (i++ < words)
	{
		char *word = sh_next_word(line, &j);
		if (!word || !*word)
		{
			ft_printf_fd(STDERR_FILENO, "syntax error!\n");
			break ;
		}
		lexer_queue_push(front, back, word, is_operator(word, 0));
	}
}

/*t_node	*lexer(char *input)
{
	t_lexeme	*front;
	t_lexeme	*back;

	front = NULL;
	back = NULL;
	if (!input)
		return (NULL);
	line_lexer(&front, &back, input);
	t_lexeme *tmp = front;
#ifdef LEX_DEBUG
	while (tmp)
	{
		ft_printf_fd(STDERR_FILENO, "TYPE: %s, STR: %s\n", g_term.symbls[tmp->set], tmp->data);
		tmp = tmp->next;
	}
#endif
	return(parser(front));
}*/

t_lexeme	*new_lex(char *data, enum e_tokentype type, t_lexeme **head)
{
	t_lexeme	*new;
	t_lexeme	*tmp;
	char		*env;

	tmp = *head;
	new = malloc(sizeof(t_lexeme));
	new->set = type;
	new->data = data;
	new->pos = 0;
	new->designation = BASE;
	new->next = NULL;
	if (data[0] == '$' && data[1] != '(')
	{
		if ((env = find_env(data + 1)))
		{
			new->data = ft_strdup(env);
			free(data);
		}
	}
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

void		printthings(t_lexeme *ref)
{
	t_lexeme *tmp;

	tmp = ref;
	while (tmp)
	{
		ft_printf_fd(STDERR_FILENO, "lexeme data: [%s]\n", tmp->data);
		tmp = tmp->next;
	}
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
		if ((match_single == 1 && input[i + 1] == '\'') || (match_double == 1 && input[i + 1] == '\"'))
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
			new_lex(ft_strndup(input, ft_strlen(g_term.symbls[op])), op, &ref);
			input += ft_strlen(g_term.symbls[op]);
		}
	}
	//printthings(ref);
	return (parser(ref));
}
