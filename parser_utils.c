/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 13:16:46 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/20 13:35:21 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize(t_shell *sh)
{
	sh->isfound = 0;
	sh->err[0] = 0;
	sh->err[1] = 0;
	sh->sq = -1;
	sh->dq = -1;
	sh->f = 0;
	sh->ff = -1;
	sh->cc = 0;
	sh->cntr = -1;
	sh->pips = 0;
	sh->env_n_l = 0;
	sh->kv = NULL;
	sh->env = NULL;
	sh->dup_env = NULL;
	sh->list = NULL;
	sh->ls_hist = NULL;
}

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

void    print_error(const char *error_msg, t_shell *sh)
{
    g_exit = 258;
    write(2, "\n", 1);
    write(2, error_msg, ft_strlen(error_msg));
    write(2, "\n", 1);
	errcode(sh, 1, g_exit);
}

void    print_cmd_error(const char *error_msg, t_shell *sh)
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
