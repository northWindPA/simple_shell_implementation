/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keuclide <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 18:39:49 by keuclide          #+#    #+#             */
/*   Updated: 2020/11/23 14:04:32 by keuclide         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>

# define BUFFER_SIZE 1

int		get_next_line(int fd, char **line);
size_t	ft_mystrlen(const char *s);
char	*ft_mymzero(int n);
char	*ft_mystrjoin(char const *s1, char const *s2);
char	*ft_mystrchr(const char *s, int c);
char	*ft_mystrdup(const char *s1);

#endif
