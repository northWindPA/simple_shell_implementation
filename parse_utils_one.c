/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_one.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:43:18 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/27 21:16:18 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	switch_space(t_shell *sh, int one, int two, int three)
{
	sh->ff[0] = one;
	sh->ff[1] = two;
	sh->ff[2] = three;
}

int		join_if_arg(t_shell *sh, char *str)
{
	sh->fl_arg[sh->cntr] = 0;
	if (sh->ff[0] == -1 && sh->cntr > 1 && sh->fl_arg[sh->cntr - 1] == 0)
	{
		sh->cntr--;
		sh->cmd_n_args[sh->cntr] =
		ft_strjoin_free(sh->cmd_n_args[sh->cntr], str, 1);
		return (1);
	}
	switch_space(sh, -1, -1, 0);
	return (0);
}

void	save_str_to_structure(t_shell *sh, char *str)
{
	int		i;
	char	**old;

	sh->cntr++;
	old = sh->cmd_n_args;
	if (!sh->ff[2] && (!ft_strcmp(str, ">") || !ft_strcmp(str, "<")
	|| !ft_strcmp(str, ">>") || !ft_strcmp(str, "|")))
		sh->fl_arg[sh->cntr] = 1;
	else
	{
		if (join_if_arg(sh, str))
			return ;
	}
	sh->ff[2] = 0;
	if (!(sh->cmd_n_args = (char **)malloc(sizeof(char *) * (sh->cntr + 2))))
		print_error("bush: cannot allocate memory", sh);
	i = -1;
	while (++i < sh->cntr)
		sh->cmd_n_args[i] = ft_strdup(old[i]);
	sh->cmd_n_args[sh->cntr] = ft_strdup(str);
	sh->cmd_n_args[sh->cntr + 1] = NULL;
	if (sh->cntr == 0)
		return ;
	free_maker(old);
}

void	save_str_or_oldstr(t_shell *sh, char *str, char *oldstr)
{
	if (sh->isfound)
		save_str_to_structure(sh, str);
	else
		save_str_to_structure(sh, oldstr);
	free(oldstr);
	free(str);
	sh->isfound = 0;
}

char	*replace_env_n_value(t_shell *sh,
char *str, char *env_key, char *env_value)
{
	char	*tmp1;
	char	*tmp2;
	int		i;

	i = 0;
	while (str[i] && str[i] != '$')
		i++;
	if (!(tmp1 = ft_substr(str, 0, i)))
		print_error("bush: cannot allocate memory", sh);
	if (!(tmp1 = ft_strjoin_free(tmp1, env_value, 1)))
		print_error("bush: cannot allocate memory", sh);
	if (!(tmp2 = ft_substr_free(
		str, i + ft_strlen(env_key) + 1, ft_strlen(str))))
		print_error("bush: cannot allocate memory", sh);
	if (!(tmp1 = ft_strjoin_free(tmp1, tmp2, 1)))
		print_error("bush: cannot allocate memory", sh);
	free(tmp2);
	return (tmp1);
}
