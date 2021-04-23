/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_three.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:45:29 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/23 22:45:37 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_maker(char **str)
{
	int i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void	errcode(t_shell *sh, int i, int errcode)
{
	sh->err[0] = i;
	sh->err[1] = errcode;
}

void	print_error(const char *error_msg, t_shell *sh)
{
	g_exit = 258;
	write(2, "\n", 1);
	write(2, error_msg, ft_strlen(error_msg));
	write(2, "\n", 1);
	errcode(sh, 1, g_exit);
}

void	print_cmd_error(const char *error_msg, t_shell *sh)
{
	g_exit = 127;
	write(2, "\nbush: ", 7);
	write(2, error_msg, ft_strlen(error_msg));
	write(2, ": command not found\n", 20);
	errcode(sh, 1, g_exit);
}

void	do_exit(char *msg)
{
	write(1, "\n", 1);
	write(1, msg, ft_strlen(msg));
	write(1, "\n", 1);
	exit(0);
}
