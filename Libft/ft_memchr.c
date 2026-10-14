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
	unsigned char	uc;
	unsigned char	us;

	uc = (unsigned char)c;
	us = *(unsigned char *)s;
	while (n && us)
	{
		if (us == uc)
			return ((void *)s);
		s++;
		us = *(unsigned char *)s;
		n--;
	}
	return (NULL);
}
