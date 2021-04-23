/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pps_n_cmd_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:40:16 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:41:34 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		chk_pps_tkns_2(t_shell *shell, int i)
{
	shell->ind_arg[shell->pps + 1] = i + 1;
	free(shell->cmd_n_args[i]);
	shell->cmd_n_args[i] = 0;
	(shell->pps)++;
	return (i);
}

int		chk_pps_tkns(t_shell *shell, int i)
{
	if (!ft_strcmp(shell->cmd_n_args[i], "|"))
		i = chk_pps_tkns_2(shell, i);
	else if (!ft_strcmp(shell->cmd_n_args[i], ">")
	|| !ft_strcmp(shell->cmd_n_args[i], ">>")
	|| !ft_strcmp(shell->cmd_n_args[i], "<"))
		if (!fl_chk_rdr(shell, i))
			return (0);
	return (1);
}

int		flag_chk(t_shell *shell)
{
	int i;

	i = 0;
	while (shell->cmd_n_args[i])
	{
		if (shell->fl_arg[i])
		{
			if (!chk_pps_tkns(shell, i))
				return (0);
		}
		else
			shell->ind_arg[i] = i;
		i++;
	}
	return (1);
}
