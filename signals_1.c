/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 16:26:24 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:48:50 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	disp_cmd_ctrl_slash(int fr)
{
	(void)fr;
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
