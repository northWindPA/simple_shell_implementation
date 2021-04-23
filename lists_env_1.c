/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_env_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 17:24:08 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:37:19 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_env_list(char **env, t_shell *sh)
{
	int	i;

	i = -1;
	while (env[++i])
		ft_lstadd_back(&sh->list, ft_lstnew(env[i]));
	parse_env(sh);
	ft_lstclear(&sh->list, free);
}

void	get_env(char **env, t_shell *sh)
{
	int		i;

	i = -1;
	while (env[++i])
		;
	if ((sh->dup_env = (char **)malloc(sizeof(char *) * (++i))) == NULL)
		exit(0);
	sh->env_n_l = -1;
	while (env[++sh->env_n_l])
		if ((sh->dup_env[sh->env_n_l] = ft_strdup(env[sh->env_n_l])) == NULL)
			exit(0);
	sh->dup_env[sh->env_n_l] = 0;
	make_env_list(sh->dup_env, sh);
}

char	**ft_shell_lvl(char **env)
{
	int		i;
	int		j;
	int		value;
	char	*new_value;

	i = -1;
	while (env[++i])
	{
		j = 0;
		if (!ft_strncmp(env[i], "SHLVL=", 6))
		{
			while (env[i][j] && env[i][j] != '=')
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
