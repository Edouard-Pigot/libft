/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epigot <edouard.pigot@42paris.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 21:08:44 by epigot            #+#    #+#             */
/*   Updated: 2026/10/04 21:08:44 by epigot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	src_len;
	size_t	dst_len;
	int	i;

	src_len = ft_strlen(src);
	dst_len = ft_strlen(dst);
	i = 0;
	if (dsize < dst_len)
		return (src_len + dsize);
	while (src[i] && (dst_len + i) < (dsize - 1))
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len + dst_len);
}
