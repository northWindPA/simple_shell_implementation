/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/10 22:07:09 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/10 22:09:54 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr_free(char const *s, unsigned int start, size_t len)
{
	char	*mem;
	size_t	i;

	i = 0;
	if (s == NULL)
		return (NULL);
	if (start > ft_strlen(s))
	{
		mem = (char *)malloc((sizeof(char)) * 1);
		if (NULL == mem)
			return (NULL);
		mem[0] = '\0';
		return (mem);
	}
	mem = (char *)malloc((sizeof(char)) * (len + 1));
	if (NULL == mem)
		return (NULL);
	while (i < len)
	{
		mem[i] = s[start + i];
		i++;
	}
	mem[i] = '\0';
    free((char *)s);
	return (mem);
}
