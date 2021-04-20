/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 11:15:00 by keuclide          #+#    #+#             */
/*   Updated: 2020/11/09 13:17:48 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	unsigned char	*s1;
	int				i;

	i = 0;
	s1 = (unsigned char *)s;
	while (s1[i])
		i++;
	return (i);
}