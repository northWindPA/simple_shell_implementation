/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 12:59:14 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/20 17:17:26 by keuclide         ###   ########.fr       */
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

void	check_redirects(char *line, t_shell *sh)
{
	int i;

	sh->f = 0;
	if ((line[ft_strlen(line) - 1] == '>' ||
	line[ft_strlen(line) - 1] == '<') && sh->f++ == 0)
		print_error("bush: syntax error near unexpected token `newline'", sh);
	i = skipspaces(line);
	while (line[i] && sh->f == 0)
	{
		if ((line[i] == '>' || line[i] == '<')
		&& line[i + 1] == '<' && line[i + 1] == '>'
		&& line[i + 2] != '<' && line[i + 2] != '>')
		{
			if (!ft_strncmp(line + i, "<", 1) || !ft_strncmp(line + i, ">", 1))
				i++;
			else if (!ft_strncmp(line + i, ">>", 2))
				i += 2;
			while (ft_isspace(line[i]))
				i++;
			if (line[i] == '\0' || line[i] == '>' || line[i] == '<')
				print_error(
				"bush: syntax error near unexpected token `newline'", sh);
		}
		i++;
	}
}

void	check_quotes(char *line, t_shell *sh)
{
	int i;

	i = skipspaces(line);
	if ((line[i] == '"' || line[i] == '\'') && sh->dq == -1 && sh->sq == -1 &&
	(num_of_chars(line + i, '"') || num_of_chars(line + i, '\'')))
		print_cmd_error("", sh);
	while (line[i])
	{
		if (line[i] == '"' && sh->dq == 1)
		{
			print_error("bush: missing closing quote `\"'", sh);
			sh->dq *= -1;
		}
		else if (line[i] == '\'' && sh->sq == 1)
		{
			print_error("bush: missing closing quote `\''", sh);
			sh->sq *= -1;
		}
		i++;
	}
}

void	check_shielding(char *line, t_shell *sh)
{
	int i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '"' && line[i - 1] == '\\' && sh->dq == 1)
			;
		else if (line[i] == '"' && line[i - 1] != '\\')
			sh->dq *= -1;
		if (line[i] == '\'' && line[i - 1] == '\\' && sh->sq == 1)
			sh->sq *= -1;
		else if (line[i] == '\'' && line[i - 1] != '\\')
			sh->sq *= -1;
		i++;
	}
}

void	preparser(t_shell *sh)
{
	if (!sh->parsed_buf[skipspaces(sh->parsed_buf)])
	{
		sh->err[0] = 1;
		return ;
	}
	if ((ft_strchr(sh->parsed_buf, '"') ||
	ft_strchr(sh->parsed_buf, '\'')) && sh->err[0] != 1)
	{
		check_shielding(sh->parsed_buf, sh);
		check_quotes(sh->parsed_buf, sh);
	}
	if (ft_strchr(sh->parsed_buf, ';') && sh->err[0] != 1)
		check_semicolons(sh->parsed_buf, sh);
	if (ft_strchr(sh->parsed_buf, '|') && sh->err[0] != 1)
		check_pipes(sh->parsed_buf, sh);
	if ((ft_strchr(sh->parsed_buf, '>') ||
	ft_strchr(sh->parsed_buf, '<')) && sh->err[0] != 1)
		check_redirects(sh->parsed_buf, sh);
}

