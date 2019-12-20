/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_classify.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:15:00 by calamber          #+#    #+#             */
/*   Updated: 2019/12/20 12:53:00 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

/*
** classify
** Searches input for context and applies a type based on that context.
*/

int				is_fd_lit(t_lexeme *lexeme)
{
	if (!lexeme)
		return (0);
	if (lexeme->set == IO_NAME)
	{
		if (is_mod(lexeme->next) && lexeme->next->next && (
				lexeme->next->next->set == IO_NAME ||
				lexeme->next->next->set == WORD))
		{
			lexeme->next->designation = REDIR;
			lexeme->next->next->designation = FD_LIT;
			if (is_arg(lexeme->next->next->next))
				lexeme->next->next->next->designation = ARG;
		}
		return (1);
	}
	return (-1);
}

int				is_mod(t_lexeme *lexeme)
{
	if (!lexeme)
		return (0);
	if (lexeme->set >= AND && lexeme->set <= IO_NAME)
	{
		if (lexeme->next && (lexeme->set == LESS || lexeme->set == GREAT
			|| lexeme->set == RDGREAT || lexeme->set == RDLESS))
		{
			lexeme->next->designation = lexeme->set == RDLESS ? FD_H :
				lexeme->next->designation;
			lexeme->next->designation = lexeme->set == LESS ? FD_R :
				lexeme->next->designation;
			lexeme->next->designation = lexeme->set == GREAT ? FD_W :
				lexeme->next->designation;
			lexeme->next->designation = lexeme->set == RDGREAT ? FD_A :
				lexeme->next->designation;
		}
		if ((!lexeme->next && lexeme->set == SEMI) ||
			(lexeme->next && !is_mod(lexeme->next)))
			return (1);
		if (lexeme->next && (lexeme->set == L_REDIRECT
						|| lexeme->set == R_REDIRECT))
			return (1);
	}
	return (0);
}

int				is_arg(t_lexeme *lexeme)
{
	if (!lexeme)
		return (0);
	if (lexeme->set == WORD && (!lexeme->next || (is_arg(lexeme->next)
		|| is_mod(lexeme->next) > 0 || is_fd_lit(lexeme->next) > 0)))
	{
		if (is_arg(lexeme->next))
			lexeme->next->designation = ARG;
		else if (is_mod(lexeme->next))
			lexeme->next->designation = MOD;
		return (1);
	}
	return (0);
}

int				is_exec(t_lexeme *lexeme)
{
	if (!lexeme)
		return (0);
	if (lexeme->set == WORD && (!lexeme->next || (is_arg(lexeme->next)
		|| is_mod(lexeme->next) > 0 || is_fd_lit(lexeme->next) > 0)))
	{
		if (is_arg(lexeme->next))
			lexeme->next->designation = ARG;
		else if (is_mod(lexeme->next))
			lexeme->next->designation = MOD;
		return (1);
	}
	return (0);
}

enum e_nodetype	classify(t_lexeme *lexeme)
{
	int		res;

	if (!lexeme)
		return (1);
	if (lexeme->designation != BASE)
		return (lexeme->designation);
	if ((res = is_exec(lexeme)))
		return (res > 0 ? EXEC : ERR);
	else if ((res = is_arg(lexeme)))
		return (res > 0 ? ARG : ERR);
	else if ((res = is_mod(lexeme)))
		return (res > 0 ? MOD : ERR);
	else if ((res = is_fd_lit(lexeme)))
		return (res > 0 ? FD_LIT : ERR);
	return (2);
}
