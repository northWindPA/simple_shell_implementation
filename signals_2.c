/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:47:56 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:48:44 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_crtl_c(int sg)
{
	if (sg == SIGINT)
	{
		g_exit = 1;
		write(1, "\n", 1);
		ft_putstr_fd(PROMT, 1);
		signal(SIGINT, cmd_crtl_c);
	}
}

void	disp_cmd_crtl_c(int sg)
{
	(void)sg;
	write(1, "\n", 1);
	g_exit = 130;
	signal(SIGINT, disp_cmd_crtl_c);
}

void	cmd_ctrl_slash(int fr)
{
	if (fr == SIGQUIT)
	{
		signal(SIGQUIT, cmd_ctrl_slash);
	}
}
