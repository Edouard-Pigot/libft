/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epigot <edouard.pigot@42paris.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 23:12:44 by epigot            #+#    #+#             */
/*   Updated: 2026/10/14 23:33:03 by epigot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*bytes;
	unsigned char		uc;

	uc = (unsigned char)c;
	bytes = (const unsigned char *)s;
	while (n)
	{
		if (*bytes == uc)
			return ((void *)bytes);
		bytes++;
		n--;
	}
	return (NULL);
}
