/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 19:03:50 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:37:25 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit = 0;

int		main(int argc, char **argv, char **env)
{
	t_shell shell;

	(void)argv;
	if (argc != 1)
		return (-1);
	ft_bzero(shell.fl_arg, 1024);
	initialize(&shell);
	get_env(ft_shell_lvl(env), &shell);
	setup_term(&shell);
	term_func(&shell);
	return (0);
}
