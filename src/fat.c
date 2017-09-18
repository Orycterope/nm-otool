/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 17:39:04 by tvermeil          #+#    #+#             */
/*   Updated: 2017/09/18 17:17:32 by tvermeil         ###   ########.fr       */
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

static t_file_map	parse_fat_32(t_file_map mapping, void *ptr, uint32_t n)
{
	struct fat_arch	*a;

	a = (struct fat_arch *)ptr;
	while (n-- && (void *)a + sizeof(*a) < mapping.addr + mapping.size)
	{
		if (a->offset)
		{
			if ((size_t)R(a->offset) + R(a->size) > mapping.size)
				continue ;
			mapping.size = R(a->size);
			mapping.addr += R(a->offset);
			return (mapping);
		}
	}
	ERROR("The fat file contains no valid architecture");
	ft_bzero(&mapping, sizeof(t_file_map));
	return (mapping);
}

static t_file_map	parse_fat_64(t_file_map mapping, void *ptr, uint32_t n)
{
	struct fat_arch_64	*a;

	a = (struct fat_arch_64 *)ptr;
	while (n-- && (void *)a + sizeof(*a) < mapping.addr + mapping.size)
	{
		if (a->offset)
		{
			if ((size_t)R(a->offset) + R(a->size) > mapping.size)
				continue ;
			mapping.size = R(a->size);
			mapping.addr += R(a->offset);
			return (mapping);
		}
	}
	ERROR("The fat file contains no valid architecture");
	ft_bzero(&mapping, sizeof(t_file_map));
	return (mapping);
}

/*
** This function returns the address of the first entry in a fat file
** If the file is not a fat file, the pointer is returned as-is so this
** function can be called regardless of the file type.
*/

t_file_map			get_fat_entry(t_file_map mapping)
{
	struct fat_header	*header;

	header = (struct fat_header*)mapping.addr;
	g_wrong_endian = 0;
	if (header->magic == FAT_CIGAM || header->magic == FAT_CIGAM_64)
		g_wrong_endian = 1;
	if (header->magic == FAT_MAGIC || header->magic == FAT_CIGAM)
		return (parse_fat_32(mapping,
					mapping.addr + sizeof(*header), R(header->nfat_arch)));
	else if (header->magic == FAT_MAGIC_64 || header->magic == FAT_CIGAM_64)
		return (parse_fat_64(mapping,
					mapping.addr + sizeof(*header), R(header->nfat_arch)));
	else
		return (mapping);
}
