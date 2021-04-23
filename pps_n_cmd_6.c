/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pps_n_cmd_7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:43:31 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:44:04 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		rdr_in(t_shell *shell, int i)
{
	if (shell->fl_rdr_in && !i)
	{
		shell->in_fd = open(shell->p_in, O_RDONLY);
		if (shell->in_fd < 0)
		{
			ft_putstr_fd("bush", 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(shell->p_in, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			g_exit = 1;
		}
		close(0);
		dup(shell->in_fd);
		close(shell->in_fd);
	}
	return (0);
}

void	child(t_shell *shell, int i)
{
	char	*cmd_n_path;

	if (shell->fl_rdr_in && !i)
		rdr_in(shell, i);
	if (shell->fl_rdr_out
	&& i == shell->pps)
		rdr_out(shell);
	if (shell->fl_rdr_app
	&& i == shell->pps)
		rdr_app(shell);
	if (shell->pps > 0)
		pps_fd(shell, i);
	if (!(cmd_n_path = bin_path(shell,
	shell->cmd_n_args[shell->ind_arg[i]])))
		exit(3);
	shell->stts = execve(cmd_n_path, &shell->cmd_n_args
	[shell->ind_arg[i]], shell->dup_env);
	if (cmd_n_path)
		free(cmd_n_path);
	err_out(cmd_n_path);
	exit(shell->stts);
}

void	cmd_not_found(t_shell *shell)
{
	ft_putstr_fd("bush", 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(shell->cmd_n_args[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("command not found", 2);
}
