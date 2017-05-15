/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_endianness.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/08 21:08:07 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/15 19:58:18 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_write_big_endian(long long n, void *dst, size_t lenght)
{
	size_t	i;
	char	*dest;

	if (lenght == 0)
		return (0);
	dest = (char *)(dst + lenght - 1);
	i = 0;
	while (i < lenght)
	{
		*dest-- = (n & 0xFF);
		n >>= 8;
		i++;
	}
	return (i);
}

long long	ft_read_big_endian(void *src, size_t length)
{
	long long		n;
	long long		power;
	unsigned char	*ptr;

	n = 0;
	power = 1;
	ptr = (unsigned char *)src;
	while (length > 0)
	{
		n += ((long long)ptr[length - 1]) * power;
		power <<= 8;
		length--;
	}
	if ((n & (power >> 1)) != 0)
		while (power != 0x0)
		{
			n |= power;
			power <<= 1;
		}
	return (n);
}

/*
** Returns a copy of the string passed in reversed byte order in long long
*/

long long	ft_reverse_bytes_to_long(void *src, size_t length)
{
	long long		copy;
	unsigned char	*src_char;
	unsigned long	iter;

	src_char = (unsigned char *)src;
	copy = 0;
	iter = 0;
	while (length-- && iter++ < sizeof(long long))
	{
		copy = (copy << 8) | (*src_char++);
	}
	return (copy);
}
