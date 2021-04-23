/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_exec_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 19:10:53 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:55:59 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_parent(t_shell *shell, int stts)
{
	if (stts == 0)
	{
		g_exit = 0;
		return ;
	}
	if (WIFEXITED(stts) != 0)
	{
		g_exit = 1;
		if (WEXITSTATUS(stts) == 127)
		{
			g_exit = 1;
			ft_putstr_fd("bush", 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(shell->cmd_n_args[0], 2);
			ft_putstr_fd(": ", 2);
			err_cmd(shell);
		}
	}
}

void	exec_child(t_shell *shell, char *cmd_n_path, int stts)
{
	if ((stts = execve(cmd_n_path, shell->cmd_n_args, shell->dup_env)))
		exit(127);
	free(cmd_n_path);
	exit(stts);
}

int		exec_other_bins(t_shell *shell)
{
	pid_t	pid;
	int		stts;
	char	*cmd_n_path;

	g_exit = 0;
	stts = 0;
	signals_start(2);
	cmd_n_path = bin_path(shell, shell->cmd_n_args[0]);
	if ((pid = fork()) < 0)
		err_out("fork()");
	if (pid == 0)
		exec_child(shell, cmd_n_path, stts);
	else
	{
		waitpid(pid, &stts, 0);
		exec_parent(shell, stts);
		if (cmd_n_path)
			free(cmd_n_path);
	}
	return (0);
}
