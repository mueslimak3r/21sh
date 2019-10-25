/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 10:24:04 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/25 04:12:59 by calamber         ###   ########.fr       */
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
		}
		else if (*fmt == '%' && *(fmt + 1) == 'd')
		{
			str = ft_itoa(va_arg(vargs, int));
			ft_putstr_fd(str, fd);
			free(str);
			str = NULL;
		}
		else
			ft_putchar_fd(*(fmt--), fd);
		fmt += 2;
	}
	return (1);
}
