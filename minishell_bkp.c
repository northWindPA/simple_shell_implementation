#include "minishell.h"

void	ft_sigint(int signal)
{
	pid_t	pid;
	int		status;

	(void)signal;
	pid = waitpid(0, &status, WNOHANG);
	if (pid == 0)
		g_sig = 2;
	else
	{
		ft_putstr_fd(PROMT, 1);
		g_sig = 1;
	}
}

void	ft_sigquit(int signal)
{
	pid_t	pid;
	int		status;

	(void)signal;
	pid = waitpid(0, &status, WNOHANG);
	if (pid == 0)
	{
		ft_putstr_fd(PROMT, 1);
		g_sig = 3;
	}
}

void	initialize(t_shell *sh)
{
	sh->isfound = 0;
	sh->err[0] = 0;
	sh->err[1] = 0;
	sh->sq = -1;
	sh->dq = -1;
	sh->f = 0;
	sh->ff = -1;
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
    write(2, "\n", 1);
    write(2, error_msg, ft_strlen(error_msg));
    write(2, "\n", 1);
	errcode(sh, 1, 258);
}

void    print_cmd_error(const char *error_msg, t_shell *sh)
{
    write(2, "\nbush: ", 7);
    write(2, error_msg, ft_strlen(error_msg));
    write(2, ": command not found\n", 20);
	errcode(sh, 1, 127);
}

