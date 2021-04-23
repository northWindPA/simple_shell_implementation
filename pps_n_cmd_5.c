/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pps_n_cmd_6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:42:27 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:43:07 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pps_fd(t_shell *shell, int i)
{
	if (!i)
		pps_fd_1(shell);
	else if (i < shell->pps)
		pps_fd_2(shell);
	else
		pps_fd_3(shell);
}

int		rdr_app(t_shell *shell)
{
	shell->out_fd = open(shell->p_out, O_CREAT | O_WRONLY | O_APPEND, 0777);
	if (shell->out_fd < 0)
		return (1);
	close(1);
	dup(shell->out_fd);
	close(shell->out_fd);
	return (0);
}

int		rdr_out(t_shell *shell)
{
	shell->out_fd = open(shell->p_out, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (shell->out_fd < 0)
		return (1);
	close(1);
	dup(shell->out_fd);
	close(shell->out_fd);
	return (0);
}
