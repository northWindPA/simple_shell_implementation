/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_env_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 22:27:58 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/23 22:31:25 by mhumfrey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_key_value(char *key, t_kv **kv)
{
	t_kv	*tmp;

	tmp = *kv;
	if (!kv || !(*kv))
		return (NULL);
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (tmp->value)
				return (tmp->value);
			else if (tmp->value == NULL)
				return (NULL);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	**ft_freeall(char **arr)
{
	int i;

	i = 0;
	while (arr[i] != 0)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

t_kv	*ft_kvlast(t_kv *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}
