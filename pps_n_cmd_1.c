/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pps_n_cmd_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 15:49:29 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:44:15 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_change_fd(t_shell *shell, int i)
{
	g_exit = 0;
	if (i > 0)
	{
		close(shell->pp_l[0]);
		close(shell->pp_l[1]);
	}
	shell->pp_l[0] = shell->pp_r[0];
	shell->pp_l[1] = shell->pp_r[1];
}

void	parent(t_shell *shell, int i)
{
	close_change_fd(shell, i);
	wait(&shell->stts);
	if (WIFEXITED(shell->stts) != 0)
	{
		if (WEXITSTATUS(shell->stts) == 3)
			cmd_not_found(shell);
		g_exit = 1;
	}
}

int		cmd_exec(t_shell *shell)
{
	int		i;
	pid_t	pid;
	int		pip_res;

	i = 0;
	pip_res = 0;
	while (i <= shell->pps)
	{
		if (i != shell->pps && shell->pps > 0)
		{
			pip_res = pipe(shell->pp_r);
			if (pip_res < 0)
				err_out("pipe error");
		}
		pid = fork();
		if (pid == 0)
			child(shell, i);
		else if (pid < 0)
			err_out("fork error");
		else
			parent(shell, i);
		i++;
	}
	return (0);
}
