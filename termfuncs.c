/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termfuncs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 12:58:47 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/21 20:29:27 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	term.c_lflag &= ~(ISIG);
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
	ft_putstr_fd(PROMT, 1);
	tputs(save_cursor, 1, ft_putchar);
	get_hist(sh);
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

void	term_loop(t_shell *sh, int i, int *pos)
{
	while (ft_strcmp(sh->buf, "\n"))
	{
		i = read(0, sh->buf, 999);
		sh->buf[i] = 0;
		if (!ft_strncmp(sh->buf, "\n", 1) ||
		(!ft_strncmp(sh->buf, "\4", 1) && sh->hist[*pos][0] == 0))
			return ;
		if (!ft_strcmp(sh->buf, "\e[A") || !ft_strcmp(sh->buf, "\e[B"))
			*pos = up_and_down(sh, *pos);
		else if (!ft_strcmp(sh->buf, "\177") || !ft_strcmp(sh->buf, "\t") ||
		(ft_strcmp(sh->buf, "\e[C") && ft_strcmp(sh->buf, "\e[D")))
			backslash_and_tab(sh, *pos, i);
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
		signals_start(1);
		print_promt(sh, &i, &pos);
		term_loop(sh, i, &pos);
		sh->parsed_buf = ft_strdup((sh->hist)[pos]);
		history_record_and_call_parser(sh, pos);
	}
	free(sh->buf);
}
