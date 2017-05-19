/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/19 17:24:10 by tvermeil          #+#    #+#             */
/*   Updated: 2017/05/19 19:20:28 by tvermeil         ###   ########.fr       */
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
		void	(callback)(void *))
{
	uint32_t			ncmds;
	struct load_command *lc;

	ncmds = parse_header(&map);
	while (ncmds)
	{
		lc = (struct load_command *)map.addr;
		if (map.size < sizeof(*lc) || map.size < lc->cmdsize)
		{
			ft_putendl_fd("This file seems to have been truncated", 2);
			break ;
		}
		if (lc->cmd == cmd)
			callback(map.addr);
		map.size -= lc->cmdsize;
		map.addr += lc->cmdsize;
		ncmds--;
	}
}
