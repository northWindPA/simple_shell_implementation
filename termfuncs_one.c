/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termfuncs_one.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 12:58:47 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/23 23:02:02 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		up_and_down(t_shell *sh, int pos)
{
	if (!ft_strcmp(sh->buf, "\e[A"))
	{
		if (ft_strslen(sh->hist) - pos > 1)
		{
			tputs(restore_cursor, 1, ft_putchar);
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
			tputs(restore_cursor, 1, ft_putchar);
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
		i = 1;
		pos = 0;
		signals_start(1);
		print_promt(sh);
		term_loop(sh, i, &pos);
		sh->parsed_buf = ft_strdup((sh->hist)[pos]);
		if (!ft_strncmp(sh->buf, "\4", 1))
			ft_putendl_fd("exit", 1);
		history_record_and_call_parser(sh, pos);
	}
	free(sh->buf);
}
