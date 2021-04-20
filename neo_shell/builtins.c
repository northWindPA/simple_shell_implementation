/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 13:07:57 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/20 09:46:05 by mhumfrey         ###   ########.fr       */
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
	if ((*home = ft_strdup(find_key_value("HOME", &shell->kv))) == NULL)
		no_home(shell);
	return (0);
}

void	mini_cd(t_shell *shell)
{
	char	*home;

	home = NULL;
	if (!shell->cmd_n_args[1])
	{
		if (!check_path(shell, &home))
			return ;
	}
	// else
	chdir(shell->cmd_n_args[1]);
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

void	export_value(t_shell *shell, int *i, int *j)
{
	(*j)++;
	while(shell->cmd_n_args[*i][*j]
	&& ft_isalnum(shell->cmd_n_args[*i][*j]))
	{
		shell->exp_val = ft_chrjoin_free
		(shell->exp_val, shell->cmd_n_args[*i][*j], 1);
		(*j)++;
	}
	ft_kvadd_back(&shell->kv, ft_lstkv(shell->exp_key, shell->exp_val));
}

void	export_add_2(t_shell *shell)
{
	free(shell->exp_key);
	free(shell->exp_val);
	ft_putendl_fd("bush: export: `=': not a valid identifier", 1);
}

void	export_add(t_shell *shell)
{
	int		i;
	int		j;

	i = 0;
	while(shell->cmd_n_args[++i])
	{
		j = -1;
		shell->exp_key = ft_strdup("");
		shell->exp_val = ft_strdup("");
		while(shell->cmd_n_args[i][++j] && ft_isalpha(shell->cmd_n_args[i][j]))
		{
			shell->exp_key = ft_chrjoin_free
			(shell->exp_key, shell->cmd_n_args[i][j], 1);
		}
		if (shell->cmd_n_args[i][j] == '=')
			export_value(shell, &i, &j);
		else if (ft_strchr(&shell->cmd_n_args[i][j], '='))
		{
			export_add_2(shell);
			break ;
		}
		else
			export_value(shell, &i, &j);
	}
}

void	mini_export(t_shell *shell)
{
	if (len_of_2d_array(shell) == 1)
		display_export(shell);
	else
		export_add(shell);
}

void	mini_env(t_shell *shell)
{
	t_kv *tmp;

	tmp = shell->kv;
	while (tmp)
	{
		ft_putstr_fd(tmp->key, 1);
		ft_putstr_fd("=", 1);
		ft_putendl_fd(tmp->value, 1);
		tmp = tmp->next;
	}
}

void	mini_unset_2(t_shell *shell, t_kv **sacrifice, t_kv **tmp, int i)
{
	if (!ft_strcmp(shell->cmd_n_args[i], (*tmp)->key))
	{
		*sacrifice = *tmp;
		*tmp = (*tmp)->next;
		free((*sacrifice)->key);
		free((*sacrifice)->value);
		free((*sacrifice));
		shell->kv_head = *tmp;
		shell->kv = *tmp;
	}
}

void	mini_unset(t_shell *shell)
{
	t_kv	*tmp;
	t_kv	*sacrifice;
	int		i;

	i = 0;
	tmp = shell->kv;
	while(shell->cmd_n_args[++i])
	{
		tmp = shell->kv;
		mini_unset_2(shell, &sacrifice, &tmp, i);
		while (tmp->next)
		{
			if (!ft_strcmp(shell->cmd_n_args[i], tmp->next->key))
			{
				sacrifice = tmp->next;
				tmp->next = sacrifice->next;
				free(sacrifice->key);
				free(sacrifice->value);
				free(sacrifice);
			}
			tmp = tmp->next;
		}
	}
}

void	mini_pwd(t_shell *shell)
{
	char	*pwd;
	char	*env_pwd_value;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		env_pwd_value = ft_strdup(find_key_value("PWD", &shell->kv));
		if (env_pwd_value)
			ft_putendl_fd(env_pwd_value, 1);
	}
	else
		ft_putendl_fd(pwd, 1);
	if (pwd)
		free(pwd);
}


void	mini_echo(char **cmd_n_args)
{
	int i;

	i = 1;
	while(cmd_n_args[i])
	{
		write(1, cmd_n_args[i], ft_strlen(cmd_n_args[i]));
		if (cmd_n_args[i + 1])
			write(1, " ", 1);
		i++;
		write(1, "\n", 1);
	}
	g_exit = 0;
}
