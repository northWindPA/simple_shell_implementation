/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pps_n_cmd_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:40:14 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:40:43 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		op_path(t_shell *shell, int fd, char **path)
{
	if ((fd = open(*path, O_CREAT | O_WRONLY | O_APPEND, 0777)) < 0)
	{
		err_out(*path);
		return (1);
	}
	shell->rdr_i--;
	close(fd);
	return (0);
}

int		flag_on(t_shell *shell, char **path, int i, int *fl)
{
	int fd;

	*path = ft_strdup(shell->cmd_n_args[i + 1]);
	if (chk_tkn(shell->cmd_n_args[i], ">")
	|| chk_tkn(shell->cmd_n_args[i], ">>")
	|| chk_tkn(shell->cmd_n_args[i], "<"))
		if (op_path(shell, fd, path))
			return (1);
	if (shell->rdr_i > 0)
		if (*path)
			free(*path);
	free(shell->cmd_n_args[i + 1]);
	free(shell->cmd_n_args[i]);
	shell->cmd_n_args[i + 1] = ft_strdup("eshkere");
	shell->cmd_n_args[i] = 0;
	*fl = 1;
	return (0);
}

int		fl_chk_rdr(t_shell *shell, int i)
{
	int fl_res;

	fl_res = 0;
	if (!ft_strcmp(shell->cmd_n_args[i], ">"))
	{
		fl_res = flag_on(shell, &shell->p_out, i, &shell->fl_rdr_out);
		if (fl_res)
			return (0);
	}
	else if (!ft_strcmp(shell->cmd_n_args[i], ">>"))
	{
		fl_res = flag_on(shell, &shell->p_out, i, &shell->fl_rdr_app);
		if (fl_res)
			return (0);
	}
	else if (!ft_strcmp(shell->cmd_n_args[i], "<"))
	{
		fl_res = flag_on(shell, &shell->p_in, i, &shell->fl_rdr_in);
		if (fl_res)
			return (0);
	}
	free(shell->cmd_n_args[i + 1]);
	return (1);
}
