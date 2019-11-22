/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 19:12:13 by calamber          #+#    #+#             */
/*   Updated: 2019/11/21 20:27:58 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int			handle_redirect_2(t_lexeme **head, char *op, int i)
{
	char	*io_right;
	int		j;

	io_right = NULL;
	j = 0;
	if (op && *op && !ft_isspace(*op))
	{
		while (op[j] && !ft_isspace(op[j]))
			j++;
		io_right = ft_strndup(op, j + 1);
		new_lex(io_right, IO_NAME, head);
		i += ft_strlen(io_right);
		op += ft_strlen(io_right);
	}
	return (i);
}

int			handle_redirect(char *op, t_lexeme **head)
{
	int		i;
	char	*io_left;
	char	*redir;

	i = 0;
	redir = NULL;
	if (!op)
		return (0);
	if (ft_isdigit(*op))
	{
		io_left = ft_itoa(ft_atoi(op));
		new_lex(io_left, IO_NAME, head);
		i += ft_strlen(io_left);
		op += ft_strlen(io_left);
	}
	if (op && (!ft_strncmp(op, "<&", 2) || !ft_strncmp(op, ">&", 2) ||
				!ft_strncmp(op, "&>", 2) || !ft_strncmp(op, "&>", 2)))
	{
		redir = ft_strndup(op, 2);
		new_lex(redir, ((!ft_strncmp(op, ">&", 2) ||
			!ft_strncmp(op, "&>", 2)) ? R_REDIRECT : L_REDIRECT), head);
		i += 2;
		op += 2;
	}
	return (handle_redirect_2(head, op, i));
}

int			is_redirect(char *op)
{
	int i;
	int	found_amp;

	found_amp = 0;
	i = 0;
	if (op[i] == '&')
	{
		found_amp = 1;
		i++;
	}
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
		else if (found_amp)
			return (1);
	}
	return (0);
}

int			is_nb_before_redir(char *op)
{
	int	i;

	i = 0;
	if (!op)
		return (0);
	if (ft_isdigit(op[i]))
	{
		while (op[i] && ft_isdigit(op[i]))
			i++;
		if (op[i] && (op[i] == '>' || !ft_strncmp(op + i, ">>", 2)))
		{
			ft_printf_fd(STDERR_FILENO, "found nb before redir\n");
			return (1);
		}
	}
	return (0);
}
