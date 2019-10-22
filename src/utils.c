/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 10:24:04 by alkozma           #+#    #+#             */
/*   Updated: 2019/09/21 11:06:46 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		ft_printf_fd(int fd, const char *fmt, ...)
{
	va_list	vargs;
	char	*str;

	str = 0;
	va_start(vargs, (char*)fmt);
	while (*fmt)
	{
		if (*fmt == '%' && *(fmt + 1) == 's')
		{
			fmt++;
			str = va_arg(vargs, char*);
			ft_putstr_fd(str, fd);
		}
		else if (*fmt == '%' && *(fmt + 1) == 'd')
		{
			fmt++;
			str = ft_itoa(va_arg(vargs, int));
			ft_putstr_fd(str, fd);
			free(str);
			str = NULL;
		}
		else
			ft_putchar_fd(*fmt, fd);
		fmt++;
	}
	return (1);
}
