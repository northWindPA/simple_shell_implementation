/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 18:38:28 by keuclide          #+#    #+#             */
/*   Updated: 2020/12/29 19:06:45 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int				check_count(int fd, char **line, char **rem, char *buff)
{
	char		*tmp;
	char		*n;
	int			count;

	n = NULL;
	while (n == NULL && (count = read(fd, buff, BUFFER_SIZE)) > 0)
	{
		if (count == -1)
			return (-1);
		buff[count] = '\0';
		if ((n = ft_mystrchr(buff, '\n')))
		{
			*n = '\0';
			*rem = ft_mystrdup(++n);
		}
		tmp = *line;
		*line = ft_mystrjoin(*line, buff);
		free(tmp);
	}
	return (count);
}

char			*check_rem(char **line, char **rem)
{
	char		*n;

	n = NULL;
	if (*rem != NULL)
	{
		if ((n = ft_mystrchr(*rem, '\n')))
		{
			*n = '\0';
			*line = *rem;
			*rem = ft_mystrdup(++n);
		}
		else
		{
			*line = *rem;
			*rem = NULL;
		}
	}
	else
		*line = ft_mymzero(1);
	return (n);
}

int				get_next_line(int fd, char **line)
{
	static char	*rem;
	char		*buff;
	int			count;

	if (fd < 0 || !line || BUFFER_SIZE < 1)
		return (-1);
	if (!(buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (-1);
	if ((count = read(fd, buff, 0)) == -1)
	{
		free(buff);
		return (-1);
	}
	if (!(check_rem(line, &rem)))
		if ((count = check_count(fd, line, &rem, buff)) < 0)
			return (-1);
	free(buff);
	if (rem || count)
		return (1);
	return (0);
}
