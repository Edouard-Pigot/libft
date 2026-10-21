/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epigot <edouard.pigot@42paris.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/17 18:09:43 by epigot            #+#    #+#             */
/*   Updated: 2026/10/17 22:01:28 by epigot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!*little)
		return ((char *)big);
	while (i < len && big[i])
	{
		while (j < len - i && little[j] && big[i + j]
			&& little[j] == big[i + j])
		{
			j++;
		}
		if (!little[j])
			return ((char *)big + i);
		j = 0;
		i++;
	}
	return (NULL);
}