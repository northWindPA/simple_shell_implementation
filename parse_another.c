/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_another.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:39:34 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/23 22:40:18 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		break_chr(char c)
{
	if (c == ' ' || c == ';' || c == '\'' || c == '\"'
	|| c == '$' || c == '|' || c == '>' || c == '<' || c == '\\')
		return (1);
	return (0);
}

char	*save_redirects(t_shell *sh, char *line, char *str, int *i)
{
	if (line[*i] == '>' && line[*i + 1] != '>' && line[*i + 1] != '<')
	{
		str = ft_strdup_free(str, ">", 1);
		save_str_to_structure(sh, str);
	}
	else if (line[*i] == '<' && line[*i + 1] != '>' && line[*i + 1] != '<')
	{
		str = ft_strdup_free(str, "<", 1);
		save_str_to_structure(sh, str);
	}
	else if (line[*i] == '>' && line[*i + 1] == '>'
	&& line[*i + 2] != '<' && line[*i + 2] != '>')
	{
		str = ft_strdup_free(str, ">>", 1);
		save_str_to_structure(sh, str);
		(*i)++;
	}
	return (str);
}

char	*save_pipe_or_isalnum(t_shell *sh, char *line, char *str, int *i)
{
	if (line[*i] == '|')
	{
		str = ft_strdup_free(str, "|", 1);
		save_str_to_structure(sh, str);
	}
	else if (line[*i] == '\\')
	{
		if (line[*i + 1] != '\0')
			str = ft_strdup_free(str, ft_substr(line, *i + 1, 1), 3);
		save_str_to_structure(sh, str);
		(*i)++;
	}
	return (str);
}

int		parse_if_all_the_rest(t_shell *sh, char *line, int i)
{
	char	*str;

	str = ft_strdup("");
	while (line[i] && !break_chr(line[i]))
	{
		if (break_chr(line[i + 1]))
		{
			str = ft_chrjoin_free(str, line[i], 1);
			break ;
		}
		str = ft_chrjoin_free(str, line[i], 1);
		i++;
	}
	if (line[i] == ' ')
		switch_space(sh, 1, 0);
	if (!break_chr(line[i]))
		save_str_to_structure(sh, str);
	str = save_redirects(sh, line, str, &i);
	str = save_pipe_or_isalnum(sh, line, str, &i);
	free(str);
	return (i);
}
