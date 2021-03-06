/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 10:24:04 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/20 15:59:27 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

bool		validate_term(void)
{
	char	*term_id;
	char	buf[2048];

	term_id = getenv("TERM");
	if (!term_id)
		ft_putstr_fd("error: $TERM variable invalid\n",
		STDERR_FILENO);
	else if (!tgetent(buf, term_id))
		ft_putstr_fd("error: tgetent failed\n",
		STDERR_FILENO);
	else
		return (true);
	return (false);
}

void		init_term(void)
{
	char	buf[150];
	char	*temp;

	temp = buf;
	tcgetattr(STDERR_FILENO, &g_term.old_term);
	tcgetattr(STDERR_FILENO, &g_term.new_term);
	g_term.new_term.c_lflag &= ~(ICANON | ECHO);
	if (!(tgetstr("vi", &temp)))
	{
		ft_putstr_fd(temp, STDERR_FILENO);
		exit(0);
	}
	tcsetattr(STDERR_FILENO, TCSANOW, &g_term.new_term);
	ioctl(STDERR_FILENO, TIOCGWINSZ, &g_window_size);
	zero_cursor(0);
}

void		reset_term(void)
{
	tcsetattr(STDERR_FILENO, TCSANOW, &g_term.old_term);
}

int			handle_s_d(int fd, char **fmt, va_list *vargs)
{
	char *str;

	str = NULL;
	if (!*fmt)
		return (0);
	if (**fmt == '%' && *(*fmt + 1) == 's')
	{
		str = va_arg(*vargs, char*);
		ft_putstr_fd(str, fd);
		*fmt += 2;
	}
	else if (**fmt == '%' && *(*fmt + 1) == 'd')
	{
		str = ft_itoa(va_arg(*vargs, int));
		ft_putstr_fd(str, fd);
		free(str);
		str = NULL;
		*fmt += 2;
	}
	else
		return (0);
	return (1);
}

int			ft_printf_fd(int fd, char *fmt, ...)
{
	va_list	vargs;
	char	*str;

	str = NULL;
	va_start(vargs, (char*)fmt);
	while (*fmt)
	{
		if (handle_s_d(fd, &fmt, &vargs))
			continue ;
		else if (*fmt == '%' && *(fmt + 1) && *(fmt + 2) &&
			ft_strncmp(fmt + 1, "lu", 2) == 0)
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
