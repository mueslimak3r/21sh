/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/24 22:02:06 by calamber          #+#    #+#             */
/*   Updated: 2019/10/16 17:59:54 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

static void		count_quotes(char **line, int *words, char quotet, int *ret)
{
	(*line)++;
	while ((**line) != quotet && (**line))
		(*line)++;
	if (**line)
	{
		(*line)++;
		(*words)++;
	}
	else
		*ret = 1;
}

int				sh_count_words(char *line)
{
	int			words;
	int			ret;

	words = 0;
	ret = 0;
	while (*line)
	{
		while (*line && *line <= 32)
			line++;
		if ((*line == '\"' && *(line + 1) != '\"') ||
			(*line == '\'' && *(line + 1) != '\''))
		{
			count_quotes((&line), &words, *line, &ret);
			if (ret == 1)
				return (words);
		}
		if (*line > 32 && *line != '\"' && *line != '\'')
			words++;
		while (*line > 32 && *line != '\"' && *line != '\'')
			line++;
		if ((*line == '\"' && *(line + 1) == '\"') ||
			(*line == '\'' && *(line + 1) == '\''))
			line += 2;
	}
	return (words);
}

static char		*handle_quotes(char *line, int *j, char type)
{
	int			size;

	size = 0;
	(*j)++;
	while (line[*j] != type && line[*j])
	{
		size++;
		(*j)++;
	}
	if (line[*j])
	{
		(*j)++;
		return (ft_strndup(line + *j - size - 1, size));
	}
	else
		return (ft_strnew(1));
}

char			*sh_next_word(char *line, int *j)
{
	int			size;

	while (*(line + *j))
	{
		while (*(line + *j) && *(line + *j) <= 32)
			(*j)++;
		if ((*(line + *j) == '\"' && *(line + *j + 1) != '\"') ||
			(*(line + *j) == '\'' && *(line + *j + 1) != '\''))
			return (handle_quotes(line, j, line[*j]));
		if (*(line + *j) > 32 && *(line + *j) != '\"' && *(line + *j) != '\'')
		{
			size = 0;
			while (*(line + *j + size) > 32 && *(line + *j + size) != '\"'
				&& *(line + *j + size) != '\'')
				size++;
			while (*(line + *j) > 32 && *(line + *j) != '\"' &&
				*(line + *j) != '\'')
				(*j)++;
			return (ft_strndup(line + *j - size, size));
		}
		if ((*(line + *j) == '\"' && *(line + *j + 1) == '\"') ||
			(*(line + *j) == '\'' && *(line + *j + 1) == '\''))
			*j += 2;
	}
	return (0);
}
