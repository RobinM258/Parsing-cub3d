/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: romartin <romartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 10:57:54 by romartin          #+#    #+#             */
/*   Updated: 2023/03/17 10:28:35 by romartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_free(char *strs, char *buf)
{
	if (buf)
		free(buf);
	if (strs)
		free(strs);
	return (NULL);
}

static char	*ft_get_end(char *strs)
{
	int		i;
	int		a;
	char	*red;

	i = 0;
	a = -1;
	while (strs[i] != 0 && strs[i] != '\n')
		i++;
	if (strs[i] == 0)
	{
		free(strs);
		return (NULL);
	}
	red = ft_calloc((ft_strlen(strs) - i) + 1, sizeof(char));
	if (!red)
	{
		free (strs);
		return (NULL);
	}
	i++;
	while (strs[i + (++a)])
		red[a] = strs[i + a];
	free(strs);
	return (red);
}

static char	*ft_get_start(char *strs, char *ret)
{
	int		i;

	i = 0;
	if (!strs)
		return (NULL);
	while (strs[i] && strs[i] != '\n')
		i++;
	if (strs[i] == '\n')
		i++;
	ret = ft_calloc(i + 1, sizeof(char));
	if (!ret)
		return (ft_free(strs, NULL));
	i = 0;
	while (strs[i] && strs[i] != '\n')
	{
		ret[i] = strs[i];
		i++;
	}
	if (strs[i] == '\n')
		ret[i++] = '\n';
	ret[i] = '\0';
	return (ret);
}

static char	*ft_read(char *strs, int fd)
{
	int		i;
	char	*buf;

	buf = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buf)
		return (ft_free(strs, NULL));
	i = 1;
	while (i > 0 && !ft_strchr(buf, '\n'))
	{
		i = read(fd, buf, BUFFER_SIZE);
		if (i == -1)
			return (ft_free(strs, buf));
		buf[i] = 0;
		strs = ft_strjoin_gnl(strs, buf, 0);
		if (!strs)
			return (ft_free(buf, NULL));
	}
	if (strs[0] == '\0')
		return (ft_free(strs, buf));
	free(buf);
	return (strs);
}

char	*get_next_line(int fd)
{
	static char	*strs = NULL;
	char		*ret;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	ret = NULL;
	strs = ft_read(strs, fd);
	if (!strs)
		return (NULL);
	ret = ft_get_start(strs, ret);
	if (!ret)
	{
		strs = NULL;
		return (NULL);
	}
	strs = ft_get_end(strs);
	return (ret);
}
