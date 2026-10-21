/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epigot <edouard.pigot@42paris.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 23:18:36 by epigot            #+#    #+#             */
/*   Updated: 2026/10/14 23:34:09 by epigot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*bytes1;
	const unsigned char	*bytes2;

	bytes1 = (const unsigned char *)s1;
	bytes2 = (const unsigned char *)s2;
	while (n)
	{
		if (*bytes1 != *bytes2)
			return (*bytes1 - *bytes2);
		bytes1++;
		bytes2++;
		n--;
	}
	return (0);
}
