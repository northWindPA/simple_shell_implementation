/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 19:03:59 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/20 06:54:02 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	manage_order(t_shell *shell, int i)
{
	int		j;
	char	*tmp;

	j = 0;
	while (shell->cmd_n_args[j])
	{
		if (chk_tkn(">", shell->cmd_n_args[j]))
			break;
		j++;
	}
	tmp = shell->cmd_n_args[i];
	while (i > j)
	{
		shell->cmd_n_args[i] = shell->cmd_n_args[i - 1];
		i--;
	}
	shell->cmd_n_args[i] = tmp;
	shell->fl_arg[j] = 0;
	shell->fl_arg[j + 1] = 1;
}

int 	new_ord(t_shell *shell)
{
	int i;

	i = 0;
	while (i < shell->num_args)
	{
		if (chk_tkn(">", shell->cmd_n_args[i]) && shell->cmd_n_args[i + 2])
		{
			if (!chk_tkn(">", shell->cmd_n_args[i + 2]) &&
			!chk_tkn(">>", shell->cmd_n_args[i + 2]) &&
			!chk_tkn("<", shell->cmd_n_args[i + 2]) &&
			!chk_tkn("|", shell->cmd_n_args[i + 2]))
				manage_order(shell, i + 2);
			i++;
		}
		i++;
	}
	return (0);
}

int 	old_ord(t_shell *shell)
{
	int i;

	i = 0;
	while (i < shell->num_args)
	{
		if (((chk_tkn(">", shell->cmd_n_args[i]) && shell->fl_arg[i])
		|| (chk_tkn(">>", shell->cmd_n_args[i]) && shell->fl_arg[i]))
		&& (i + 2 < shell->num_args))
		{
			if (((!chk_tkn(">", shell->cmd_n_args[i + 2])
			&& !shell->fl_arg[i + 2]) &&
			(!chk_tkn(">>", shell->cmd_n_args[i + 2])
			&& !shell->fl_arg[i + 2]) &&
			(!chk_tkn("<", shell->cmd_n_args[i + 2])
			&& !shell->fl_arg[i + 2]) &&
			(!chk_tkn("|", shell->cmd_n_args[i + 2])
			&& !shell->fl_arg[i + 2])) && !shell->cmd_n_args[i + 2])
				return (1);
			return (0);
		}
		i++;
	}
	return ((i == shell->num_args - 1) ? 0 : 1);
}

int		tokens(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->cmd_n_args[i])
	{
		if (shell->fl_arg[i] &&
		ft_strlen(shell->cmd_n_args[i]) &&
		(!ft_strcmp(">",shell->cmd_n_args[i])
		|| !ft_strcmp("<",shell->cmd_n_args[i])
		|| !ft_strcmp("|",shell->cmd_n_args[i])
		|| !ft_strcmp(">>",shell->cmd_n_args[i])))
		{
			pps_exc(shell);
			return (1);
		}
		i++;
	}
	return (0);
}

void	builtins(t_shell * shell)
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
	// else if (!ft_strcmp("exit", shell->cmd_n_args[0]))
	// 	ft_exit(shell->cmd_n_args);
}

void 	exec(t_shell *shell)
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
