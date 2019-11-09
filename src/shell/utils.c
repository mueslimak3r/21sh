/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 10:24:04 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/08 17:49:58 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		ft_printf_fd(int fd, const char *fmt, ...)
{
	va_list	vargs;
	char	*str = NULL;

	va_start(vargs, (char*)fmt);
	while (*fmt)
	{
		if (*fmt == '%' && *(fmt + 1) == 's')
		{
			str = va_arg(vargs, char*);
			ft_putstr_fd(str, fd);
			fmt += 2;
		}
		else if (*fmt == '%' && *(fmt + 1) == 'd')
		{
			str = ft_itoa(va_arg(vargs, int));
			ft_putstr_fd(str, fd);
			free(str);
			str = NULL;
			fmt += 2;
		}
		else if (*fmt == '%' && *(fmt + 1) && *(fmt + 2) && ft_strncmp(fmt + 1, "lu", 2) == 0)
		{
			str = ft_uitoa_base(va_arg(vargs, int), 10);
			ft_putstr_fd(str, fd);
			free(str);
			str = NULL;
			fmt += 3;
		}
		else
		{
			ft_putchar_fd(*fmt, fd);
			fmt += 1;
		}
	}
	return (1);
}

void	add_to_rope(const char *data)
{
	size_t	len;
	int		pos = 0;
	char	buff[LEAF_SIZE + 1];
	if (!data)
		return ;
	len = ft_strlen(data);
	/*while (len > 0)
	{
		ft_memset();
		g_term.curr_buff->rope = rope_insert(g_term.curr_buff->rope,
				(char*)data, g_term.curr_buff->cursor + 1);
		data += (len > 6 ? 6 : len);
		len -= (len > 6 ? 6 : len);
	}
	*/
	int leftover = len;
	while (leftover > 0)
	{
		ft_printf_fd(STDERR_FILENO, "loop leftover: %d\n", leftover);
		ft_memmove(buff, data + pos, (leftover >= LEAF_SIZE) ? LEAF_SIZE : leftover);
		g_term.curr_buff->rope = rope_insert(g_term.curr_buff->rope, buff, g_term.curr_buff->cursor + 1);
		leftover -= (leftover >= LEAF_SIZE) ? LEAF_SIZE : leftover;
		pos += (leftover >= LEAF_SIZE) ? LEAF_SIZE : leftover;
	}
}

/*
int		next_exp(const char *fmt, size_t pos)
{
	while (ft_isspace(fmt[pos]))
		pos++;
	return (pos);
}

int		count_padding(const char *fmt, size_t pos, size_t *lpadding, size_t *rpadding)
{
	int	padding_side;
	int i;

	i = pos;
	padding_side = 0;
	lpadding = ft_atoi(fmt + pos);
	i = pos + ft_cntdigit(ft_atoi(fmt + pos), 10);
	if (fmt[i] == '.')
	{
		rpadding = ft_atoi(fmt + ++i);
		i + ft_cntdigit(ft_atoi(fmt + i), 10);
	}
	return (i);
}

int		handle_types(va_list *vargs, const char *fmt, size_t pos, size_t len, int fd)
{
	size_t	i;
	size_t	lpadding;
	size_t	rpadding;
	char	*str;

	lpadding = 0;
	rpadding = 0;
	i = count_padding(fmt, pos + 1, &lpadding, &rpadding);
	if (i < len)
	{
		if (fmt[i] == 'd')
		{
			str = ft_itoa(va_arg(*vargs, int));
			ft_putstr_fd(str, fd);
			free(str);
			str = NULL;
		}
		else if (fmt[i] == 's')
		{
			str = va_arg(*vargs, char*);
			ft_putstr_fd(str, fd);
		}
		else
			return ();
	}
	return (1);
}

int		ft_printf_fd(int fd, const char *fmt, ...)
{
	va_list	vargs;
	char	*str = NULL;
	size_t	len;
	size_t	i;

	i = 0;
	len = ft_strlen(fmt);
	va_start(vargs, (char*)fmt);
	while (fmt[i])
	{
		if (fmt[i] == '%')
		{
			i += handle_types(fmt, i, len, fd);
		}
		else
		{
			ft_putchar_fd(fmt[i], fd);
			i++;
		}
	}
	return (1);
}
*/
