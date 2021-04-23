/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:50:03 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:50:39 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		len_of_2d_array(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->cmd_n_args[i])
		i++;
	return (i);
}

void	init(t_shell *shell)
{
	if (!(shell->ind_arg =
	(int *)malloc(sizeof(int) * len_of_2d_array(shell))))
		exit(12);
	shell->pps = 0;
	shell->p_in = NULL;
	shell->p_out = NULL;
	shell->fl_rdr_in = 0;
	shell->fl_rdr_out = 0;
	shell->fl_rdr_app = 0;
}

void	num_of_rdrs(t_shell *shell)
{
	int i;

	i = 0;
	shell->rdr_i = 0;
	while (shell->cmd_n_args[i])
	{
		if (shell->fl_arg[i] &&
		(chk_tkn(">", shell->cmd_n_args[i])
		|| chk_tkn(">>", shell->cmd_n_args[i])))
			shell->rdr_i++;
		i++;
	}
}
