/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pps_n_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 15:49:29 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/20 02:31:16 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		op_path(t_shell *shell, int fd, char **path)
{
	if ((fd = open(*path, O_CREAT | O_WRONLY | O_APPEND , 0777)) < 0)
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
	shell->cmd_n_args[i + 1] = ft_strdup("wp");
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
	return (1);
}

int		chk_pps_tkns_2(t_shell *shell, int i)
{
	shell->ind_arg[shell->pps + 1] = i + 1;
	free(shell->cmd_n_args[i]);
	shell->cmd_n_args[i] = 0;
	(shell->pps)++;
	return (i);
}

int		chk_pps_tkns(t_shell *shell, int i)
{
	if (!ft_strcmp(shell->cmd_n_args[i], "|"))
		i = chk_pps_tkns_2(shell, i);
	else if (!ft_strcmp(shell->cmd_n_args[i], ">")
	|| !ft_strcmp(shell->cmd_n_args[i], ">>")
	|| !ft_strcmp(shell->cmd_n_args[i], "<"))
		if (!fl_chk_rdr(shell, i))
			return (0);
	return (1);
}

int		flag_chk(t_shell *shell)
{
	int i;

	i = 0;
	while (shell->cmd_n_args[i])
	{
		if (shell->fl_arg[i])
		{
			if	(!chk_pps_tkns(shell, i))
				return (0);
		}
		else
			shell->ind_arg[i] = i;
		i++;
	}
	return (1);
}

void	pps_fd_1(t_shell *shell)
{
	close(1);
	dup(shell->pp_r[1]);
	close(shell->pp_r[0]);
	close(shell->pp_r[1]);
}

void	pps_fd_2(t_shell *shell)
{
	close(0);
	dup(shell->pp_l[0]);
	close(shell->pp_l[0]);
	close(shell->pp_l[1]);
	close(1);
	dup(shell->pp_r[1]);
	close(shell->pp_r[0]);
	close(shell->pp_r[1]);
}

void	pps_fd_3(t_shell *shell)
{
	close(0);
	dup(shell->pp_l[0]);
	close(shell->pp_l[0]);
	close(shell->pp_l[1]);
}

void	pps_fd(t_shell *shell, int i)
{
	if (!i)
		pps_fd_1(shell);
	else if (i < shell->pps)
		pps_fd_2(shell);
	else
		pps_fd_3(shell);
}

int		rdr_app(t_shell *shell)
{
	shell->out_fd = open(shell->p_out, O_CREAT | O_WRONLY | O_APPEND, 0777);
	if (shell->out_fd < 0)
		return (1);
	close(1);
	dup(shell->out_fd);
	close(shell->out_fd);
	return (0);
}

int		rdr_out(t_shell *shell)
{
	shell->out_fd = open(shell->p_out, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (shell->out_fd < 0)
		return (1);
	close(1);
	dup(shell->out_fd);
	close(shell->out_fd);
	return (0);
}

int		rdr_in(t_shell *shell, int i)
{
	if (shell->fl_rdr_in && !i)
	{
		shell->in_fd = open(shell->p_in, O_RDONLY);
		if (shell->in_fd < 0)
		{
			ft_putstr_fd(ERROR, 2);
			ft_putstr_fd(shell->p_in, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			g_exit = 1;
		}
		close(0);
		dup(shell->in_fd);
		close(shell->in_fd);
	}
	return (0);
}

void	child(t_shell *shell, int i)
{
	char	*cmd_n_path;

	if (shell->fl_rdr_in && !i)
		rdr_in(shell, i);
	if (shell->fl_rdr_out
	&& i == shell->pps)
		rdr_out(shell);
	if (shell->fl_rdr_app
	&& i == shell->pps)
		rdr_app(shell);
	if (shell->pps > 0)
		pps_fd(shell, i);
	if (!(cmd_n_path = bin_path(shell,
	shell->cmd_n_args[shell->ind_arg[i]])))
		exit(3);
	shell->stts = execve(cmd_n_path, &shell->cmd_n_args
	[shell->ind_arg[i]], shell->dup_env);
	if (cmd_n_path)
		free(cmd_n_path);
	err_out(cmd_n_path);
	exit(shell->stts);
}

void	cmd_not_found(t_shell *shell)
{
	ft_putstr_fd(ERROR, 2);
	ft_putstr_fd(shell->cmd_n_args[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd("command not found", 2);
}

void	close_change_fd(t_shell *shell, int i)
{
	g_exit = 0;
	if (i > 0)
	{
		close(shell->pp_l[0]);
		close(shell->pp_l[1]);
	}
	shell->pp_l[0] = shell->pp_r[0];
	shell->pp_l[1] = shell->pp_r[1];
}

void	parent(t_shell *shell, int i)
{
	close_change_fd(shell, i);
	wait(&shell->stts);
	if (WIFEXITED(shell->stts) != 0)
	{
		if (WEXITSTATUS(shell->stts) == 3)
			cmd_not_found(shell);
		g_exit = 1;
	}
}

int		cmd_exec(t_shell *shell)
{
	int 	i;
	pid_t	pid;
	int		pip_res;

	i = 0;
	pip_res = 0;
	while (i <= shell->pps)
	{
		if (i != shell->pps && shell->pps > 0)
		{
			pip_res = pipe(shell->pp_r);
			if (pip_res < 0)
				err_out("pipe error");
		}
		pid = fork();
		if (pid == 0)
			child(shell, i);
		else if (pid < 0)
			err_out("fork error");
		else
			parent(shell, i);
		i++;
	}
	return (0);
}
