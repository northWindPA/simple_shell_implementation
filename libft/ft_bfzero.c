/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bfzero.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <keuclide@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/10 00:52:59 by keuclide          #+#    #+#             */
/*   Updated: 2021/04/12 23:46:08 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bfzero(void **s, size_t n)
{
	unsigned char	**f;
	size_t			i;

	f = (unsigned char **)s;
	i = 0;
	while (i < n)
	{
		ft_bzero(*f, ft_strlen(*f));
		f++;
		i++;
	}
}
