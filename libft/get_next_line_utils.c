/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 18:39:38 by keuclide          #+#    #+#             */
/*   Updated: 2020/11/20 10:17:55 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_mystrlen(const char *s)
{
	unsigned char	*s1;
	int				i;

	i = 0;
	s1 = (unsigned char *)s;
	while (s1[i])
		i++;
	return (i);
}

char	*ft_mymzero(int n)
{
	char	*f;
	int		i;

	i = 0;
	if ((f = (char *)malloc(sizeof(char) * (n + 1))) == NULL)
		return (NULL);
	while (i < n)
	{
		f[i] = '\0';
		i++;
	}
	f[i] = '\0';
	return (f);
}

char	*ft_mystrchr(const char *s, int c)
{
	char	*ns;

	ns = (char *)s;
	while (*ns != c)
	{
		if (*ns == '\0')
			return (0);
		ns++;
	}
	return (ns);
}

char	*ft_mystrdup(const char *s1)
{
	char	*s;
	int		i;

	i = 0;
	while (s1[i] != '\0')
		i++;
	s = (char *)malloc((sizeof(char)) * (i + 1));
	if (NULL == s)
		return (0);
	i = 0;
	while (s1[i] != '\0')
	{
		s[i] = s1[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

char	*ft_mystrjoin(char const *s1, char const *s2)
{
	char	*mem;
	int		i;
	int		x;

	i = 0;
	x = 0;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	mem = (char *)malloc((sizeof(char)) * (ft_mystrlen(s1) + ft_mystrlen(s2) + 1));
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
	return (mem);
}
