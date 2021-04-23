/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_env_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:28:32 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:31:52 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_kvadd_back(t_kv **lst, t_kv *new)
{
	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	ft_kvlast(*lst)->next = new;
	return ;
}

t_kv	*ft_lstkv(void *key, void *value)
{
	t_kv *new;

	new = (t_kv *)malloc(sizeof(t_kv));
	if (new == NULL)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

void	parse_env(t_shell *sh)
{
	char	**params;
	t_list	*tmp_head;

	tmp_head = sh->list;
	while (tmp_head)
	{
		params = ft_split(tmp_head->content, '=');
		ft_kvadd_back(&sh->kv,
		ft_lstkv(ft_strdup(params[0]), ft_strdup(params[1])));
		tmp_head = tmp_head->next;
		ft_freeall(params);
	}
}
