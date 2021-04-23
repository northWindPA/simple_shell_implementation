/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 21:08:01 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 21:11:13 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	mini_unset(t_shell *shell)
{
	t_kv	*tmp;
	t_kv	*sacrifice;
	int		i;

	i = 0;
	tmp = shell->kv;
	while (shell->cmd_n_args[++i])
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
			if (tmp->next == NULL)
				break ;
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

void	err_num(int rax, char **cmd_n_args)
{
	rax = 255;
	write(1, "exit\n", 5);
	write(2, PROMT, ft_strlen(PROMT));
	write(2, ": ", 2);
	write(2, cmd_n_args[0], ft_strlen(cmd_n_args[0]));
	write(2, ": ", 2);
	write(2, cmd_n_args[1], ft_strlen(cmd_n_args[1]));
	write(2, ": ", 2);
	write(2, "numeric argument required", 25);
	write(2, "\n", 1);
	exit(rax);
}
