/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 17:39:04 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/15 19:57:13 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "endian.h"
#include <mach-o/fat.h>

/*
** This file contains the functions dealing with fat file format
** If the file contains no valid entry exit.
*/

/*
** Return the first entry of this fat file. If no entry is available
** or if no entry is valid exit.
*/

static void	*parse_fat_32(void *map_addr, void *ptr, uint32_t n)
{
	struct fat_arch	*a;

	a = (struct fat_arch *)ptr;
	while (n--)
	{
		if (a->offset)
			return (map_addr + R(a->offset));
	}
	ERROR("The fat file contains no valid architecture");
	return (NULL);
}

static void	*parse_fat_64(void *map_addr, void *ptr, uint32_t n)
{
	struct fat_arch_64	*a;

	a = (struct fat_arch_64 *)ptr;
	while (n--)
	{
		if (a->offset)
			return (map_addr + R(a->offset));
	}
	ERROR("The fat file contains no valid architecture");
	return (NULL);
}

/*
** This function returns the address of the first entry in a fat file
** If the file is not a fat file, the pointer is returned as-is so this
** function can be called regardless of the file type.
*/

void		*get_fat_entry(void *map_addr)
{
	struct fat_header	*header;

	header = (struct fat_header*)map_addr;
	g_wrong_endian = 0;
	if (header->magic == FAT_CIGAM || header->magic == FAT_CIGAM_64)
		g_wrong_endian = 1;
	if (header->magic == FAT_MAGIC || header->magic == FAT_CIGAM)
		return (parse_fat_32(map_addr,
					map_addr + sizeof(*header), R(header->nfat_arch)));
	else if (header->magic == FAT_MAGIC_64 || header->magic == FAT_CIGAM_64)
		return (parse_fat_64(map_addr,
					map_addr + sizeof(*header), R(header->nfat_arch)));
	else
		return (map_addr);
}
