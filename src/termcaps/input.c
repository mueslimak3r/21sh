/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:10:40 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/13 07:11:50 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

static int		handle_up_down(unsigned long code, t_tbuff **buff)
{

	if (code == KEY_DOWN && (!*buff || !(*buff)->next))
		return (0);
	if (*buff && (
				((*buff)->next && code == KEY_DOWN)
				|| ((*buff)->prev && code == KEY_UP)))
	{
		/*
		i = (calc_pos() + g_term.conf.prompt_size) / g_term.conf.termsize[0];
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		while (i-- > 0)
			tputs(tgetstr("up", NULL), 0, ft_charput);
		tputs(tgetstr("cd", NULL), 0, ft_charput);
		*/
		if (*buff && (*buff)->buff_str)
		{
			//ft_printf_fd(STDERR_FILENO, "yo\n");
			move_cursor(-(calc_pos()), 1, *buff, -1);
		}
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		tputs(tgetstr("cd", NULL), 0, ft_charput);
		(*buff) = code == KEY_UP
			? (*buff)->prev : (*buff)->next;
		zero_cursor();
		reprint_buffer(*buff, 0, 0);
		//move_cursor(len, 0, *buff);
	}
	return (1);
}

static int		handle_arrows(unsigned long code, t_tbuff **buff)
{
	int	pos;

	pos = calc_pos();
	if (code == KEY_UP || code == KEY_DOWN)
		return (handle_up_down(code, buff));
	//ft_printf_fd(STDERR_FILENO, "pos %d\n", pos);
	if ((code == KEY_LEFT && pos - 1 >= 0)
	 || 
		(code == KEY_RIGHT && pos + 1 <= (*buff)->len))
	{
		//ft_printf_fd(STDERR_FILENO, "arrow\n");
		//if ((code == RIGHT && g_term.conf.cursor[0] + 1 < g_term.conf.termsize[0]) ||
		//(code == LEFT && g_term.conf.cursor[0] - 1 >= 0))
		//	tputs(tgetstr(code == LEFT ? "le" : "nd", NULL), 0, ft_charput);
		move_cursor((code == KEY_LEFT ? -1 : 1), 1, *buff, -1);
	}
	return (0);
}

int		zero_cursor(void)
{
	int	size;

	redo_prompt(0, 0);
	size = g_term.conf.prompt_size;
	g_term.conf.termsize[0] = g_window_size.ws_col;
	g_term.conf.termsize[1] = g_window_size.ws_row;
	g_term.conf.cursor[1] = size != 0 && g_term.conf.termsize[0] != 0 ? (size / g_term.conf.termsize[0]) : 0;
	g_term.conf.cursor[0] = size != 0  && g_term.conf.termsize[0] != 0 ? (size % g_term.conf.termsize[0]) : 0;
	g_term.conf.curlines = g_term.conf.cursor[1] + 1;
	return (1);
}

static int		handle_enter(t_tbuff *buff)
{
	if (buff)
	{
		;
	} //&& buff->buff_str)
	//	while (g_term.conf.curlines > 1)
	//		g_term.conf.curlines--;
	//tputs(tgetstr("cr", NULL), 0, ft_charput);
	//tputs(tgetstr("sf", NULL), 0, ft_charput);
	return (1);
}

int				handle_controls(unsigned long code, t_tbuff **buff)
{
	int ret;
	int	cursor_pos;

	ret = 0;
	cursor_pos = calc_pos();
	//printf("\n%lu\n\n", code);
	if (code == KEY_BACKSPACE)
	{
		if (cursor_pos > 0)
		{
			if (buff && *buff && !(*buff)->temp)
				tbuff_replicate(buff);
			t_buff_line_rm(*buff, --cursor_pos, 1);
			move_cursor(-1, 1, *buff, cursor_pos);
			int amt = ((*buff)->len - cursor_pos) == 0 ? 0 : -((*buff)->len - cursor_pos);
			cursor_pos = calc_pos();
			reprint_buffer(*buff, cursor_pos, 0);
			move_cursor(amt, 1, *buff, -1);
		}
	}
	else if (code == KEY_ENTER)
		handle_enter(*buff);
	else if (code == KEY_ALT_UP || code == KEY_ALT_DOWN)
		jump_by_row(*buff, code);
	else if (code == KEY_UP || code == KEY_DOWN ||
		code == KEY_LEFT || code == KEY_RIGHT)
		handle_arrows(code, buff);
	else if (code == KEY_HOME || code == KEY_END)
		move_cursor(code == KEY_HOME ? -(calc_pos()) : (int)ft_strlen((*buff)->buff_str) - calc_pos(), 1, *buff, -1);
	else if (code == KEY_ALT_LEFT || code == KEY_ALT_RIGHT)
	{
		int i;
	   	
		i = jump_by_word_amt((*buff)->buff_str, calc_pos(),
				code == KEY_ALT_LEFT ? 1 : -1);
		move_cursor(i - calc_pos(), 1, *buff, -1);
	}
	else
		ret -= 1;
	ret += 1;
	return (ret);
}
