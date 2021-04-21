/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 19:10:53 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/21 19:25:17 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		pps_exc(t_shell *shell)
{
	init(shell);
	num_of_rdrs(shell);
	if (!flag_chk(shell))
	{
		pps_free(shell);
		return (0);
	}
	cmd_exec(shell);
	pps_free(shell);
	return (0);
}

char	*cmd_chk(char **path, char *cmd)
{
	int		i;
	int		fd;
	char	*tmp;
	char	*cmd_tmp;

	i = 0;
	cmd_tmp = ft_strjoin("/", cmd);
	while (path[i])
	{
		if ((tmp = ft_strjoin(path[i], cmd_tmp)))
		{
			if ((fd = open(tmp, O_RDONLY)) > 0)
			{
				close(fd);
				free(cmd_tmp);
				return (tmp);
			}
			free(tmp);
		}
		i++;
	}
	free(cmd_tmp);
	return (ft_strdup(cmd));
}

char	*bin_path(t_shell *shell, char *cmd)
{
	char	*cmd_n_path;
	char	**path;

	if (!(path = ft_split(find_key_value("PATH", &shell->kv), ':')))
		return (0);
	cmd_n_path = cmd_chk(path, cmd);
	free_maker(path);
	return (cmd_n_path);
}

int		file_dir(char *cmd)
{
	int i;
	int fl;

	if (!cmd)
		return (0);
	i = 0;
	fl = 0;
	if (cmd[i] == '/')
	{
		fl = 1;
		i++;
	}
	while (cmd[i] == '.' && cmd[i] == '/' && cmd[i])
		i++;
	if (i == 1)
		return (1);
	if (!fl)
		return (0);
	return (1);
}

void	err_is_dir()
{
	g_exit = 126;
	ft_putstr_fd("is a directory", 2);
	write(2, "\n", 1);
}

void	err_perm_deny()
{
	g_exit = 126;
	ft_putstr_fd("Permission denied", 2);
	write(2, "\n", 1);
}

void	err_no_file_dir()
{
	g_exit = 127;
	ft_putstr_fd("No such file or directory", 2);
	write(2, "\n", 1);
}

void	err_cmd_n_found()
{
	g_exit = 127;
	ft_putstr_fd("command not found", 2);
	write(2, "\n", 1);
}

void	err_cmd(t_shell *shell)
{
	struct stat s;

	stat(shell->cmd_n_args[0], &s);
	if (S_ISDIR(s.st_mode))
		err_is_dir();
	else if (S_ISREG(s.st_mode))
		err_perm_deny();
	else if (file_dir(shell->cmd_n_args[0]))
		err_no_file_dir();
	else
		err_cmd_n_found();
}

void	exec_parent(t_shell *shell, int stts)
{
	if (stts == 0)
	{
		g_exit = 0;
		return ;
	}
	if (WIFEXITED(stts) != 0)
	{
		g_exit = 1;
		if (WEXITSTATUS(stts) == 127)
		{
			g_exit = 1;
			ft_putstr_fd(ERROR, 2);
			ft_putstr_fd(shell->cmd_n_args[0], 2);
			ft_putstr_fd(": ", 2);
			err_cmd(shell);
		}
	}
}

void	exec_child(t_shell *shell, char *cmd_n_path, int stts)
{
	if ((stts = execve(cmd_n_path, shell->cmd_n_args, shell->dup_env)))
		exit (127);
	free(cmd_n_path);
	exit(stts);
}

int		exec_other_bins(t_shell * shell)
{
	pid_t	pid;
	int		stts;
	char	*cmd_n_path;

	g_exit = 0;
	stts = 0;
	signals_start(2);
	cmd_n_path = bin_path(shell, shell->cmd_n_args[0]);
	if ((pid = fork()) < 0)
		err_out("fork()");
	if (pid == 0)
		exec_child(shell, cmd_n_path, stts);
	else
	{
		waitpid(pid, &stts, 0);
		exec_parent(shell, stts);
		if (cmd_n_path)
			free(cmd_n_path);
	}
	return (0);
}
