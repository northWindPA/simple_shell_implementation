/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/10 21:59:55 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/11 15:45:33 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup_free(const char *oldstr, const char *str, int buf_num)
{
	char	*s;
	int		i;

	i = 0;
	while (str[i] != '\0')
		i++;
	s = (char *)malloc((sizeof(char)) * (i + 1));
	if (NULL == s)
		return (0);
	i = 0;
	while (str[i] != '\0')
	{
		s[i] = str[i];
		i++;
	}
	s[i] = '\0';
	if (buf_num == 1)
    	free((char *)oldstr);
	else if (buf_num == 2)
    	free((char *)str);
	else if (buf_num == 3)
	{
    	free((char *)oldstr);
    	free((char *)str);
	}
	return (s);
}
