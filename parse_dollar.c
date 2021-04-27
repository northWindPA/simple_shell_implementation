/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:41:00 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/27 21:09:04 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*dollar_one(t_shell *sh, char *str, char *oldstr, int j)
{
	if (ft_isdigit(str[j + 1]))
	{
		oldstr = ft_strjoin_free(
			oldstr, ft_substr(str, j + 2, ft_strlen(str)), 3);
		sh->isfound = 0;
	}
	else if (str[j + 1] == '?')
	{
		oldstr = ft_strjoin_free(
			oldstr, ft_itoa(sh->err[1]), 3);
		oldstr = ft_strjoin_free(
			oldstr, ft_substr(str, j + 2, ft_strlen(str)), 3);
		sh->isfound = 0;
	}
	return (oldstr);
}

char	*dollar_two(t_shell *sh, char *str, int *j)
{
	char	*env_key;

	env_key = ft_strdup("");
	while (str[++(*j)] && (ft_isalnum(str[*j]) || str[*j] == '_'))
		env_key = ft_chrjoin_free(env_key, str[*j], 1);
	while (sh->kv)
	{
		if (!ft_strcmp(env_key, sh->kv->key))
		{
			str = replace_env_n_value(sh, str, sh->kv->key, sh->kv->value);
			sh->isfound = 1;
			break ;
		}
		sh->kv = sh->kv->next;
	}
	free(env_key);
	return (str);
}

int		parse_if_dollar(t_shell *sh, char *line, int i)
{
	char	*str;
	char	*oldstr;
	int		j;

	j = 0;
	str = ft_strdup("");
	oldstr = ft_strdup("");
	while (line[i] && !ft_isspace(line[i]))
	{
		if (line[i + 1] == '$' || line[i + 1] == '"' || line[i + 1] == '\'')
		{
			str = ft_chrjoin_free(str, line[i++], 1);
			break ;
		}
		str = ft_chrjoin_free(str, line[i], 1);
		i++;
	}
	if (line[i] == ' ')
		switch_space(sh, 1, 0, 0);
	if (str[j] == '$' && (ft_isdigit(str[j + 1]) || str[j + 1] == '?'))
		oldstr = dollar_one(sh, str, oldstr, j);
	else if (str[j] == '$' && (ft_isalnum(str[j + 1]) || str[j + 1] == '_'))
		str = dollar_two(sh, str, &j);
	save_str_or_oldstr(sh, str, oldstr);
	return (--i);
}
