/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 21:08:06 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 21:09:22 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_check_pwd(t_shell *shell, char **tmp, char **home)
{
	if (!ft_strcmp("PWD", shell->kv->key))
	{
		*tmp = ft_strdup(shell->kv->value);
		if (len_2d(shell->cmd_n_args) == 1
		|| !ft_strcmp("~", shell->cmd_n_args[1]))
		{
			if ((chdir(*home)) == -1)
				err_no_dir_cd(*home);
		}
		else
		{
			if ((chdir(shell->cmd_n_args[1])) == -1)
				err_no_dir_cd(shell->cmd_n_args[1]);
		}
		free(shell->kv->value);
		shell->kv->value = getcwd(NULL, 0);
	}
	else if (!ft_strcmp("OLDPWD", shell->kv->key))
	{
		free(shell->kv->value);
		shell->kv->value = ft_strdup(*tmp);
	}
}

void	mini_cd(t_shell *shell)
{
	char	*home;
	char	*tmp;

	if (!check_path(shell, &home))
		return ;
	shell->kv = shell->kv_head;
	while (shell->kv)
	{
		cd_check_pwd(shell, &tmp, &home);
		shell->kv = shell->kv->next;
	}
	free(tmp);
	free(home);
}

void	display_export(t_shell *shell)
{
	t_kv *tmp;

	tmp = shell->kv;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", 1);
		if (!ft_strcmp(tmp->value, ""))
			ft_putendl_fd(tmp->key, 1);
		else
		{
			ft_putstr_fd(tmp->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd("\"", 1);
			ft_putstr_fd(tmp->value, 1);
			ft_putendl_fd("\"", 1);
		}
		tmp = tmp->next;
	}
}
