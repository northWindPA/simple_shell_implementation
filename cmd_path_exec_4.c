/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_exec_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:19:13 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:56:29 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_no_file_dir(void)
{
	g_exit = 127;
	ft_putstr_fd("No such file or directory", 2);
	write(2, "\n", 1);
}

void	err_cmd_n_found(void)
{
	g_exit = 127;
	ft_putstr_fd("command not found", 2);
	write(2, "\n", 1);
}

void	err_cmd(t_shell *shell)
{
	struct stat s;

	stat(shell->cmd_n_args[0], &s);
	if (S_ISDIR(s.st_mode))
		err_is_dir();
	else if (S_ISREG(s.st_mode))
		err_perm_deny();
	else if (file_dir(shell->cmd_n_args[0]))
		err_no_file_dir();
	else
		err_cmd_n_found();
}
