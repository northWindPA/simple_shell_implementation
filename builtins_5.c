/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 21:08:15 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:54:52 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
