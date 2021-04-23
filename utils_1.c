/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 15:46:27 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:50:51 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pps_free(t_shell *shell)
{
	if (shell->ind_arg)
		free(shell->ind_arg);
	if (shell->p_out)
	{
		free(shell->p_out);
		shell->p_out = NULL;
	}
	if (shell->p_in)
	{
		free(shell->p_in);
		shell->p_in = NULL;
	}
}
