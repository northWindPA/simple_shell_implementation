/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:37:08 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/27 21:09:46 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_new_str(t_shell *sh, char *line, int *i)
{
	char	*str;
	int		j;

	str = ft_strdup("");
	while (line[*i])
	{
		if (line[*i] == '"' && line[*i - 1] == '\\')
			;
		else if (line[*i] == '"')
			break ;
		str = ft_chrjoin_free(str, line[*i], 1);
		(*i)++;
	}
	if (line[*i + 1] == ' ' && sh->ff[0] == 1)
		switch_space(sh, 1, 1, 1);
	j = 0;
	while (str[j])
	{
		if (str[j] == '\\' && (str[j + 1] == '"' || str[j + 1] == '$'))
			j = remove_backslash(str, j);
		j++;
	}
	return (str);
}

int		parse_if_double_quotes(t_shell *sh, char *line, int i)
{
	char	*str;
	char	*oldstr;
	int		ln;
	int		j;

	str = make_new_str(sh, line, &i);
	oldstr = ft_strdup("");
	j = 0;
	ln = 0;
	while (str[j])
	{
		while (str[ln] && str[ln] != '$')
		{
			oldstr = ft_chrjoin_free(oldstr, str[ln], 1);
			ln++;
		}
		if (str[j] == '$' && (ft_isdigit(str[j + 1]) || str[j + 1] == '?'))
			oldstr = dollar_one(sh, str, oldstr, j);
		else if (str[j] == '$' && (ft_isalnum(str[j + 1]) || str[j + 1] == '_'))
			str = dollar_two(sh, str, &j);
		j++;
	}
	save_str_or_oldstr(sh, str, oldstr);
	return (i);
}

int		parse_if_single_quotes(t_shell *sh, char *line, int i)
{
	char	*str;

	str = ft_strdup("");
	while (line[i] && line[i] != '\'')
		str = ft_chrjoin_free(str, line[i++], 1);
	if (line[i + 1] == ' ' && sh->ff[0] == 1)
		switch_space(sh, 1, 1, 1);
	save_str_to_structure(sh, str);
	free(str);
	return (i);
}
