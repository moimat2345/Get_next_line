/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 03:17:42 by matnusko          #+#    #+#             */
/*   Updated: 2025/09/23 21:02:47 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	find_newline(const char *s)
{
	int	i;

	if (!s)
		return (-1);
	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*extract_line(char *buffer)
{
	char	*line;
	int		i;
	int		newline_pos;

	if (!buffer || buffer[0] == '\0')
		return (NULL);
	newline_pos = find_newline(buffer);
	if (newline_pos == -1)
		newline_pos = ft_strlen(buffer) - 1;
	line = malloc(sizeof(char) * (newline_pos + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (i <= newline_pos)
	{
		line[i] = buffer[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*update_buffer(char *buffer)
{
	char	*new_buffer;
	int		newline_pos;
	int		i;
	int		j;

	newline_pos = find_newline(buffer);
	if (!buffer || newline_pos == -1)
	{
		free(buffer);
		return (NULL);
	}
	new_buffer = malloc(sizeof(char) * (ft_strlen(buffer) - newline_pos));
	if (!new_buffer)
		return (NULL);
	i = newline_pos + 1;
	j = 0;
	while (buffer[i])
		new_buffer[j++] = buffer[i++];
	new_buffer[j] = '\0';
	free(buffer);
	return (new_buffer);
}

char	*read_file(int fd, char *buffer)
{
	char	*temp;
	int		bytes_read;

	if (buffer && find_newline(buffer) != -1)
		return (buffer);
	temp = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!temp)
		return (NULL);
	bytes_read = read(fd, temp, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		temp[bytes_read] = '\0';
		buffer = ft_strjoin(buffer, temp);
		if (find_newline(buffer) != -1)
			break ;
		bytes_read = read(fd, temp, BUFFER_SIZE);
	}
	free(temp);
	if (bytes_read < 0 && buffer)
		free(buffer);
	if (bytes_read < 0)
		return (NULL);
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer[MAX_FD];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer[fd] = read_file(fd, buffer[fd]);
	if (!buffer[fd])
		return (NULL);
	line = extract_line(buffer[fd]);
	buffer[fd] = update_buffer(buffer[fd]);
	return (line);
}