void	do_exit(char *msg)
{
    write(1, "\n", 1);
    write(1, msg, ft_strlen(msg));
    write(1, "\n", 1);
	exit(0);
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

int		check_spaces(t_shell *sh)
{
	int i;

	i = 0;
	while (ft_isspace(sh->parsed_buf[i]))
		i++;
	return (i);
}

void	qfl(char *line, int i, t_shell *sh)
{
	if (line[i] == '"' || line[i] == '\'')
		sh->f *= -1;
}

// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------

void	check_semicolons(char *line, t_shell *sh)
{
    int i;

	sh->f = -1;
    i = skipspaces(line);
	qfl(line, i + 1, sh);
	if (line[i] == ';' && line[i + 1] != '\0' && line[i + 1] != ';' && line[0] != ';' && sh->f == -1)
		print_error("bush: syntax error near unexpected token `;'", sh);
	else if (line[i] == ';' && line[i + 1] == '\0' && line[0] != ';' && sh->f == -1)
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
	while (line[i])
	{
		qfl(line, i, sh);
		if (line[i] == ';' && line[i - 1] == '\\')
			i++;
		if (line[i] == ';' && line[0] == ';' && line[1] != ';' && i == 0 && sh->f == -1)
			print_error("bush: syntax error near unexpected token `;'", sh);
		else if (line[i] == ';' && line[i + 1] == ';' && line[i + 2] != ';' && sh->f == -1)
			print_error("bush: syntax error near unexpected token `;;'", sh);
		i++;
	}
}

void	check_pipes(char *line, t_shell *sh)
{
	int i;

	sh->f = 0;
	i = skipspaces(line);
	if (line[i] == '|' && line[i + 1] != '\0' && line[i + 1] != '|' && line[0] != '|')
		print_error("bush: syntax error near unexpected token `|'", sh);
	while (line[i] && sh->f == 0)
	{
		if (line[i] == '|' && line[i + 1] == '|' && sh->f++ == 0)
		{
			print_error("bush: syntax error near unexpected token `||'", sh);
			sh->pips = 2;
		}
		else if (line[i] == '|' && line[i + 1] == ' ')
		{
			while (ft_isspace(line[++i]))
				;
			if (line[i] == '\0')
				print_error("bush: syntax error near unexpected token `|'", sh);
			sh->pips = 2;
		}
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
	if ((line[ft_strlen(line) - 1] == '>' || line[ft_strlen(line) - 1] == '<') && sh->f++ == 0)
		print_error("bush: syntax error near unexpected token `newline'", sh);
	i = skipspaces(line);
	while (line[i] && sh->f == 0)
	{
		if ((line[i] == '>' || line[i] == '<') && line[i + 1] == '<' && line[i + 1] == '>'
		&& line[i + 2] != '<' && line[i + 2] != '>')
		{
			if (!ft_strncmp(line + i, "<", 1) || !ft_strncmp(line + i, ">", 1))
				i++;
			else if (!ft_strncmp(line + i, ">>", 2))
				i += 2;
			while (ft_isspace(line[i]))
				i++;
			if (line[i] == '\0' || line[i] == '>' || line[i] == '<')
				print_error("bush: syntax error near unexpected token `newline'", sh);
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
	if ((ft_strchr(sh->parsed_buf, '"') || ft_strchr(sh->parsed_buf, '\'')) && sh->err[0] != 1)
	{
		check_shielding(sh->parsed_buf, sh);
		check_quotes(sh->parsed_buf, sh);
	}
	if (ft_strchr(sh->parsed_buf, ';') && sh->err[0] != 1)
		check_semicolons(sh->parsed_buf, sh);
	if (ft_strchr(sh->parsed_buf, '|') && sh->err[0] != 1)
		check_pipes(sh->parsed_buf, sh);
	if ((ft_strchr(sh->parsed_buf, '>') || ft_strchr(sh->parsed_buf, '<')) && sh->err[0] != 1)
		check_redirects(sh->parsed_buf, sh);
}

// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------

// PARSER
//-------------------------------------------------------
// echo \"dsdsdsad""
// echo "123"'456'
// echo $TERM$SHELL
// echo |           ;
// echo hello           ;             ;         ;
// echo -n
// echo -n $E
// echo 123 $R 123
//-------------------------------------------------------

// LOGIC
//-------------------------------------------------------
// cd, export and other builtin functions
//-------------------------------------------------------

int		ft_istoken(char c)
{
	if (c == '\'' || c == '"' || c == ';' || c == '\\' || c == '.' || c == '\0' ||
		c == '|' || c == ' ' || c == '>' || c == '<' || c == '/' || c == '$')
		return (1);
	return (0);
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
int		join_if_arg(t_shell *sh, char *str)
{
	sh->fl_arg[sh->cntr] = 0;
	if (sh->ff == -1 && sh->cntr > 1 && sh->fl_arg[sh->cntr - 1] == 0)
	{
		sh->cntr--;
		sh->cmd_n_args[sh->cntr] = ft_strjoin_free(sh->cmd_n_args[sh->cntr], str, 1);
		return (1);
	}
	sh->ff = -1;
	return (0);
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

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
		if (join_if_arg(sh, str))
			return ;
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

char	*replace_env_n_value(t_shell *sh, char *str, char *env_key, char *env_value)
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
	if (!(tmp2 = ft_substr_free(str, i + ft_strlen(env_key) + 1, ft_strlen(str))))
		print_error("bush: cannot allocate memory", sh);
	if (!(tmp1 = ft_strjoin_free(tmp1, tmp2, 1)))
		print_error("bush: cannot allocate memory", sh);
	free(tmp2);
	return (tmp1);
}

char	*dollar_one(t_shell *sh, char *str, char *oldstr, int j)
{
	if (ft_isdigit(str[j + 1]))
	{
		oldstr = ft_strjoin_free(oldstr, ft_substr(str, j + 2, ft_strlen(str)), 3);
		sh->isfound = 0;
	}
	else if (str[j + 1] == '?')
	{
		oldstr = ft_strjoin_free(oldstr, ft_itoa(sh->err[1]), 3);
		oldstr = ft_strjoin_free(oldstr, ft_substr(str, j + 2, ft_strlen(str)), 3);
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
	sh->kv = sh->kv_head;
	str = ft_strdup("");
	oldstr = ft_strdup("");
	while (line[i] && !ft_isspace(line[i]))
	{
		//-------------------------------------------------------------------
		//-------------------------------------------------------------------
		if (line[i + 1] == '$'  || line[i + 1] == '"' || line[i + 1] == '\'')
		{
			str = ft_chrjoin_free(str, line[i], 1);
			i++;
			break ;
		}
		//-------------------------------------------------------------------
		//-------------------------------------------------------------------
		str = ft_chrjoin_free(str, line[i], 1);
		i++;
	}
	if (line[i] == ' ')
		sh->ff *= -1;
	if (str[j] == '$' && (ft_isdigit(str[j + 1]) || str[j + 1] == '?'))
		oldstr = dollar_one(sh, str, oldstr, j);
	else if (str[j] == '$' && (ft_isalnum(str[j + 1]) || str[j + 1] == '_'))
		str = dollar_two(sh, str, &j);
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

	sh->kv = sh->kv_head;
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
		{
			str = dollar_two(sh, str, &j);
			ln += j - ln;
		}
		j++;
	}
	save_str_or_oldstr(sh, str, oldstr);
	return (i);
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
	else if (line[*i] == '>' && line[*i + 1] == '>' && line[*i + 2] != '<' && line[*i + 2] != '>')
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
	else if (line[*i] == '\\' && !ft_isalnum(line[*i + 1]))
	{
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
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_' || line[i] == '.'))
	{
		if (!ft_isalnum(line[i + 1]))
		{
			str = ft_chrjoin_free(str, line[i], 1);
			break ;
		}
		str = ft_chrjoin_free(str, line[i], 1);
		i++;
	}
	if (line[i] == ' ')
		sh->ff *= -1;
	if (ft_isalnum(line[i]) || line[i] == '_' || line[i] == '.')
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
		term.c_lflag &= ~(ISIG);
	}
	tcsetattr(0, TCSANOW, &term);
}

void	parser(t_shell *sh)
{
	int		i;
	char	*line;

	i = 0;
	sh->f = -1;
	while (sh->parsed_buf[i])
	{
		while (sh->parsed_buf[i])
		{
			qfl(sh->parsed_buf, i, sh);
			if (sh->parsed_buf[i] == ';' && sh->f == -1 && sh->parsed_buf[i - 1] != '\\')
				break ;
			line = ft_chrjoin_free(line, sh->parsed_buf[i], 1);
			i++;
		}
		processor(sh, line);
		term_backup(1);
		// exec(sh);
		term_backup(0);
		free_maker(sh->cmd_n_args);
		if (sh->parsed_buf[i] == '\0')
			break ;
		i++;
	}
	// free(line);
}

// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------

void	get_hist(t_shell *sh)
{
	char	*line;
	t_list	*tmp;
	int		fd;
	int		i;

	line = NULL;
	if ((fd = open(HISTORY, O_CREAT | O_RDONLY, 0777)) == -1)
		print_error("bush: Change name of the history file please", sh);
	while (get_next_line(fd, &line))
		ft_lstadd_back(&sh->ls_hist, ft_lstnew(line));
	ft_lstadd_back(&sh->ls_hist, ft_lstnew(line));
	i = ft_lstsize(sh->ls_hist);
	if (!(sh->hist = (char **)malloc(sizeof(char *) * (2 + i))))
		print_error("bush: cannot allocate memory", sh);
	sh->hist[i + 1] = NULL;
	tmp = sh->ls_hist;
	while (tmp)
	{
		sh->hist[i--] = tmp->content;
		tmp = tmp->next;
	}
	ft_lstclear(&sh->ls_hist, free);
	sh->hist[0] = ft_strdup("");
	close(fd);
}

void	join_buf(char **hist, char *command, t_shell *sh)
{
	int fd;

	if ((fd = open(HISTORY, O_WRONLY | O_APPEND)) == -1)
		print_error("bush: Change name of the history file please", sh);
	if (ft_strslen(hist) > 1)
		write(fd, "\n", 1);
	write(fd, command, ft_strlen(command));
	close(fd);
}

void	setup_term(t_shell *sh)
{
	struct termios	term;

	tcgetattr(0, &term);
	term.c_lflag &= ~(ECHO);
	term.c_lflag &= ~(ICANON);
	tcsetattr(0, TCSANOW, &term);
	sh->kv_head = sh->kv;
	while (sh->kv && ft_strcmp(sh->kv->key, "TERM"))
		sh->kv = sh->kv->next;
	tgetent(0, sh->kv->value);
}

void	print_promt(t_shell *sh, int *i, int *pos)
{
	*i = 1;
	*pos = 0;
	tputs(save_cursor, 1, ft_putchar);
	ft_putstr_fd(PROMT, 1);
	get_hist(sh);
}

void	history_record_and_call_parser(t_shell *sh, int pos)
{
	if (!ft_strncmp(sh->buf, "\n", 1))
	{
		sh->buf[0] = 0;
		if ((sh->hist)[pos][0] != 0)
			join_buf(sh->hist, sh->hist[pos], sh);
	}
	if (sh->hist[pos][0] != 0)
	{
		preparser(sh);
		if (sh->err[0] == 0)
			parser(sh);
		sh->err[0] = 0;
	}
	free(sh->parsed_buf);
	free_maker(sh->hist);
	if (ft_strncmp(sh->buf, "\4", 1))
		write(1, "\n", 1);
}

int		up_and_down(t_shell *sh, int pos)
{
	if (!ft_strcmp(sh->buf, "\e[A"))
	{
		if (ft_strslen(sh->hist) - pos > 1)
		{
			tputs(delete_line, 1, ft_putchar);
			pos++;
			ft_putstr_fd(PROMT, 1);
			write(1, sh->hist[pos], ft_strlen(sh->hist[pos]));
		}
	}
	else if (!ft_strcmp(sh->buf, "\e[B"))
	{
		if (pos > 0)
		{
			tputs(delete_line, 1, ft_putchar);
			pos--;
			ft_putstr_fd(PROMT, 1);
			write(1, sh->hist[pos], ft_strlen(sh->hist[pos]));
		}
	}
	return (pos);
}

void	backslash_and_tab(t_shell *sh, int pos, int i)
{
	if (!ft_strcmp(sh->buf, "\177"))
	{
		if (ft_strlen(sh->hist[pos]) > 0)
		{
			tputs(cursor_left, 1, ft_putchar);
			tputs(delete_character, 1, ft_putchar);
			sh->hist[pos][ft_strlen(sh->hist[pos]) - 1] = 0;
		}
	}
	else if (!ft_strcmp(sh->buf, "\t"))
	{
		sh->hist[pos] = ft_strjoin_free(sh->hist[pos], "    ", 1);
		write(1, "    ", 4);
	}
	else if (ft_strcmp(sh->buf, "\e[C") && ft_strcmp(sh->buf, "\e[D"))
	{
		sh->hist[pos] = ft_strjoin_free(sh->hist[pos], sh->buf, 1);
		write(1, sh->buf, i);
	}
}

void	term_func(t_shell *sh)
{
	int		pos;
	int		i;

	if (!(sh->buf = (char *)ft_calloc(1000, 1)))
		print_error("bush: malloc error", sh);
	while (ft_strncmp(sh->buf, "\4", 1))
	{
		print_promt(sh, &i, &pos);
		while (ft_strcmp(sh->buf, "\n"))
		{
			i = read(0, sh->buf, 999);
			sh->buf[i] = 0;
			if (!ft_strncmp(sh->buf, "\n", 1) || !ft_strncmp(sh->buf, "\4", 1))
				break ;
			if (!ft_strcmp(sh->buf, "\e[A") || !ft_strcmp(sh->buf, "\e[B"))
				pos = up_and_down(sh, pos);
			else if (!ft_strcmp(sh->buf, "\177") || !ft_strcmp(sh->buf, "\t") ||
			(ft_strcmp(sh->buf, "\e[C") && ft_strcmp(sh->buf, "\e[D")))
				backslash_and_tab(sh, pos, i);
		}
		sh->parsed_buf = ft_strdup((sh->hist)[pos]);
		if (!ft_strncmp(sh->parsed_buf, "exit", 4))
			break ;
		history_record_and_call_parser(sh, pos);
	}
	free(sh->buf);
	do_exit("exit");
}
