/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhumfrey <mhumfrey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/08 17:24:08 by mhumfrey          #+#    #+#             */
/*   Updated: 2021/04/18 22:14:41 by mhumfrey         ###   ########.fr       */
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
		ft_kvadd_back(&sh->kv, ft_lstkv(ft_strdup(params[0]), ft_strdup(params[1])));
		tmp_head = tmp_head->next;
		ft_freeall(params);
	}
}

void	make_env_list(char **env, t_shell *sh)
{
	int 	i;

	i = -1;
	while(env[++i])
		ft_lstadd_back(&sh->list, ft_lstnew(env[i]));
	parse_env(sh);
	ft_lstclear(&sh->list, free);
}

void	get_env(char **env, t_shell *sh)
{
	int		i;

	i = -1;
	while (env[++i])
		;
	if ((sh->dup_env = (char **)malloc(sizeof(char *) * (++i))) == NULL)
		exit(0);
	sh->env_n_l = -1;
	while (env[++sh->env_n_l])
		if ((sh->dup_env[sh->env_n_l] = ft_strdup(env[sh->env_n_l])) == NULL)
			exit(0);
	sh->dup_env[sh->env_n_l] = 0;
	make_env_list(sh->dup_env, sh);
}
