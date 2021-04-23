/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/18 13:07:57 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:24:41 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		neg_pos_dig(char *cmd_n_args, int pos_neg)
{
	int i;

	i = 0;
	if (pos_neg == 1)
	{
		if (cmd_n_args[0] == '+')
			i++;
		else if (cmd_n_args[0] == '-')
			i++;
	}
	while (cmd_n_args[i])
	{
		if (!ft_isdigit(cmd_n_args[i++]))
			return (0);
	}
	return (1);
}

int		ex_args(char **cmd_n_args)
{
	if (len_2d(cmd_n_args) > 2)
	{
		if (neg_pos_dig(cmd_n_args[1], 1))
		{
			if (neg_pos_dig(cmd_n_args[1], 0))
			{
				g_exit = 1;
				if (neg_pos_dig(cmd_n_args[1], 0))
					write(2, "exit\n", 5);
				write(2, PROMT, 19);
				write(2, "exit: ", 6);
				write(2, "too many arguments", 18);
				write(2, "\n", 1);
				return (1);
			}
		}
	}
	return (0);
}

void	mini_exit(char **cmd_n_args)
{
	int	rax;

	rax = 0;
	if (ex_args(cmd_n_args))
		return ;
	if (cmd_n_args[1])
	{
		if (neg_pos_dig(cmd_n_args[1], 1))
			rax = ft_atoi(cmd_n_args[1]);
		else if (!neg_pos_dig(cmd_n_args[1], 1))
			err_num(rax, cmd_n_args);
	}
	write(1, "exit\n", 5);
	if (rax < 0)
		exit(rax);
	if (rax > 255)
		rax = 255;
	free_maker(cmd_n_args);
	exit(rax);
}

void	mini_echo(char **cmd_n_args)
{
	int i;

	i = 1;
	if (len_2d(cmd_n_args) == 1)
	{
		write(1, "\n", 1);
		return ;
	}
	if (!ft_strcmp(cmd_n_args[1], "-n"))
		i = 2;
	while (cmd_n_args[i])
	{
		write(1, cmd_n_args[i], ft_strlen(cmd_n_args[i]));
		if (cmd_n_args[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (ft_strcmp(cmd_n_args[1], "-n"))
		write(1, "\n", 1);
	g_exit = 0;
}
