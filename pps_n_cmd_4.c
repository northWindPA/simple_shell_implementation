/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pps_n_cmd_5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:42:29 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:42:53 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pps_fd_1(t_shell *shell)
{
	close(1);
	dup(shell->pp_r[1]);
	close(shell->pp_r[0]);
	close(shell->pp_r[1]);
}

void	pps_fd_2(t_shell *shell)
{
	close(0);
	dup(shell->pp_l[0]);
	close(shell->pp_l[0]);
	close(shell->pp_l[1]);
	close(1);
	dup(shell->pp_r[1]);
	close(shell->pp_r[0]);
	close(shell->pp_r[1]);
}

void	pps_fd_3(t_shell *shell)
{
	close(0);
	dup(shell->pp_l[0]);
	close(shell->pp_l[0]);
	close(shell->pp_l[1]);
}
