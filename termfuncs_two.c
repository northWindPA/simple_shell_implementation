/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termfuncs_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:58:21 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/23 23:00:05 by keuclide         ###   ########.fr       */
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
	}
	tcsetattr(0, TCSANOW, &term);
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
	tgetent(0, find_key_value("TERM", &sh->kv));
}

void	print_promt(t_shell *sh)
{
	tputs(save_cursor, 1, ft_putchar);
	ft_putstr_fd(PROMT, 1);
	get_hist(sh);
}
