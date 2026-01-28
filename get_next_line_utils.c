/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matnusko <matnusko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 03:17:39 by matnusko          #+#    #+#             */
/*   Updated: 2025/09/23 18:22:00 by matnusko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strdup(const char *s, int size)
{
	char	*rendu;
	int		i;

	if (size < 0)
		return (NULL);
	i = 0;
	rendu = (char *)malloc(sizeof(char) * (size + 1));
	if (rendu == 0)
		return (0);
	while (i <= size && s[i])
	{
		rendu[i] = s[i];
		i++;
	}
	rendu[i] = '\0';
	return (rendu);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*rendu;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2, ft_strlen(s2)));
	if (!s2)
		return (ft_strdup(s1, ft_strlen(s1)));
	rendu = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!rendu)
		return (NULL);
	i = -1;
	j = 0;
	while (s1[++i] != '\0')
		rendu[i] = s1[i];
	while (s2[j] != '\0')
		rendu[i++] = s2[j++];
	rendu[i] = '\0';
	free(s1);
	return (rendu);
}
