/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:50:01 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:50:23 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_out(char *str)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
}

int		chk_tkn(char *str, char *line)
{
	if ((ft_strcmp(str, line) != 0) || (ft_strlen(str) != ft_strlen(line)))
		return (0);
	return (1);
}

int		len_2d(char **array)
{
	int i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}
