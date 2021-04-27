/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 13:16:46 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/27 21:08:10 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_strslen(char **line)
{
	unsigned char	**s;
	int				i;

	i = 0;
	s = (unsigned char **)line;
	while (s[i])
		i++;
	return (i);
}

int		remove_backslash(char *line, int i)
{
	char	*tmp;
	int		j;

	j = i;
	tmp = line;
	while (line[i])
	{
		line[i] = tmp[i + 1];
		i++;
	}
	return (j);
}

int		num_of_chars(char *s, int c)
{
	int i;
	int	count;

	count = 0;
	i = 0;
	if (s[i] == c)
		i++;
	while (s[i])
	{
		if (s[i] == c)
			count++;
		if (s[i] == '\0')
			return (0);
		i++;
	}
	return (count);
}

void	qfl(char *line, int i, t_shell *sh)
{
	if (line[i] == '"' || line[i] == '\'')
		sh->f *= -1;
}

int		skipspaces(char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' ||
		s[i] == '\v' || s[i] == '\f' || s[i] == '\n')
		i++;
	return (i);
}
