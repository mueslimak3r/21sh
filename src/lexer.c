/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/16 12:07:28 by alkozma          ###   ########.fr       */
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

	if (!str)
		return ;
	if (!(new = ft_memalloc(sizeof(t_lexeme))))
		return ;
	if (*back)
		(*back)->next = new;
	else
		*front = new;
	*back = new;
	if (ft_strlen(str) >= 2 && str[0] == '$' && str[1] != '(')
	{
		if ((rep = find_env2(str + 1)))
			new->data = ft_strdup(rep);
		else
			new->data = NULL;
	}
	else
		new->data = str;
	new->set = set;
	new->next = NULL;
}

void	line_lexer(t_lexeme **front, t_lexeme **back, char *line)
{
	int words = sh_count_words(line);
	int j = 0;

	for (int i = 0; i < words; i++)
	{
		char *word = sh_next_word(line, &j);
		if (!word)
			break ;
		if (!*word)
		{
			ft_printf_fd(STDERR_FILENO, "syntax error!\n");
			break ;
		}
		lexer_queue_push(front, back, word, is_operator(word, 0));
	}
}

t_node	*lexer(char *input)
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
}
