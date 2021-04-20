/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 16:26:24 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/19 23:44:43 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cmd_crtl_c(int sg)
{
	if (sg == SIGINT)
	{
		g_exit = 1;
		write(1, "\b\b  \b\b", 6);
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
		write(1, "\b\b  \b\b", 6);
		signal(SIGQUIT, cmd_ctrl_slash);
	}
}

void	disp_cmd_ctrl_slash(int fr)
{
	(void)fr;
	write(1, "\b\b  \b\b", 6);
	write(1, "^\\Quit: 3\n", 10);
	g_exit = 131;
	signal(SIGQUIT, disp_cmd_ctrl_slash);
}

void	signals_start(int cat_time)
{
	if (cat_time == 1)
	{
		signal(SIGINT, cmd_crtl_c);
		signal(SIGQUIT, cmd_ctrl_slash);
	}
	if (cat_time == 2)
	{
		signal(SIGINT, disp_cmd_crtl_c);
		signal(SIGQUIT, disp_cmd_ctrl_slash);
	}
}
