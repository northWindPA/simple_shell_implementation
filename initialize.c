/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:46:18 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/27 21:08:48 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize(t_shell *sh)
{
	sh->isfound = 0;
	sh->err[0] = 0;
	sh->err[1] = 0;
	sh->sq = -1;
	sh->dq = -1;
	sh->f = 0;
	sh->ff[0] = -1;
	sh->ff[1] = -1;
	sh->ff[2] = 0;
	sh->cc = 0;
	sh->cntr = -1;
	sh->pips = 0;
	sh->env_n_l = 0;
	sh->kv = NULL;
	sh->env = NULL;
	sh->dup_env = NULL;
	sh->list = NULL;
	sh->ls_hist = NULL;
}
