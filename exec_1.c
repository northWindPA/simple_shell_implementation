/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 19:03:59 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:26:57 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		tokens(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->cmd_n_args[i])
	{
		if (shell->fl_arg[i] &&
		ft_strlen(shell->cmd_n_args[i]) &&
		(!ft_strcmp(">", shell->cmd_n_args[i])
		|| !ft_strcmp("<", shell->cmd_n_args[i])
		|| !ft_strcmp("|", shell->cmd_n_args[i])
		|| !ft_strcmp(">>", shell->cmd_n_args[i])))
		{
			pps_exc(shell);
			return (1);
		}
		i++;
	}
	return (0);
}

void	builtins(t_shell *shell)
{
	if (!ft_strcmp("cd", shell->cmd_n_args[0]))
		mini_cd(shell);
	if (!ft_strcmp("pwd", shell->cmd_n_args[0]))
		mini_pwd(shell);
	if (!ft_strcmp("echo", shell->cmd_n_args[0]))
		mini_echo(shell->cmd_n_args);
	if (!ft_strcmp("env", shell->cmd_n_args[0]))
		mini_env(shell);
	if (!ft_strcmp("export", shell->cmd_n_args[0]))
		mini_export(shell);
	if (!ft_strcmp("unset", shell->cmd_n_args[0]))
		mini_unset(shell);
	if (!ft_strcmp("exit", shell->cmd_n_args[0]))
		mini_exit(shell->cmd_n_args);
}

void	exec(t_shell *shell)
{
	if (shell->cc++ == 0)
		write(1, "\n", 1);
	shell->kv = shell->kv_head;
	shell->list = NULL;
	shell->kv = shell->kv_head;
	shell->num_args = len_of_2d_array(shell);
	if (tokens(shell))
		return ;
	else if (!ft_strcmp("cd", shell->cmd_n_args[0])
	|| !ft_strcmp("pwd", shell->cmd_n_args[0])
	|| !ft_strcmp("echo", shell->cmd_n_args[0])
	|| !ft_strcmp("env", shell->cmd_n_args[0])
	|| !ft_strcmp("export", shell->cmd_n_args[0])
	|| !ft_strcmp("unset", shell->cmd_n_args[0])
	|| !ft_strcmp("exit", shell->cmd_n_args[0]))
		builtins(shell);
	else
		exec_other_bins(shell);
}
