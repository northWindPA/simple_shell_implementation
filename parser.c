/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 13:00:02 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/20 22:09:18 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		join_if_arg(t_shell *sh, char *str)
{
	sh->fl_arg[sh->cntr] = 0;
	if (sh->ff == -1 && sh->cntr > 1 && sh->fl_arg[sh->cntr - 1] == 0)
	{
		sh->cntr--;
		sh->cmd_n_args[sh->cntr] =
		ft_strjoin_free(sh->cmd_n_args[sh->cntr], str, 1);
		return (1);
	}
	sh->ff = -1;
	return (0);
}

void	save_str_to_structure(t_shell *sh, char *str)
{
	int		i;
	char	**old;

	sh->cntr++;
	old = sh->cmd_n_args;
	if (!ft_strcmp(str, ">") || !ft_strcmp(str, "<")
	|| !ft_strcmp(str, ">>") || !ft_strcmp(str, "|"))
		sh->fl_arg[sh->cntr] = 1;
	else
	{
		if (join_if_arg(sh, str))
			return ;
	}
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

char	*replace_env_n_value(
		t_shell *sh, char *str, char *env_key, char *env_value)
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

void	one_two_dollars(t_shell *sh, char **str, char **oldstr, int *j)
{
	if (*str[*j] == '$' && (ft_isdigit(*str[*j + 1]) || *str[*j + 1] == '?'))
		*oldstr = dollar_one(sh, *str, *oldstr, *j);
	else if (*str[*j] == '$' &&
	(ft_isalnum(*str[*j + 1]) || *str[*j + 1] == '_'))
		*str = dollar_two(sh, *str, j);
}

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
			str = ft_chrjoin_free(str, line[i], 1);
			i++;
			break ;
		}
		str = ft_chrjoin_free(str, line[i], 1);
		i++;
	}
	if (line[i] == ' ')
		sh->ff *= -1;
	one_two_dollars(sh, &str, &oldstr, &j);
	save_str_or_oldstr(sh, str, oldstr);
	return (--i);
}

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
	if (line[*i + 1] == ' ' && sh->ff == 1)
		sh->ff *= -1;
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
		one_two_dollars(sh, &str, &oldstr, &j);
		j++;
	}
	save_str_or_oldstr(sh, str, oldstr);
	return (i);
}

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
		sh->ff *= -1;
	if (!break_chr(line[i]))
		save_str_to_structure(sh, str);
	str = save_redirects(sh, line, str, &i);
	str = save_pipe_or_isalnum(sh, line, str, &i);
	free(str);
	return (i);
}

int		parse_if_single_quotes(t_shell *sh, char *line, int i)
{
	char	*str;

	str = ft_strdup("");
	while (line[i] && line[i] != '\'')
	{
		str = ft_chrjoin_free(str, line[i], 1);
		i++;
	}
	if (line[i + 1] == ' ' && sh->ff == 1)
		sh->ff *= -1;
	save_str_to_structure(sh, str);
	free(str);
	return (i);
}

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

void	term_backup(int i)
{
	struct termios	term;

	tcgetattr(0, &term);
	if (i == 1)
	{
		term.c_lflag |= (ECHO);
		term.c_lflag |= (ICANON);
		term.c_lflag |= (ISIG);
	}
	else
	{
		term.c_lflag &= ~(ECHO);
		term.c_lflag &= ~(ICANON);
		// term.c_lflag &= ~(ISIG);
	}
	tcsetattr(0, TCSANOW, &term);
}

void	cutter(t_shell *sh, char **line, int *i)
{
	while (sh->parsed_buf[*i])
	{
		qfl(sh->parsed_buf, *i, sh);
		if (sh->parsed_buf[*i] == ';' &&
		sh->f == -1 && sh->parsed_buf[*i - 1] != '\\')
			break ;
		*line = ft_chrjoin_free(*line, sh->parsed_buf[*i], 1);
		(*i)++;
	}
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
