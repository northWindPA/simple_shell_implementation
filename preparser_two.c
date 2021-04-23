/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparser_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:56:05 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/23 22:56:15 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_semicolons2(char *line, t_shell *sh, int *i)
{
	int h;

	while (line[*i])
	{
		qfl(line, *i, sh);
		if (line[*i] == ';' && line[*i - 1] == '\\')
			(*i)++;
		if (line[*i] == ';' && sh->f == -1)
		{
			h = skipspaces(line += *i + 1);
			if (line[h] == ';')
				print_error("bush: syntax error near unexpected token `;'", sh);
		}
		if (line[*i] == ';' && line[0] == ';' &&
		line[1] != ';' && *i == 0 && sh->f == -1)
			print_error("bush: syntax error near unexpected token `;'", sh);
		else if (line[*i] == ';' && line[*i + 1] == ';'
		&& line[*i + 2] != ';' && sh->f == -1)
			print_error("bush: syntax error near unexpected token `;;'", sh);
		(*i)++;
	}
}

void	check_semicolons(char *line, t_shell *sh)
{
	int i;

	sh->f = -1;
	i = skipspaces(line);
	qfl(line, i + 1, sh);
	if (line[i] == ';' && line[i + 1] != '\0' &&
	line[i + 1] != ';' && line[0] != ';' && sh->f == -1)
		print_error("bush: syntax error near unexpected token `;'", sh);
	else if (line[i] == ';' && line[i + 1] == '\0'
	&& line[0] != ';' && sh->f == -1)
		print_error("bush: syntax error near unexpected token `;'", sh);
	while (ft_isspace(line[i]))
	{
		qfl(line, i, sh);
		if (line[i] == ';' && line[i - 1] == '\\')
			i++;
		if (line[i + 1] == ';' && sh->f == -1)
			print_error("bush: syntax error near unexpected token `;'", sh);
		else if (line[i + 1] == ';' && line[i + 2] == ';' && sh->f == -1)
			print_error("bush: syntax error near unexpected token `;;'", sh);
		i++;
	}
	check_semicolons2(line, sh, &i);
}

void	check_pipes2(char *line, t_shell *sh, int *i)
{
	if (line[*i] == '|' && line[*i + 1] == '|' && sh->f++ == 0)
	{
		print_error("bush: syntax error near unexpected token `||'", sh);
		sh->pips = 2;
	}
	else if (line[*i] == '|' && line[*i + 1] == ' ')
	{
		while (ft_isspace(line[++(*i)]))
			;
		if (line[*i] == '\0' || line[*i] == ';')
			print_error("bush: syntax error near unexpected token `|'", sh);
		sh->pips = 2;
	}
}

void	check_pipes(char *line, t_shell *sh)
{
	int i;

	sh->f = 0;
	i = skipspaces(line);
	if (line[i] == '|' && line[i + 1] != '\0'
	&& line[i + 1] != '|' && line[0] != '|')
		print_error("bush: syntax error near unexpected token `|'", sh);
	while (line[i] && sh->f == 0)
	{
		check_pipes2(line, sh, &i);
		i++;
	}
	if (sh->pips != 2 && (line[0] == '|' || line[ft_strlen(line) - 1] == '|'))
		print_error("bush: syntax error near unexpected token `|'", sh);
	sh->pips = 1;
}
