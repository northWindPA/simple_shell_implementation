/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/07 19:03:50 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/20 04:48:43 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit = 0;

char	**ft_shell_lvl(char **env)
{
	int		i;
	int		j;
	int		value;
	char	*new_value;

	i = -1;
	while(env[++i])
	{
		j = 0;
		if (!ft_strncmp(env[i], "SHLVL=", 6))
		{
			while(env[i][j] && env[i][j] != '=')
				j++;
			j++;
			if (env[i][j])
			{
				value = ft_atoi(&env[i][j]);
				value++;
				new_value = ft_itoa(value);
				ft_memcpy(&env[i][j], new_value, ft_strlen(new_value));
				free(new_value);
			}
		}
	}
	return (env);
}

int		main(int argc, char **argv, char **env)
{
	t_shell shell;

	(void)argv;
	if (argc != 1)
		return (-1);
	signal(SIGINT, ft_sigint);
	signal(SIGQUIT, ft_sigquit);
	ft_bzero(shell.fl_arg, 1024);
	initialize(&shell);
	get_env(ft_shell_lvl(env), &shell);
	setup_term(&shell);
	term_func(&shell);
	return (0);
}
