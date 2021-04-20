/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/10 00:29:31 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/10 00:54:35 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static  void	ft_free_buf(char const *s1, char const *s2, int buf_num)
{
    if (buf_num == 1)
        free((char *)s1);
    else if (buf_num == 2)
        free((char *)s2);
    else if (buf_num == 3)
    {
        free((char *)s1);
        free((char *)s2);
    }
}

char			*ft_strjoin_free(char const *s1, char const *s2, int buf_num)
{
	char	*mem;
	int		i;
	int		x;

	i = 0;
	x = 0;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	mem = (char *)malloc((sizeof(char)) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (NULL == mem)
		return (NULL);
	while (s1[i] != '\0')
	{
		mem[i] = s1[i];
		i++;
	}
	while (s2[x] != '\0')
	{
		mem[i + x] = s2[x];
		x++;
	}
	mem[i + x] = '\0';
    ft_free_buf(s1, s2, buf_num);
	return (mem);
}
