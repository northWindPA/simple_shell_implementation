/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_chrjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/10 00:47:53 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/13 02:21:46 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_chrjoin_free(char const *str, char c, int free_buf)
{
	char	*mem;
	int		i;

	i = 0;
	if (!str)// || !c)
		return (NULL);
	if (!(mem = (char *)malloc((sizeof(char)) * (ft_strlen(str) + 2))))
		return (NULL);
	while (str[i] != '\0')
	{
		mem[i] = str[i];
		i++;
	}
	mem[i] = c;
	mem[i + 1] = '\0';
    if (free_buf == 1)
        free((char *)str);
	return (mem);
}
