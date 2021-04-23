/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 21:08:04 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 21:09:01 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	no_home(t_shell *shell)
{
	ft_putstr_fd(PROMT, 2);
	ft_putstr_fd(shell->cmd_n_args[0], 2);
	ft_putstr_fd(":", 2);
	ft_putstr_fd(shell->cmd_n_args[1], 2);
	ft_putstr_fd(":", 2);
	ft_putstr_fd(strerror(errno), 2);
}

int		check_path(t_shell *shell, char **home)
{
	shell->kv = shell->kv_head;
	if ((*home = ft_strdup(find_key_value("HOME", &shell->kv))) == NULL)
	{
		no_home(shell);
		return (0);
	}
	return (1);
}

void	err_no_dir_cd(char *dir)
{
	g_exit = 127;
	ft_putstr_fd("bush", 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("cd", 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(dir, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("No such file or directory", 2);
	write(2, "\n", 1);
}
