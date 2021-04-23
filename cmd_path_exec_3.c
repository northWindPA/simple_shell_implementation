/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_exec_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:19:15 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:56:16 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		file_dir(char *cmd)
{
	int i;
	int fl;

	if (!cmd)
		return (0);
	i = 0;
	fl = 0;
	if (cmd[i] == '/')
	{
		fl = 1;
		i++;
	}
	while (cmd[i] == '.' && cmd[i] == '/' && cmd[i])
		i++;
	if (i == 1)
		return (1);
	if (!fl)
		return (0);
	return (1);
}

void	err_is_dir(void)
{
	g_exit = 126;
	ft_putstr_fd("is a directory", 2);
	write(2, "\n", 1);
}

void	err_perm_deny(void)
{
	g_exit = 126;
	ft_putstr_fd("Permission denied", 2);
	write(2, "\n", 1);
}
