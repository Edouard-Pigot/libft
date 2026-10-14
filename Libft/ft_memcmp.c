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
	int						result;
	unsigned char	us1;
	unsigned char	us2;

	result = 0;
	us1 = *(unsigned char *)s1;
	us2 = *(unsigned char *)s2;
	while (n && us1 && us2)
	{
		result += (us1 - us2);
		s1++;
		s2++;
		us1 = *(unsigned char *)s1;
		us2 = *(unsigned char *)s2;
		n--;
	}
	return (result);
}
