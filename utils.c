/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 15:46:27 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/20 02:31:46 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	err_out(char *str)
{
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
}

int		chk_tkn(char *str, char *line)
{
	if ((ft_strcmp(str, line) != 0) || (ft_strlen(str) != ft_strlen(line)))
		return (0);
	return (1);
}

int		len_of_2d_array(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->cmd_n_args[i])
		i++;
	return (i);
}

void	init(t_shell *shell)
{
	if (!(shell->ind_arg = (int *)malloc
	(sizeof(int) * len_of_2d_array(shell))))
		exit(12);
	shell->pps = 0;
	shell->p_in = NULL;
	shell->p_out = NULL;
	shell->fl_rdr_in = 0;
	shell->fl_rdr_out = 0;
	shell->fl_rdr_app = 0;
}

void	num_of_rdrs(t_shell *shell)
{
	int i;

	i = 0;
	shell->rdr_i = 0;
	while (shell->cmd_n_args[i])
	{
		if (shell->fl_arg[i] &&
		(chk_tkn(">", shell->cmd_n_args[i])
		|| chk_tkn(">>", shell->cmd_n_args[i])))
			shell->rdr_i++;
		i++;
	}
}

void	pps_free(t_shell *shell)
{
	if (shell->ind_arg)
		free(shell->ind_arg);
	if (shell->p_out)
	{
		free(shell->p_out);
		shell->p_out = NULL;
	}
	if (shell->p_in)
	{
		free(shell->p_in);
		shell->p_in = NULL;
	}
}
