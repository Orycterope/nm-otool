/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/19 17:24:10 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/26 16:53:23 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include "endian.h"
#include <mach-o/loader.h>

/*
** Parse the header of the mach-o file, change the map size and address, and
** return the number of load_commands.
*/

static uint32_t	parse_header(t_file_map *map)
{
	struct mach_header		*header;
	size_t					header_size;

	header = (struct mach_header *)map->addr;
	if (header->magic == MH_CIGAM || header->magic == MH_CIGAM_64)
		g_wrong_endian = 1;
	else
		g_wrong_endian = 0;
	if (R(header->magic) == MH_MAGIC)
		header_size = sizeof(struct mach_header);
	else if (R(header->magic) == MH_MAGIC_64)
		header_size = sizeof(struct mach_header_64);
	else
	{
		ft_putendl_fd("This is not a mach-o file", 2);
		return (0);
	}
	if (map->size < R(header->sizeofcmds) + header_size)
	{
		ft_putendl_fd("This file seems to have been truncated", 2);
		return (0);
	}
	map->size -= header_size;
	map->addr += header_size;
	return (R(header->ncmds));
}

/*
** Calls the callback function everytime a load_command of type -cmd- is
** encountered.
** The callback function takes the address of the load_command as param
*/

void			get_load_commands(t_file_map map, uint32_t cmd,
	void (callback)(void *, void *), void *data)
{
	uint32_t			ncmds;
	struct load_command *lc;

	ncmds = parse_header(&map);
	while (ncmds)
	{
		lc = (struct load_command *)map.addr;
		if (map.size < sizeof(*lc) || map.size < (size_t)R(lc->cmdsize))
		{
			ft_putendl_fd("This file seems to have been truncated", 2);
			break ;
		}
		if (R(lc->cmd) == cmd)
			callback(map.addr, data);
		map.size -= R(lc->cmdsize);
		map.addr += R(lc->cmdsize);
		ncmds--;
	}
}

/*
** Gets the n-th section of the mach-o file
** Returns NULL if there is no such section.
*/
struct section	*get_section_by_number(t_file_map map, int n)
{
	uint32_t				ncmds;
	struct segment_command	*lc;
	int						s;

	ncmds = parse_header(&map);
	while (ncmds-- && n > 0)
	{
		lc = (struct segment_command *)map.addr;
		if ((R(lc->cmd) == LC_SEGMENT && (s = R(lc->nsects)))
			|| (R(lc->cmd) == LC_SEGMENT_64
			&& (s = R(((struct segment_command_64*)lc)->nsects))))
		{
			if (s >= n && R(lc->cmd) == LC_SEGMENT)
				return ((void *)(lc + 1) + (n - 1) * sizeof(struct section));
			else if (s >= n)
				return ((void *)(lc) + sizeof(struct segment_command_64)
						+ (n - 1) * sizeof(struct section_64));
			n -= s;
		}
		map.size -= lc->cmdsize;
		map.addr += lc->cmdsize;
	}
	return (NULL);
}

t_file_map		get_section_in_segment(struct segment_command *lc,
	const char *sect_name, t_file_map file)
{
	struct section				*s;
	int							n;
	int							is_32;

	is_32 = R(lc->cmd) == LC_SEGMENT ? 1 : 0;
	n = is_32 ? R(lc->nsects) : R(((struct segment_command_64*)lc)->nsects);
	s = is_32 ? lc + 1 : ((void *)lc) + sizeof(struct segment_command_64);
	while (n--)
	{
		if (ft_strncmp(sect_name, s->sectname, 16) == 0)
		{
			file.addr += is_32 ? s->offset : ((struct section_64 *)s)->offset;
			file.size = is_32 ? s->size : ((struct section_64 *)s)->size;
			return (file);
		}
	}
	ft_bzero(&file, sizeof(file));
	return (file);
}

/*
** Gets the section -sect_name- in -seg_name-
** If nothing is found returns a t_file_map pointing to NULL
*/
t_file_map		get_section_by_name(t_file_map map, const char *seg_name,
	const char *sect_name)
{
	uint32_t				ncmds;
	struct segment_command	*lc;
	t_file_map				orig_map;

	orig_map = map;
	ncmds = parse_header(&map);
	while (ncmds--)
	{
		lc = (struct segment_command *)map.addr;
		if (R(lc->cmd) == LC_SEGMENT || R(lc->cmd) == LC_SEGMENT_64)
		{
			if (ft_strncmp(seg_name, lc->segname, 16) == 0)
				return (get_section_in_segment(lc, sect_name, orig_map));
		}
		map.size -= lc->cmdsize;
		map.addr += lc->cmdsize;
	}
	ft_bzero(&map, sizeof(map));
	return (map);
}
