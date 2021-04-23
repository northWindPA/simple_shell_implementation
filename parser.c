/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:34:02 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/23 23:00:14 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	processor(t_shell *sh, char *line)
{
	int j;

	j = 0;
	sh->cntr = -1;
	while (line[j])
	{
		if (line[j] == '$')
			j = parse_if_dollar(sh, line, j);
		else if (line[j] == '"')
			j = parse_if_double_quotes(sh, line, ++j);
		else if (line[j] == '\'')
			j = parse_if_single_quotes(sh, line, ++j);
		else if (line[j])
			j = parse_if_all_the_rest(sh, line, j);
		if (line[j] == '\0')
			break ;
		j++;
	}
}

void	history_record_and_call_parser(t_shell *sh, int pos)
{
	if (!ft_strncmp(sh->buf, "\n", 1))
	{
		sh->buf[0] = 0;
		if (sh->hist[pos][0] != 0)
			join_buf(sh->hist, sh->hist[pos], sh);
		else
			write(1, "\n", 1);
	}
	if (sh->hist[pos][0] != 0)
	{
		preparser(sh);
		if (sh->err[0] == 0)
			parser(sh);
		errcode(sh, 0, g_exit);
		sh->cc = 0;
	}
	free(sh->parsed_buf);
	free_maker(sh->hist);
}

void	parse_and_exec(t_shell *sh, char *line)
{
	sh->kv = sh->kv_head;
	processor(sh, line);
	term_backup(1);
	exec(sh);
	term_backup(0);
	free_maker(sh->cmd_n_args);
}

void	parser(t_shell *sh)
{
	int		i;
	char	*line;

	i = -1;
	sh->f = -1;
	while (sh->parsed_buf[++i])
	{
		line = ft_strdup("");
		while (sh->parsed_buf[i])
		{
			qfl(sh->parsed_buf, i, sh);
			if (sh->parsed_buf[i] == ';' && sh->f == -1
			&& sh->parsed_buf[i - 1] != '\\')
				break ;
			line = ft_chrjoin_free(line, sh->parsed_buf[i], 1);
			i++;
		}
		parse_and_exec(sh, line);
		if (sh->parsed_buf[i] == '\0')
		{
			free(line);
			break ;
		}
		free(line);
	}
}
