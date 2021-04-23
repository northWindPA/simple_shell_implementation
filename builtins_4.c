/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 21:08:10 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 21:10:08 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	export_value(t_shell *shell, int *i, int *j)
{
	(*j)++;
	while (shell->cmd_n_args[*i][*j]
	&& ft_isalnum(shell->cmd_n_args[*i][*j]))
	{
		shell->exp_val =
		ft_chrjoin_free(shell->exp_val, shell->cmd_n_args[*i][*j], 1);
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
	while (shell->cmd_n_args[++i])
	{
		j = -1;
		shell->exp_key = ft_strdup("");
		shell->exp_val = ft_strdup("");
		while (shell->cmd_n_args[i][++j] && ft_isalpha(shell->cmd_n_args[i][j]))
		{
			shell->exp_key =
			ft_chrjoin_free(shell->exp_key, shell->cmd_n_args[i][j], 1);
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
