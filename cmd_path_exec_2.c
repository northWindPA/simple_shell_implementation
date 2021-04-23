/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_exec_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:18:11 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:18:49 by mhumfrey         ###   ########.fr       */
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
